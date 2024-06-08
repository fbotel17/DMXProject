import { ipAddress, fetchSceneFromAPI } from '../config';

class ConfigurerLightBoard {
  constructor(idUser) {
    this.idUser = idUser;
  }

  async configurer(sceneData) {
    try {
      const response = await fetch(`http://${ipAddress}/ProjetDMX/CodeDMX/configurerLightBoard.php`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ idUser: this.idUser, sceneData }),
      });
      if (!response.ok) {
        throw new Error('Failed to configure lightboard');
      }
      const result = await response.json();
      return result;
    } catch (error) {
      console.error('Error configuring lightboard:', error);
      return null;
    }
  }

  async fetchScenes() {
    return await fetchSceneFromAPI(this.idUser);
  }
}

export default ConfigurerLightBoard;
