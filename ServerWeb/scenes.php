<?php
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);
header("Access-Control-Allow-Origin: *");
header("Content-Type: application/json; charset=UTF-8");
header("Access-Control-Allow-Methods: GET, POST, PUT, DELETE");
header("Access-Control-Allow-Headers: Content-Type, Authorization");

// Inclusion du fichier de connexion à la base de données
require_once ('obj/connexionBdd.php');

try {
    if (isset($_GET['userId'])) {
        $idUser = $_GET['userId'];

        // Requête SQL pour sélectionner toutes les lignes sauf celles où idUser = $idUser
        $sql = "SELECT id, nom FROM scene WHERE id NOT IN (SELECT idScene FROM lightBoard WHERE idUser = ?)";
        $stmt = $pdo->prepare($sql);
        $stmt->execute([$idUser]);

        // Récupérer tous les résultats
        $results = $stmt->fetchAll(PDO::FETCH_ASSOC);

        // Réponse JSON avec les résultats
        echo json_encode($results);
    } else {
        $sql = "SELECT id, nom FROM scene";
        $stmt = $pdo->prepare($sql);
        $stmt->execute();

        // Récupérer tous les résultats
        $results = $stmt->fetchAll(PDO::FETCH_ASSOC);

        // Réponse JSON avec les résultats
        echo json_encode($results);
    }
} catch (PDOException $e) {
    echo 'Erreur : ' . $e->getMessage();
}

// Fermeture de la connexion à la base de données
$pdo = null;
