<?php
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);
header("Access-Control-Allow-Origin: *");
header("Content-Type: application/json; charset=UTF-8");
header("Access-Control-Allow-Methods: GET, POST, PUT, DELETE");
header("Access-Control-Allow-Headers: Content-Type, Authorization");
header("Access-Control-Allow-Methods: GET");

// Inclure le fichier de connexion à la base de données
require_once ('obj/connexionBdd.php');

// Vérifier si les IDs de l'utilisateur et de la scène sont fournis
if (isset($_GET['id'])) {
     // Récupération des données depuis la requête GET
     $idLightBoard = $_GET['id'];

    try {
        // Préparer et exécuter la requête SQL pour supprimer la scène de la lightBoard
        $sql = "DELETE FROM lightBoard WHERE id = ?";
            $stmt = $pdo->prepare($sql);
            $stmt->execute([$idLightBoard]);

        echo "Scene deleted from lightBoard successfully";
    } catch (PDOException $e) {
        echo "Error deleting scene from lightBoard: " . $e->getMessage();
    }
} else {
    echo "No user ID or scene ID provided";
}
?>