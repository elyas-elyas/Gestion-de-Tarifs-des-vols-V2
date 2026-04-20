# 1. Image de base officielle Python (legere)
FROM python:3.10-slim

# 2. Installation des outils de compilation C++ et CMake
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    && rm -rf /var/lib/apt/lists/*

# 3. Definition du repertoire de travail dans le conteneur
WORKDIR /app

# 4. Copie de tous les fichiers du projet dans le conteneur
COPY . /app/

# 5. Compilation du moteur C++ (directement dans le conteneur Linux)
RUN mkdir -p build && cd build && cmake .. && cmake --build .

# 6. Installation des bibliotheques Python pour le Dashboard
RUN pip install --no-cache-dir streamlit pandas

# 7. Ouverture du port utilise par Streamlit
EXPOSE 8501

# 8. Commande de lancement automatique du Dashboard
CMD ["streamlit", "run", "dashboard.py", "--server.port=8501", "--server.address=0.0.0.0"]