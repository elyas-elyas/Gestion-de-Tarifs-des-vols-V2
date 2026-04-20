@echo off
cd /d "%~dp0"

echo === Nettoyage et Preparation ===
if exist build rmdir /s /q build
mkdir build
cd build

echo === Configuration CMake ===
cmake ..

echo === Compilation du projet ===
cmake --build .

echo === Execution des tests (CTest) ===
ctest -V
cd ..

echo === Compilation termine avec succes ! ===
pause