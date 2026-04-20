@echo off
cd /d "%~dp0"

echo === Nettoyage du dossier de compilation CMake ===
if exist build rmdir /s /q build

echo === Nettoyage termine ! ===
pause