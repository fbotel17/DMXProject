-- phpMyAdmin SQL Dump
-- version 5.2.0
-- https://www.phpmyadmin.net/
--
-- Hôte : localhost
-- Généré le : mer. 12 juin 2024 à 10:07
-- Version du serveur : 10.5.21-MariaDB-0+deb11u1
-- Version de PHP : 7.4.33

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de données : `testCodeDMX`
--

-- --------------------------------------------------------

--
-- Structure de la table `canaux`
--

CREATE TABLE `canaux` (
  `numCanal` int(11) NOT NULL,
  `idScene` int(11) NOT NULL,
  `valeur` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Déchargement des données de la table `canaux`
--

INSERT INTO `canaux` (`numCanal`, `idScene`, `valeur`) VALUES
(4, 475, 0),
(5, 475, 0),
(6, 475, 255),
(7, 475, 0),
(8, 475, 255),
(9, 475, 255),
(10, 475, 0),
(11, 475, 0),
(50, 488, 131),
(51, 488, 85),
(52, 488, 245),
(53, 488, 0),
(54, 488, 137),
(55, 488, 42),
(64, 490, 87),
(65, 490, 70),
(66, 490, 245),
(67, 490, 87),
(68, 490, 255),
(4, 485, 0),
(5, 485, 0),
(6, 485, 255),
(7, 485, 0),
(8, 485, 0),
(9, 485, 0),
(10, 485, 255),
(11, 485, 0),
(12, 485, 0),
(13, 485, 0),
(14, 485, 255),
(15, 485, 0),
(4, 487, 0),
(5, 487, 255),
(6, 487, 0),
(7, 487, 0),
(4, 486, 255),
(5, 486, 0),
(6, 486, 0),
(7, 486, 0),
(8, 486, 255),
(9, 486, 0),
(10, 486, 0),
(11, 486, 0),
(12, 486, 255),
(13, 486, 0),
(14, 486, 0),
(15, 486, 0),
(4, 499, 186),
(5, 499, 107),
(6, 499, 244),
(7, 499, 11),
(8, 499, 192),
(9, 499, 129),
(10, 499, 50),
(11, 499, 184),
(12, 499, 231),
(13, 499, 170),
(14, 499, 105),
(15, 499, 200),
(4, 512, 0),
(5, 512, 255),
(6, 512, 255),
(7, 512, 0),
(8, 512, 255),
(9, 512, 0),
(10, 512, 255),
(11, 512, 0);

-- --------------------------------------------------------

--
-- Structure de la table `champ`
--

CREATE TABLE `champ` (
  `id` int(11) NOT NULL,
  `idEquip` int(11) NOT NULL,
  `idNumCanal` int(11) NOT NULL,
  `nom` varchar(25) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Déchargement des données de la table `champ`
--

INSERT INTO `champ` (`id`, `idEquip`, `idNumCanal`, `nom`) VALUES
(4, 6, 4, 'rouge'),
(5, 6, 5, 'vert'),
(6, 6, 6, 'bleu'),
(7, 6, 7, 'blanc'),
(8, 7, 8, 'rouge'),
(9, 7, 9, 'vert'),
(10, 7, 10, 'bleu'),
(11, 7, 11, 'blanc'),
(12, 8, 12, 'rouge'),
(13, 8, 13, 'vert'),
(14, 8, 14, 'bleu'),
(15, 8, 15, 'blanc'),
(16, 9, 16, 'intensite'),
(17, 9, 17, 'rouge'),
(18, 9, 18, 'vert'),
(19, 9, 19, 'bleu'),
(20, 9, 20, 'blanc'),
(21, 9, 21, 'strobo'),
(22, 9, 22, 'mode'),
(23, 9, 23, 'effets'),
(141, 164, 50, 'diaph'),
(142, 164, 51, 'gobo'),
(143, 164, 52, 'rota gobo'),
(144, 164, 53, 'couleur'),
(145, 164, 54, 'rota'),
(146, 164, 55, 'inclinaison'),
(147, 173, 64, 'shutter'),
(148, 173, 65, 'gobo'),
(149, 173, 66, 'color'),
(150, 173, 67, 'pan'),
(151, 173, 68, 'rota');

-- --------------------------------------------------------

--
-- Structure de la table `equipement`
--

CREATE TABLE `equipement` (
  `id` int(11) NOT NULL,
  `nom` varchar(20) NOT NULL,
  `adresse` int(11) NOT NULL,
  `nbCanal` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Déchargement des données de la table `equipement`
--

INSERT INTO `equipement` (`id`, `nom`, `adresse`, `nbCanal`) VALUES
(6, 'saber1', 4, 4),
(7, 'saber2', 8, 4),
(8, 'saber3', 12, 4),
(9, 'ghost1', 16, 8),
(164, 'twist-152', 50, 6),
(173, 'scanner', 64, 5);

-- --------------------------------------------------------

--
-- Structure de la table `lightBoard`
--

CREATE TABLE `lightBoard` (
  `id` int(11) NOT NULL,
  `idUser` int(11) NOT NULL,
  `x` int(11) NOT NULL,
  `y` int(11) NOT NULL,
  `onOff` int(1) NOT NULL DEFAULT 0,
  `idScene` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Déchargement des données de la table `lightBoard`
--

INSERT INTO `lightBoard` (`id`, `idUser`, `x`, `y`, `onOff`, `idScene`) VALUES
(1554, 12, 0, 0, 0, 187),
(1556, 12, 1, 0, 0, 475),
(1596, 2, 0, 0, 1, 187),
(1597, 2, 0, 2, 0, 475),
(1598, 2, 1, 2, 0, 499),
(1601, 2, 0, 1, 0, 487),
(1602, 2, 1, 1, 0, 486),
(1603, 2, 1, 0, 0, 488),
(1604, 2, 2, 0, 0, 490),
(1605, 2, 2, 1, 0, 512);

-- --------------------------------------------------------

--
-- Structure de la table `scene`
--

CREATE TABLE `scene` (
  `id` int(11) NOT NULL,
  `nom` varchar(25) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Déchargement des données de la table `scene`
--

INSERT INTO `scene` (`id`, `nom`) VALUES
(187, 'eteindre'),
(475, 'la providence'),
(499, 'multicolor'),
(512, 'revueFinale'),
(485, 'sceneBleu'),
(486, 'sceneRouge'),
(487, 'sceneVert'),
(490, 'testScan'),
(488, 'test_twist');

-- --------------------------------------------------------

--
-- Structure de la table `user`
--

CREATE TABLE `user` (
  `id` int(11) NOT NULL,
  `nom` varchar(50) NOT NULL,
  `prenom` varchar(50) NOT NULL,
  `email` varchar(50) NOT NULL,
  `password` varchar(50) NOT NULL,
  `isAdmin` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Déchargement des données de la table `user`
--

INSERT INTO `user` (`id`, `nom`, `prenom`, `email`, `password`, `isAdmin`) VALUES
(1, 'lopes', 'damien', 'damienlopes@gmail.com', 'dadadu', 1),
(2, 'Leduc', 'Jean', 'jeanleduc@gmail.com', 'varna', 1),
(3, 'lopes', 'damien', 'damienlopes@gmail.com', 'dadadu', 1),
(4, 'lefevre', 'tom', 'tomlefevre@gmail.com', 'toto', 1),
(5, 'Gina', 'Laurent', 'Laurentgina@gmail.com', 'orangina', 0),
(6, 'Mentation', 'Ali', 'alimentation', 'corsair', 0),
(7, 'Lerdorf', 'Rasmus', 'Rasmuslerdorf', 'creatotphp@gmail.com', 0),
(8, 'lepretre', 'alexandre', 'lepretrealexandre@gmail.com', 'lepretre', 0),
(9, 'Senepart', 'mathias', 'blancdeblanc@gmail.com', 'messi', 0),
(10, 'tiennot', 'tibo', 'tibotiennot@gmail.com', 'OMBest', 0),
(11, 'Hautemaniere', 'Edouard', 'edouardhaute@gmail.com', 'doudou', 0),
(12, 'Briaux', 'simon', 'simonbriaux@gmail.com', 'briauxS', 0),
(13, 'admin', 'admin', 'admin@gmail.com', 'admin', 1),
(14, 'bilhaut', 'theo', 'Btheo@gmail.com', 'btheo', 0),
(15, 'tabary', 'hugo', 'hugotabary@gmail.com', 'hugot', 0);

--
-- Index pour les tables déchargées
--

--
-- Index pour la table `canaux`
--
ALTER TABLE `canaux`
  ADD KEY `idScene` (`idScene`),
  ADD KEY `numCanal` (`numCanal`);

--
-- Index pour la table `champ`
--
ALTER TABLE `champ`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `numCanal` (`idNumCanal`),
  ADD UNIQUE KEY `IdnumCanal` (`idNumCanal`),
  ADD UNIQUE KEY `IdNumCanal_2` (`idNumCanal`),
  ADD UNIQUE KEY `idNumCanal_3` (`idNumCanal`),
  ADD KEY `idcanaux` (`idEquip`);

--
-- Index pour la table `equipement`
--
ALTER TABLE `equipement`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `nom` (`nom`);

--
-- Index pour la table `lightBoard`
--
ALTER TABLE `lightBoard`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `idUser_2` (`idUser`,`x`,`y`),
  ADD UNIQUE KEY `idUser_3` (`idUser`,`idScene`),
  ADD KEY `idUser` (`idUser`,`idScene`),
  ADD KEY `idScene` (`idScene`);

--
-- Index pour la table `scene`
--
ALTER TABLE `scene`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `nom` (`nom`);

--
-- Index pour la table `user`
--
ALTER TABLE `user`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT pour les tables déchargées
--

--
-- AUTO_INCREMENT pour la table `champ`
--
ALTER TABLE `champ`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=163;

--
-- AUTO_INCREMENT pour la table `equipement`
--
ALTER TABLE `equipement`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=194;

--
-- AUTO_INCREMENT pour la table `lightBoard`
--
ALTER TABLE `lightBoard`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=1606;

--
-- AUTO_INCREMENT pour la table `scene`
--
ALTER TABLE `scene`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=513;

--
-- AUTO_INCREMENT pour la table `user`
--
ALTER TABLE `user`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=39477;

--
-- Contraintes pour les tables déchargées
--

--
-- Contraintes pour la table `canaux`
--
ALTER TABLE `canaux`
  ADD CONSTRAINT `canaux_ibfk_1` FOREIGN KEY (`idScene`) REFERENCES `scene` (`id`),
  ADD CONSTRAINT `canaux_ibfk_2` FOREIGN KEY (`numCanal`) REFERENCES `champ` (`idNumCanal`);

--
-- Contraintes pour la table `champ`
--
ALTER TABLE `champ`
  ADD CONSTRAINT `Champ_ibfk_1` FOREIGN KEY (`idEquip`) REFERENCES `equipement` (`id`);

--
-- Contraintes pour la table `lightBoard`
--
ALTER TABLE `lightBoard`
  ADD CONSTRAINT `lightBoard_ibfk_1` FOREIGN KEY (`idUser`) REFERENCES `user` (`id`),
  ADD CONSTRAINT `lightBoard_ibfk_2` FOREIGN KEY (`idScene`) REFERENCES `scene` (`id`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
