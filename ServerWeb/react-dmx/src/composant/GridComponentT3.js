import React, { useState, useEffect } from 'react';
import './GridComponentT3.css';

const GridComponentT3 = () => {
  const [scenes, setScenes] = useState([]);
  const [gridData, setGridData] = useState([]);
  const [mode, setMode] = useState('configuration');
  const [idUser, setUser] = useState(12);
  const [draggedGridDataId, setDraggedGridDataId] = useState(null);

  useEffect(() => {
    setUser(12);
    fetchDataFromAPI(idUser).then(data => {
      setGridData(data);
    });
    fetch('http://192.168.65.91/ProjetDMX/CodeDMX/scenes.php?userId=${idUser}')
      .then(response => response.json())
      .then(data => {
        setScenes(data);
      })
      .catch(error => {
        console.error('Erreur lors de la récupération des scènes:', error);
      });
  }, []);

  const handleCellClick = (cellData) => {
    alert(`Envoi de l'ID de la scène : ${cellData.idScene}, (${cellData.nom}) vers l'API`);
  };

  const handleCellDragStart = (event, cellId) => {
    const draggedCellData = gridData.find(cell => cell.id === cellId);
    if (draggedCellData) {
      setDraggedGridDataId(draggedCellData.id);
    }
  };

  const handleCellDragStartList = (event) => { };
  const handleCellDragOverList = (event) => { };
  const handleCellDropList = (event) => { };

  const handleCellDragOver = (event) => {
    event.preventDefault();
  };

  const handleCellDrop = async (event, targetCellId) => {
    event.preventDefault();
    const draggedCellId = event.dataTransfer.getData('text/plain');
    const targetX = targetCellId[5];
    const targetY = targetCellId[7];

    if (![0, 1, 2].includes(parseInt(targetX)) || ![0, 1, 2].includes(parseInt(targetY))) {
      return;
    }

    await updateCellInDatabase(draggedGridDataId, targetX, targetY);

    fetchDataFromAPI(12).then(data => {
      setGridData(data);
    });
  };

  const toggleMode = () => {
    setMode(prevMode => (prevMode === 'studio' ? 'configuration' : 'studio'));
  };

  return (
    <div className="scene-list-container">
      <h3>Liste des Scènes</h3>
      <div className="scene-scroll-container">
        {scenes.map(scene => (
          <div
            key={scene.id}
            className="grid-cell"
            draggable={mode === 'configuration'}
            onDragStart={(event) => handleCellDragStartList(event)}
            onDragOver={(event) => handleCellDragOverList(event)}
            onDrop={(event) => handleCellDropList(event)}
          >
            {scene.nom}
          </div>
        ))}
      </div>
    </div>
  );
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

const updateCellInDatabase = async (id, newX, newY) => {
  try {
    const response = await fetch(`http://192.168.65.91/ProjetDMX/CodeDMX/updatePosition.php?id=${id}&x=${newX}&y=${newY}`, {
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

export default GridComponentT3;
