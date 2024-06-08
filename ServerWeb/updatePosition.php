<?php
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);
header("Access-Control-Allow-Origin: *");
header("Content-Type: application/json; charset=UTF-8");
header("Access-Control-Allow-Methods: GET, POST, PUT, DELETE");
header("Access-Control-Allow-Headers: Content-Type, Authorization");

// Vérification de la méthode de la requête
if ($_SERVER['REQUEST_METHOD'] === 'GET') {
    // Inclusion du fichier de connexion à la base de données
    require_once ('obj/connexionBdd.php');

    // Vérification de la présence des paramètres nécessaires
    if (isset($_GET['idScene']) && isset($_GET['x']) && isset($_GET['y']) && isset($_GET['idUser'])) {
        // Récupération des données depuis la requête GET
        $idScene = $_GET['idScene'];
        $newX = $_GET['x'];
        $newY = $_GET['y'];
        $idUser = $_GET['idUser'];

        try {
            // Requête SQL pour sélectionner l'id où x et y sont égaux à newX et newY
            $sqlSelect = "SELECT id FROM lightBoard WHERE idUser = ? AND x = ? AND y = ?";
            $stmtSelect = $pdo->prepare($sqlSelect);
            $stmtSelect->execute([$idUser, $newX, $newY]);

            // Récupérer le résultat
            $result = $stmtSelect->fetch(PDO::FETCH_ASSOC);

            if ($result) {
                $selectedId = $result['id'];

                // Requête SQL pour récupérer les coordonnées x et y de la lightBoard spécifiée
                $sqlSelect3 = "SELECT x, y FROM lightBoard WHERE id = ?";
                $stmtSelect3 = $pdo->prepare($sqlSelect3);
                $stmtSelect3->execute([$idScene]);

                // Récupérer le résultat
                $result3 = $stmtSelect3->fetch(PDO::FETCH_ASSOC);

                if ($result3) {
                    // Stockage des coordonnées dans des variables PHP
                    $currentX = $result3['x'];
                    $currentY = $result3['y'];

                    // Requête SQL pour mettre à jour la position de la scène avec des coordonnées spécifiques (3, 3)
                    $sqlUpdate = "UPDATE lightBoard SET x = 3, y = 3 WHERE id = ?";
                    $stmtUpdate = $pdo->prepare($sqlUpdate);
                    $stmtUpdate->execute([$selectedId]);

                    // Requête SQL pour mettre à jour la position de l'ancienne lightBoard avec les nouvelles coordonnées
                    $sqlUpdateOld = "UPDATE lightBoard SET x = ?, y = ? WHERE id = ?";
                    $stmtUpdateOld = $pdo->prepare($sqlUpdateOld);
                    $stmtUpdateOld->execute([$newX, $newY, $idScene]);

                    // Requête SQL pour mettre à jour la position de l'ancienne lightBoard avec les coordonnées actuelles
                    $sqlUpdateOld2 = "UPDATE lightBoard SET x = ?, y = ? WHERE id = ?";
                    $stmtUpdateOld2 = $pdo->prepare($sqlUpdateOld2);
                    $stmtUpdateOld2->execute([$currentX, $currentY, $selectedId]);

                    // Réponse JSON indiquant que la mise à jour a réussi
                    echo json_encode(array("message" => "Position de la scène mise à jour avec succès"));
                } else {
                    echo json_encode(array("error" => "Scène introuvable avec l'ID spécifié"));
                }
            } else {
                // Requête SQL pour mettre à jour la position de l'ancienne lightBoard avec les nouvelles coordonnées
                $sqlUpdateOld = "UPDATE lightBoard SET x = ?, y = ? WHERE id = ?";
                $stmtUpdateOld = $pdo->prepare($sqlUpdateOld);
                $stmtUpdateOld->execute([$newX, $newY, $idScene]);
            }
        } catch (PDOException $e) {
            // En cas d'erreur, renvoyer un message d'erreur JSON
            echo json_encode(array("error" => "Erreur lors de la mise à jour de la position de la scène : " . $e->getMessage()));
        }

        // Fermeture de la connexion à la base de données
        $pdo = null;
    } else {
        echo json_encode(array("error" => "Paramètres manquants. Les paramètres idScene, x, y, et idUser sont requis."));
    }
} else {
    // Si la méthode de requête n'est pas GET, renvoyer une erreur JSON
    echo json_encode(array("error" => "Méthode de requête non autorisée. Seules les requêtes GET sont acceptées."));
}
?>