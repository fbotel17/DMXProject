import React, { useState, useEffect, useRef } from 'react';
import './GridComponentT7.css';

const GridComponentT7 = () => {
    const [gridData, setGridData] = useState([]);
    const [mode, setMode] = useState('studio');
    const [idUser, setUser] = useState(12);
    const [draggedGridDataId, setDraggedGridDataId] = useState(null);
    const [idNewScene, setIdNewScene] = useState(null);
    const webSocket = useRef(null);
    const [scenes, setScenes] = useState([]);

    useEffect(() => {
        setUser(12); // Setting user ID to 12
        fetchDataFromAPI(idUser).then(data => {
            setGridData(data);
        });
        fetchSceneFromAPI(idUser).then(data => {
            setScenes(data);
        });

        // Configurer la connexion WebSocket
        webSocket.current = new WebSocket('ws://192.168.64.170:12346'); // Remplacer par l'URL de votre serveur WebSocket
        webSocket.current.onopen = () => {
            console.log('WebSocket connection established');
        };

        webSocket.current.onclose = () => {
            console.log('WebSocket connection closed');
        };

        return () => {
            if (webSocket.current) {
                webSocket.current.close();
            }
        };
    }, [idUser]);

    const handleCellClick = (cellData) => {
        if (mode === 'studio' && cellData) {
            if (webSocket.current && webSocket.current.readyState === WebSocket.OPEN) {
                // Envoyer l'ID de la scène au serveur WebSocket
                webSocket.current.send(JSON.stringify({ idScene: cellData.idScene }));

                // Mettre à jour la base de données pour mettre onOff à 1 pour l'ID de la lightBoard
                // et mettre à 0 pour tous les autres pour cet ID utilisateur
                updateOnOffInDatabase(cellData.id, idUser)
                    .then(() => {
                        console.log('onOff updated successfully');

                        // Rafraîchir la grille après la mise à jour de la base de données
                        fetchDataFromAPI(idUser).then(data => {
                            setGridData(data);
                        });
                    })
                    .catch(error => {
                        console.error('Error updating onOff:', error);
                    });
            } else {
                console.error('WebSocket connection is not open');
            }
        }
    };

    const handleCellDragStart = (event, cellId) => {
        const draggedCellData = gridData.find(cell => cell.id === cellId);
        if (draggedCellData) {
            setDraggedGridDataId(draggedCellData.id);
            event.dataTransfer.setData('text/plain', cellId);
        }
    };

    const handleCellDragOver = (event) => {
        event.preventDefault();
    };

    const handleCellDrop = async (event, targetCellId) => {
        event.preventDefault();
    
        // Vérifier si l'élément est glissé sur la zone de suppression
        if (targetCellId === 'delete-zone' && draggedGridDataId !== null) {
            await deleteSceneFromLightBoard(draggedGridDataId, idUser, setScenes);
        } else {
            const targetX = targetCellId.split('-')[1];
            const targetY = targetCellId.split('-')[2];
    
            if (![0, 1, 2].includes(parseInt(targetX)) || ![0, 1, 2].includes(parseInt(targetY))) {
                return;
            }
    
            if (draggedGridDataId !== null) {
                await updateCellInDatabase(draggedGridDataId, targetX, targetY, idUser);
            } else if (idNewScene !== null) {
                await addSceneOnLightBoard(idNewScene, targetX, targetY, idUser, setScenes);
            }
        }
    
        fetchDataFromAPI(idUser).then(data => {
            setGridData(data);
        });
    };
    
    const handleSceneDragStart = (event, sceneId) => {
        setIdNewScene(sceneId);
        setDraggedGridDataId(null);
    };

    const renderGrid = () => {
        const gridItems = [];
        for (let i = 0; i < 9; i++) {
            const x = String(i % 3);
            const y = String(Math.floor(i / 3));
            const cellData = gridData.find(cell => parseInt(cell.x) === i % 3 && parseInt(cell.y) === Math.floor(i / 3));
            const sceneName = cellData ? cellData.nom : '';

            const cellClassName = `grid-cell ${cellData && cellData.onOff === "1" ? 'highlighted' : 's'}`;

            const cellId = `cell-${x}-${y}`;

            gridItems.push(
                <div
                    key={i}
                    id={cellId}
                    className={cellClassName}
                    draggable={mode === 'configuration'}
                    onClick={() => handleCellClick(cellData)}
                    onDragStart={(event) => handleCellDragStart(event, cellData?.id)}
                    onDragOver={(event) => handleCellDragOver(event)}
                    onDrop={(event) => handleCellDrop(event, cellId)}
                >
                    {sceneName}
                </div>
            );
        }
        return gridItems;
    };

    const toggleMode = () => {
        setMode(prevMode => (prevMode === 'studio' ? 'configuration' : 'studio'));
    };

    const deleteAllLightBoards = async () => {
        try {
            const response = await fetch(`http://192.168.65.91/ProjetDMX/CodeDMX/deleteAllLightBoards.php?idUser=${idUser}`, {
                method: 'GET',
                headers: {
                    'Content-Type': 'application/json',
                },
            });

            if (!response.ok) {
                throw new Error('Failed to delete all lightboards');
            }

            console.log('All lightboards deleted successfully');

            // Rafraîchir la grille après suppression
            fetchDataFromAPI(idUser).then(data => {
                setGridData(data);
            });
        } catch (error) {
            console.error('Error deleting all lightboards:', error);
        }
    };

    return (
        <div> Module de test N°7 : Ajout de des listes de lumiere
            <h2>Mode {mode === 'studio' ? 'Studio' : 'Configuration'}</h2>
            {mode === 'configuration' && (
                <div
                    id="scene-list-container"
                    className="scene-list-container"
                >
                    <h3>Liste des Scènes</h3>
                    <div className="scene-scroll-container">
                        {scenes.map(scene => (
                            <div
                                key={scene.id}
                                id={`scene-${scene.id}`}
                                className="grid-cell"
                                draggable={mode === 'configuration'}
                                onDragStart={(event) => handleSceneDragStart(event, scene.id)}
                            >
                                {scene.nom}
                            </div>
                        ))}
                    </div>
                </div>
            )}
            <div className="grid-container" onDragOver={(event) => handleCellDragOver(event)} onDrop={(event) => handleCellDrop(event, 'grid-container')}>
                {renderGrid()}
            </div>
            {mode === 'configuration' && (
                <div
                    id="delete-zone"
                    className="delete-zone"
                    onDragOver={(event) => handleCellDragOver(event)}
                    onDrop={(event) => handleCellDrop(event, 'delete-zone')}
                >
                    🗑️
                </div>
            )}
            <button onClick={toggleMode}>Mode {mode === 'studio' ? 'Configuration' : 'Studio'}</button>
            {mode === 'configuration' && (
                <button onClick={deleteAllLightBoards}>Supprimer toutes les lightboards</button>
            )}
        </div>
    );
};

const fetchSceneFromAPI = async (idUser) => {
    try {
        const response = await fetch(`http://192.168.65.91/ProjetDMX/CodeDMX/scenes.php?userId=${idUser}`);
        if (!response.ok) {
            throw new Error('Failed to fetch data from API');
        }
        const data = await response.json();
        return data;
    } catch (error) {
        console.error('Error fetching data:', error);
        return [];
    }
};

const fetchDataFromAPI = async (userId) => {
    try {
        const response = await fetch(`http://192.168.65.91/ProjetDMX/CodeDMX/getGrille.php?userId=${userId}`);
        if (!response.ok) {
            throw new Error('Failed to fetch data from API');
        }
        const data = await response.json();
        return data;
    } catch (error) {
        console.error('Error fetching data:', error);
        return [];
    }
};

const updateCellInDatabase = async (idScene, newX, newY, idUser) => {
    try {
        const response = await fetch(`http://192.168.65.91/ProjetDMX/CodeDMX/updatePosition.php?idUser=${idUser}&idScene=${idScene}&x=${newX}&y=${newY}`, {
            method: 'GET',
            headers: {
                'Content-Type': 'application/json',
            },
        });

        if (!response.ok) {
            throw new Error('Failed to update cell in database');
        }

        console.log('Cell updated in database successfully');
    } catch (error) {
        console.error('Error updating cell in database:', error);
    }
};

const addSceneOnLightBoard = async (idScene, newX, newY, idUser, setScenes) => {
    try {
        const response = await fetch(`http://192.168.65.91/ProjetDMX/CodeDMX/addScene.php?idUser=${idUser}&idScene=${idScene}&x=${newX}&y=${newY}`, {
            method: 'GET',
            headers: {
                'Content-Type': 'application/json',
            },
        });

        if (!response.ok) {
            throw new Error('Failed to add scene to lightboard');
        }

        console.log('Scene added to lightboard successfully');

        // Rafraîchir la liste des scènes après l'ajout
        fetchSceneFromAPI(idUser).then(data => {
            setScenes(data);
        });        
    } catch (error) {
        console.error('Error adding scene to lightboard:', error);
    }
};

const deleteSceneFromLightBoard = async (id, idUser, setScenes) => {
    try {
        const response = await fetch(`http://192.168.65.91/ProjetDMX/CodeDMX/deleteScene.php?id=${id}`, {
            method: 'GET',
            headers: {
                'Content-Type': 'application/json',
            },
        });

        if (!response.ok) {
            throw new Error('Failed to delete scene from lightboard');
        }

        console.log('Scene deleted from lightboard successfully');

        // Rafraîchir la liste des scènes après la suppression
        fetchSceneFromAPI(idUser).then(data => {
            setScenes(data);
        });
    } catch (error) {
        console.error('Error deleting scene from lightboard:', error);
    }
};

const updateOnOffInDatabase = async (cellId, idUser) => {
    try {
        const response = await fetch(`http://192.168.65.91/ProjetDMX/CodeDMX/updateOnOff.php?id=${cellId}&idUser=${idUser}`, {
            method: 'GET',
            headers: {
                'Content-Type': 'application/json',
            },
        });

        if (!response.ok) {
            throw new Error('Failed to update onOff in database');
        }
    } catch (error) {
        throw new Error('Error updating onOff in database:', error);
    }
};

export default GridComponentT7;