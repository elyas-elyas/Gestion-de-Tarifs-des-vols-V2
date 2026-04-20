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

echo === Generation de la documentation Doxygen ===
doxygen Doxyfile

echo === Creation du raccourci HTML ===
(
    echo ^<meta http-equiv="REFRESH" content="0;URL=html/index.html"^>
) > doc\Documentation.html

echo === Compilation et documentation terminees avec succes ! ===
pause