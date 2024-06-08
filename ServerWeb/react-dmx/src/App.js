
import React, { useState } from 'react';
import './App.css';
//import GridComponentT1 from './composant/GridComponentT1';
//import GridComponentT2 from './composant/GridComponentT2';
//import GridComponentT3 from './composant/GridComponentT3';
//import GridComponentT4 from './composant/GridComponentT4';
//import GridComponentT5 from './composant/GridComponentT5';
//import GridComponentT6 from './composant/GridComponentT6';
//import GridComponentT7 from './composant/GridComponentT7';
//import GridComponentT8 from './composant/GridComponentT8';
//import GridComponentT9 from './composant/GridComponentT9';
import StreamDeck from './StreamDeck';
import Login from './Login';

function App() {

  const [userId, setUserId] = useState(null);

  const handleUserId = (id) => {
    setUserId(id);
  };

  const toggleMode = () => {
    setUserId(null);
  };

  return (
    <div className="App">
      {/*<GridComponentT1 />  */}
      {/* <GridComponentT2 /> */}
      {/* <GridComponentT3 /> */}
      {/* <GridComponentT4 /> */}
      {/* <GridComponentT5 /> */}
      {/* <GridComponentT6 /> */}
      {/* <GridComponentT7 /> */}
      {/* <GridComponentT8 /> */}
      {/* <GridComponentT9 /> */}

      {userId ? (<StreamDeck IdUser={userId} />) : (<Login onLoginSuccess={handleUserId} />)}
      {userId ? (<button className="deco-button" onClick={toggleMode}>Deconnexion </button>) : null}
    </div>
  );
}

export default App;
