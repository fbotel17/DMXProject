<?php
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);
header("Access-Control-Allow-Origin: *");
header("Content-Type: application/json; charset=UTF-8");
header("Access-Control-Allow-Methods: GET, POST, PUT, DELETE");
header("Access-Control-Allow-Headers: Content-Type, Authorization");

// Inclusion du fichier de connexion à la base de données
require_once('obj/connexionBdd.php');

if ($_SERVER['REQUEST_METHOD'] === 'GET' && isset($_GET['idUser'])) {
    $idUser = $_GET['idUser'];

    try {
        // Supprimer toutes les lightBoard pour l'utilisateur donné
        $sql = "DELETE FROM lightBoard WHERE idUser = ?";
        $stmt = $pdo->prepare($sql);
        $stmt->execute([$idUser]);

        // Réponse JSON indiquant que la suppression a réussi
        echo json_encode(array("message" => "Toutes les lightBoards ont été supprimées avec succès"));
    } catch (PDOException $e) {
        // En cas d'erreur, renvoyer un message d'erreur JSON
        echo json_encode(array("error" => "Erreur lors de la suppression des lightBoards : " . $e->getMessage()));
    }

    // Fermeture de la connexion à la base de données
    $pdo = null;
} else {
    // Si les paramètres nécessaires ne sont pas fournis, renvoyer une erreur JSON
    echo json_encode(array("error" => "Paramètres invalides ou méthode de requête non autorisée. Seules les requêtes GET avec le paramètre 'idUser' sont acceptées."));
}
?>
