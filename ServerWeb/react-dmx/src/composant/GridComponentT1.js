import React, { useState, useEffect } from 'react';
import './GridComponentT1.css';

const GridComponentT1 = () => {
  const [gridData, setGridData] = useState([]);
  const [mode, setMode] = useState('studio');

  useEffect(() => {
    fetchDataFromAPI(12).then(data => {
      setGridData(data);
    });
  }, []);

  const renderGrid = () => {
    const gridItems = [];

    for (let i = 0; i < 9; i++) {
      const x = String(i % 3);
      const y = String(Math.floor(i / 3));
      //const cellData = gridData.filter(cell => +cell.x === i % 3 && +cell.y === Math.floor(i / 3));
      const cellData = gridData.find(cell => parseInt(cell.x) === i % 3 && parseInt(cell.y) === Math.floor(i / 3));

     
      //const sceneNames = cellData.map(cell => cell.idScene).join(', '); // Concatène les noms de scène avec une virgule
      const sceneName = cellData ? cellData.nom : ''; // Nom de la scène
      const cellId = `cell-${x}-${y}`;

      // Condition pour déterminer si les clics sont activés ou non
      const onClickHandler = mode === 'studio' ? () => handleCellClick(cellData) : null;


      gridItems.push(
        <div key={i} id={cellId} className="grid-cell" onClick={onClickHandler}>
          {sceneName}
        </div>
      );
    }

    return gridItems;
  };

  const toggleMode = () => {
    setMode(prevMode => (prevMode === 'studio' ? 'configuration' : 'studio'));
  };


  const handleCellClick = (cellData) => {
    // Logique à exécuter lorsqu'une cellule est cliquée en mode studio
    alert(`Envoi de l'ID de la scène : ${cellData.idScene}, ( ${cellData.nom} )  vers l'API `);
  };

  return (
    <div>
        <h2>Mode {mode === 'studio' ? 'Studio' : 'Configuration'}</h2>
        <div className="grid-container">
        {renderGrid()}
        </div>
        <button onClick={toggleMode}>Mode {mode === 'studio' ? 'Configuration' : 'Studio'}</button>
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
    console.log(data);
    return data;
  } catch (error) {
    console.error('Error fetching data:', error);
    return [];
  }
};

export default GridComponentT1;