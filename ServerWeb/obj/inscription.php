<?php
include "connexionBdd.php";

session_start();

// Formulaire soumis ?
if (isset ($_POST['nom'], $_POST['prenom'], $_POST['email'], $_POST['password'])) {
    // Récupérer les données du formulaire
    $nom = $_POST['nom'];
    $prenom = $_POST['prenom'];
    $email = $_POST['email'];
    $password = $_POST['password'];

    try {
        // Requête sql
        $sql = "INSERT INTO `user` (`nom`, `prenom`, `email`, `password`, `isAdmin`) VALUES (:nom, :prenom, :email, :password, '0')";
        $stmt = $pdo->prepare($sql);
        $stmt->bindParam(":nom", $nom);
        $stmt->bindParam(":prenom", $prenom);
        $stmt->bindParam(":email", $email);
        $stmt->bindParam(":password", $password);

        // Exécution requête
        $stmt->execute();

        // Récupérer l'ID de l'utilisateur inséré
        $user_id = $pdo->lastInsertId();

        // Stocker l'ID de l'utilisateur dans la session
        $_SESSION['user_id'] = $user_id;

        // Redirection
        header("Location: siteDMX.php");
        exit();
    } catch (PDOException $error) {
        $_SESSION['inscriptionError'] = "Erreur lors de l'exécution de la requête";
    }
}