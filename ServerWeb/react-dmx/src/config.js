export const ipAddress = "192.168.65.91"; // Adresse IP à utiliser
export const fetchSceneFromAPI = async (idUser) => {
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
