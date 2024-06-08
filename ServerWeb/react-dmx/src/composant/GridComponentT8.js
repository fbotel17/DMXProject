import React, { useState, useEffect, useRef } from 'react';
import './GridComponentT8.css';

const ipAddress = "192.168.65.91"; // Adresse IP à utiliser
const ipAddressWebSocket = "192.168.64.170:12346"; // Adresse IP WebSocket à utiliser
const $idUserConnect = "12";

const GridComponentT8 = () => {
    const [gridData, setGridData] = useState([]);
    const [mode, setMode] = useState('studio');
    const [idUser, setUser] = useState($idUserConnect);
    const [draggedGridDataId, setDraggedGridDataId] = useState(null);
    const [idNewScene, setIdNewScene] = useState(null);
    const webSocket = useRef(null);
    const [scenes, setScenes] = useState([]);

    useEffect(() => {
        setUser($idUserConnect);
        fetchDataFromAPI(idUser).then(data => {
            setGridData(data);
        });
        fetchSceneFromAPI(idUser).then(data => {
            setScenes(data);
        });

        webSocket.current = new WebSocket(`ws://${ipAddressWebSocket}`);
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
        if (mode === 'studio' && cellData && cellData.idScene) {
            if (webSocket.current && webSocket.current.readyState === WebSocket.OPEN) {
                webSocket.current.send(JSON.stringify({ idScene: cellData.idScene }));
                updateOnOffInDatabase(cellData.id, idUser)
                    .then(() => {
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
        } else {
            console.error('Invalid cellData or missing idScene');
        }
    };

    const handleCellDragStart = (event, cellId) => {
        const draggedCellData = gridData.find(cell => cell.id === cellId);
        if (draggedCellData) {
            setDraggedGridDataId(draggedCellData.id);
            event.dataTransfer.setData('text/plain', cellId);
            event.target.classList.add('dragging');
        }
    };

    const handleCellDragEnd = (event) => {
        event.target.classList.remove('dragging');
    };

    const handleCellDragOver = (event) => {
        event.preventDefault();
    };

    const handleCellDrop = async (event, targetCellId) => {
        event.preventDefault();

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

        event.target.classList.add('dropped');
        setTimeout(() => {
            event.target.classList.remove('dropped');
        }, 500);

        setDraggedGridDataId(null);  // Réinitialiser l'état après le drop
        setIdNewScene(null);  // Réinitialiser également l'id de la nouvelle scène
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

            const cellClassName = `grid-cell ${cellData && cellData.onOff === "1" ? 'highlighted' : ''}`;

            const cellId = `cell-${x}-${y}`;

            gridItems.push(
                <div
                    key={i}
                    id={cellId}
                    className={cellClassName}
                    draggable={mode === 'configuration' && !!cellData}
                    onClick={() => handleCellClick(cellData)}
                    onDragStart={(event) => handleCellDragStart(event, cellData?.id)}
                    onDragEnd={handleCellDragEnd}
                    onDragOver={handleCellDragOver}
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
            const response = await fetch(`http://${ipAddress}/ProjetDMX/CodeDMX/deleteAllLightBoards.php?idUser=${idUser}`, {
                method: 'GET',
                headers: {
                    'Content-Type': 'application/json',
                },
            });

            if (!response.ok) {
                throw new Error('Failed to delete all lightboards');
            }

            console.log('All lightboards deleted successfully');

            fetchDataFromAPI(idUser).then(data => {
                setGridData(data);
            });
        } catch (error) {
            console.error('Error deleting all lightboards:', error);
        }
    };

    const handleDeleteAllLightBoards = async () => {
        await deleteAllLightBoards();
        fetchSceneFromAPI(idUser).then(data => {
            setScenes(data);
        });
    };

    return (
        <div className="container">
            <div className="mode-title">
                Mode {mode === 'studio' ? 'Studio' : 'Configuration'}
            </div>
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
                            >                                {scene.nom}
                            </div>
                        ))}
                    </div>
                </div>
            )}
            <div className="grid-container" onDragOver={handleCellDragOver} onDrop={(event) => handleCellDrop(event, 'grid-container')}>
                {renderGrid()}
            </div>
            {mode === 'configuration' && (
                <div
                    id="delete-zone"
                    className="delete-zone"
                    onDragOver={handleCellDragOver}
                    onDrop={(event) => handleCellDrop(event, 'delete-zone')}
                    onClick={handleDeleteAllLightBoards}
                >
                    🗑️
                </div>
            )}
            <button className="mode-button" onClick={toggleMode}>
                Mode {mode === 'studio' ? 'Configuration' : 'Studio'}
            </button>
        </div>
    );
};

const fetchSceneFromAPI = async (idUser) => {
    try {
        const response = await fetch(`http://${ipAddress}/ProjetDMX/CodeDMX/scenes.php?userId=${idUser}`);
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
        const response = await fetch(`http://${ipAddress}/ProjetDMX/CodeDMX/getGrille.php?userId=${userId}`);
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
        const response = await fetch(`http://${ipAddress}/ProjetDMX/CodeDMX/updatePosition.php?idUser=${idUser}&idScene=${idScene}&x=${newX}&y=${newY}`, {
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
        const response = await fetch(`http://${ipAddress}/ProjetDMX/CodeDMX/addScene.php?idUser=${idUser}&idScene=${idScene}&x=${newX}&y=${newY}`, {
            method: 'GET',
            headers: {
                'Content-Type': 'application/json',
            },
        });

        if (!response.ok) {
            throw new Error('Failed to add scene to lightboard');
        }

        console.log('Scene added to lightboard successfully');

        fetchSceneFromAPI(idUser).then(data => {
            setScenes(data);
        });
    } catch (error) {
        console.error('Error adding scene to lightboard:', error);
    }
};

const deleteSceneFromLightBoard = async (id, idUser, setScenes) => {
    try {
        const response = await fetch(`http://${ipAddress}/ProjetDMX/CodeDMX/deleteScene.php?id=${id}`, {
            method: 'GET',
            headers: {
                'Content-Type': 'application/json',
            },
        });

        if (!response.ok) {
            throw new Error('Failed to delete scene from lightboard');
        }

        console.log('Scene deleted from lightboard successfully');

        fetchSceneFromAPI(idUser).then(data => {
            setScenes(data);
        });
    } catch (error) {
        console.error('Error deleting scene from lightboard:', error);
    }
};

const updateOnOffInDatabase = async (cellId, idUser) => {
    try {
        const response = await fetch(`http://${ipAddress}/ProjetDMX/CodeDMX/updateOnOff.php?id=${cellId}&idUser=${idUser}`, {
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

export default GridComponentT8;
