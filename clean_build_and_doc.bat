@echo off
cd /d "%~dp0"

echo === Nettoyage du dossier de compilation CMake ===
if exist build rmdir /s /q build

echo === Suppression de la documentation Doxygen ===
if exist doc\html rmdir /s /q doc\html
if exist doc\Documentation.html del /q doc\Documentation.html

echo === Nettoyage complet termine ! ===
pause