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
        // Requête SQL pour récupérer les données de la table 'scene'
       // $sql = 'SELECT * FROM lightBoard WHERE idUser='.$_GET['userId'].'';
       $sql = 'select lightBoard.id AS id, lightBoard.idUser AS idUser, lightBoard.x AS x, lightBoard.y AS y, lightBoard.idScene AS idScene, scene.nom AS nom, lightBoard.onOff AS onOff from lightBoard, scene WHERE lightBoard.idUser='.$_GET['userId'].' AND lightBoard.idScene=scene.id';
        $stmt = $pdo->query($sql);
        $scenes = $stmt->fetchAll(PDO::FETCH_ASSOC);

        // Renvoi des données au format JSON
        echo json_encode($scenes);
    }
} catch (PDOException $e) {
    echo 'Erreur : ' . $e->getMessage();
}

// Fermeture de la connexion à la base de données
$pdo = null;
