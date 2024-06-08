<?php
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);
header("Access-Control-Allow-Origin: *");
header("Content-Type: application/json; charset=UTF-8");
header("Access-Control-Allow-Methods: GET, POST, PUT, DELETE");
header("Access-Control-Allow-Headers: Content-Type, Authorization");

// Vérification de la méthode de la requête
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Inclusion du fichier de connexion à la base de données
    require_once ('obj/connexionBdd.php');

    // Récupération des données depuis la requête POST
    $data = json_decode(file_get_contents("php://input"), true);
    $email = $data['email'];
    $password = $data['password'];

    try {
        // Requête SQL pour vérifier l'utilisateur
        $sqlSelect = "SELECT * FROM user WHERE email = ? AND password = ?";
        $stmtSelect = $pdo->prepare($sqlSelect);
        $stmtSelect->execute([$email, $password]);

        // Récupérer le résultat
        $result = $stmtSelect->fetch(PDO::FETCH_ASSOC);

        if ($result) {
            // Réponse JSON avec succès et l'id de l'utilisateur
            echo json_encode(array("success" => true, "userId" => $result['id']));
        } else {
            // Réponse JSON indiquant que la connexion a échoué
            echo json_encode(array("success" => false, "error" => "login fail"));
        }
    } catch (PDOException $e) {
        // En cas d'erreur, renvoyer un message d'erreur JSON
        echo json_encode(array("success" => false, "error" => "login fail: " . $e->getMessage()));
    }

    // Fermeture de la connexion à la base de données
    $pdo = null;
} else {
    // Si la méthode de requête n'est pas POST, renvoyer une erreur JSON
    echo json_encode(array("error" => "Méthode de requête non autorisée. Seules les requêtes POST sont acceptées."));
}
?>
