<?php
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);
header("Access-Control-Allow-Origin: *");
header("Content-Type: application/json; charset=UTF-8");
header("Access-Control-Allow-Methods: GET, POST, PUT, DELETE");
header("Access-Control-Allow-Headers: Content-Type, Authorization");


// Vérification de la méthode de la requête
if (isset($_GET["id"])) {
    // Inclusion du fichier de connexion à la base de données
    require_once ('obj/connexionBdd.php');


    // Récupération des données depuis la requête GET
    $idLightBoard = $_GET['id'];
    $idUser = $_GET['idUser'];


    try {
        // Requête SQL pour mettre à jour la position de la scène
        $sql = "UPDATE lightBoard SET onOff = (CASE WHEN id = ? THEN 1 ELSE 0 END) WHERE idUser = ?";
        $stmt = $pdo->prepare($sql);
        $stmt->execute([$idLightBoard, $idUser]);

        // Réponse JSON indiquant que la mise à jour a réussi
        echo json_encode(array("message" => "onOff de la scène mise à jour avec succès"));
    } catch (PDOException $e) {
        // En cas d'erreur, renvoyer un message d'erreur JSON
        echo json_encode(array("error" => "Erreur lors de la mise à jour de onOff de la scène : " . $e->getMessage()));
    }

    // Fermeture de la connexion à la base de données
    $pdo = null;
} else {
    // Si la méthode de requête n'est pas GET, renvoyer une erreur JSON
    echo json_encode(array("error" => "Méthode de requête non autorisée. Seules les requêtes GET sont acceptées."));
}
?>