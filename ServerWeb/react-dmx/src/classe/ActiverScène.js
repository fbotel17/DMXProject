import { ipAddress, fetchSceneFromAPI } from '../config';

class ActiverScène {
  constructor(idUser) {
    this.idUser = idUser;
  }

  async activer(idScene) {
    try {
      const response = await fetch(`http://${ipAddress}/ProjetDMX/CodeDMX/activerScene.php?idUser=${this.idUser}&idScene=${idScene}`);
      if (!response.ok) {
        throw new Error('Failed to activate scene');
      }
      const result = await response.json();
      return result;
    } catch (error) {
      console.error('Error activating scene:', error);
      return null;
    }
  }
}

export default ActiverScène;
