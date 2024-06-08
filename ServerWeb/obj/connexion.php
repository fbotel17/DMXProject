<!-- connexion.php -->
<?php
include "connexionBdd.php";

// Formulaire soumis ?
if(isset($_POST['email'], $_POST['password'])) {
    // Récupération des données du formulaire
    $email = $_POST['email'];
    $password = $_POST['password'];

    try {
        // Requête SQL
        $sql = "SELECT `id`, `email`, `password` FROM `user` WHERE email=:email";
        $stmt = $pdo->prepare($sql);
        $stmt->bindParam(":email", $email);
        $stmt->execute();
        $user = $stmt->fetch(PDO::FETCH_ASSOC);

        // L'utilisateur existe-t-il ?
        if($user) {
            // Vérification du mot de passe
            if($password === $user['password']) {
                // Stockage de l'ID de l'utilisateur dans la session
                $_SESSION['user_id'] = $user['id'];

                // Redirection vers la page de succès
                header("Location: siteDMX.php");
                exit();
            } else {
                // Message d'erreur si le mot de passe est incorrect
                $_SESSION['passwordError'] = "Mot de passe incorrect.";
            }
        } else {
            // Message d'erreur si l'utilisateur n'existe pas
            $_SESSION['identifiantsError'] = "Utilisateur non trouvé.";
        }
    } catch(PDOException $error) {
        echo "Erreur lors de l'exécution de la requête : " . $error->getMessage();
    }
}
?>
