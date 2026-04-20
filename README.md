# Projet de Gestion de Tarifs des vols

![C++17](https://img.shields.io/badge/Language-C%2B%2B17-blue)
![Python](https://img.shields.io/badge/Frontend-Python%20Streamlit-yellow)
![Doxygen](https://img.shields.io/badge/Documentation-Doxygen-green)
![Docker](https://img.shields.io/badge/Deploy-Docker-blue)

## Introduction

Ce projet a été développé dans le cadre de gestion de tarifs aériens. Il vise à concevoir un système permettant de :  

- Gérer les tarifs en fonction des classes de réservation et des composantes tarifaires.
- Lire des fichiers CSV contenant des données de vol et de tarification (optimisé via Zero-Copy et Multithreading).
- Analyser et exporter des rapports optimisés pour des analyses stratégiques.
- Visualiser les données de Revenue Management via une interface interactive d'aide à la décision.

## Objectifs

- [x] Modélisation des itinéraires (O&D)
- [x] Gestion des tarifs associés aux itinéraires  
- [x] Association des tarifs aux itinéraires 
- [x] Gestion et analyse des tarifs
- [x] Modélisation des vols
- [x] Lecture des données des vols et des tarifs
- [x] Génération d'un rapport d'analyse
- [x] Optimisation des performances (O(1), Smart Pointers, Multithreading C++17)
- [x] Création d'un Dashboard interactif métier (Streamlit)
- [x] Dockerisation du projet pour un déploiement Cloud-ready

## Prise en main

Pour faciliter l'utilisation et la compilation du projet, plusieurs scripts batch (`.bat`) sont fournis :

### 1. Scripts de compilation et nettoyage (Système C++)
- **`build_and_doc.bat`** : Compile le projet (CMake), lance les tests unitaires et génère la documentation (Doxygen).
- **`build_without_doc.bat`** : Compile le projet (CMake) et lance les tests unitaires sans générer la documentation.
- **`clean_build_and_doc.bat`** : Supprime le dossier de compilation CMake et la documentation générée.
- **`clean_only_build.bat`** : Supprime uniquement le dossier de compilation CMake sans toucher à la documentation.

### 2. Lancement de l'Interface Interactive (Python)
- **`install_python_deps.bat`** : Installe les librairies requises (pandas, streamlit) pour l'interface graphique locale.
- **`run_dashboard.bat`** : Lance le tableau de bord interactif dans votre navigateur.

## Fichier d'entrée

Le projet utilise un fichier CSV en entrée pour lire les données de vol et de tarification.  

- **Fichier par défaut** : [`data/sample.csv`](./data/)
Si vous souhaitez utiliser un autre fichier CSV, utilisez la fonctionnalité d'importation directement intégrée au nouveau Dashboard interactif.

## Résultats

Une fois l'exécution du programme terminée, plusieurs éléments sont générés :

- **Tableau de bord interactif** : Accessible via navigateur, permettant de filtrer les O&D, de visualiser le "Spread" tarifaire (volatilité) et d'exporter des sélections personnalisées.

- **Rapport d'analyse** : Le programme génère un fichier CSV contenant les résultats de l'analyse à partir de `sample.csv`.
  - [rapport_analyse.csv](./data/)

- **Documentation** : La documentation générée par Doxygen est disponible en HTML.
  - [documentation](./doc/) - Pour y acceder lancer le fichier `Documentation.html`
  Il est possible de générer d'autres formats en modifiant les options dans le fichier `Doxyfile`.
**Arborescence du projet final** :

```text
Projet/
├── build/                      
├── data/
│   ├── rapport_analyse.csv
│   ├── sample.csv
├── doc/
│   ├── html/
│   ├── Documentation.html
├── include/
│   ├── FlightNbr.hpp
│   ├── GestionnaireCSV.hpp
│   ├── OD.hpp
│   ├── Tarif.hpp
├── src/
│   ├── FlightNbr.cpp
│   ├── GestionnaireCSV.cpp
│   ├── OD.cpp
│   ├── Tarif.cpp
├── tests/
│   ├── test_FlightNbr.cpp
│   ├── test_GestionnaireCSV.cpp
│   ├── test_OD.cpp
│   ├── test_Tarif.cpp
├── build_and_doc.bat
├── build_without_doc.bat
├── clean_build_and_doc.bat
├── clean_only_build.bat
├── install_python_deps.bat
├── run_dashboard.bat
├── docker-compose.yml
├── Dockerfile
├── dashboard.py
├── CMakeLists.txt
├── Doxyfile
├── main.cpp
└── README.md