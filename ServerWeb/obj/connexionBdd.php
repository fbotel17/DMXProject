<?php

// On essaye de se connecter  à  la BDD
try {
    $ipserver = "192.168.64.213";
    $nomBase = "testCodeDMX";
    $loginPrivilege = "root";
    $passPrivilege = "root";

    $pdo = new PDO('mysql:host='.$ipserver.';dbname='.$nomBase.";charset=utf8mb4",$loginPrivilege,$passPrivilege);
}
catch (Exception $error)
{
    $error->getMessage();
    echo "Erreur BDD : " .$error;
}
?>
