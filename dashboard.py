import streamlit as st
import pandas as pd
import subprocess
import os

# Configuration de la page
st.set_page_config(page_title="Tableau de Bord Revenue Management", layout="wide")

def run_cpp_engine(input_file, output_file):
    """
    Execute le binaire C++ en lui passant les fichiers d'entree et de sortie.
    """
    try:
        executable = "./build/main_app"
        if os.name == 'nt':  
            executable = "./build/main_app.exe"
            
        subprocess.run([executable, input_file, output_file], check=True)
        return True
    except Exception as erreur:
        st.error(f"Erreur lors de l'execution du moteur C++ : {erreur}")
        return False

def main():
    st.title("Analyse des Tarifs - Aide a la Decision")
    
    if 'analyse_terminee' not in st.session_state:
        st.session_state.analyse_terminee = False

    st.sidebar.header("Configuration de l'Analyse")
    
    choix_source = st.sidebar.radio(
        "1. Selectionnez la source des donnees :",
        ("Fichier present dans le dossier", "Importer mon propre fichier")
    )
    
    chemin_entree = "data/sample.csv"
    chemin_rapport = "data/rapport_analyse.csv"
    pret_a_lancer = False

    if choix_source == "Fichier present dans le dossier":
        st.sidebar.info("Le fichier par defaut 'sample.csv' sera utilise.")
        pret_a_lancer = True
        
    elif choix_source == "Importer mon propre fichier":
        fichier_upload = st.sidebar.file_uploader("Chargez votre fichier CSV", type=["csv"])
        if fichier_upload is not None:
            chemin_entree = os.path.join("data", "uploaded_sample.csv")
            with open(chemin_entree, "wb") as f:
                f.write(fichier_upload.getbuffer())
            st.sidebar.success(f"Fichier {fichier_upload.name} pret.")
            pret_a_lancer = True
        else:
            st.sidebar.warning("En attente d'un fichier valide.")

    if pret_a_lancer:
            if st.sidebar.button("2. Lancer l'Analyse (Moteur C++)"):
                with st.spinner("Traitement des donnees par le moteur C++ en cours..."):
                    # 1. On lance le moteur C++
                    if run_cpp_engine(chemin_entree, chemin_rapport):
                        st.session_state.analyse_terminee = True 
                        st.sidebar.success("Analyse terminee !")
                        
                        # 2. NOUVEAU : Nettoyage du fichier temporaire
                        if choix_source == "Importer mon propre fichier" and os.path.exists(chemin_entree):
                            try:
                                os.remove(chemin_entree)
                            except OSError as e:
                                st.sidebar.warning(f"Note : Le fichier temporaire n'a pas pu etre supprime ({e})")
    if not st.session_state.analyse_terminee:
        st.info("Veuillez configurer votre source de donnees dans le menu de gauche puis lancer l'analyse pour visualiser les resultats.")
        
    elif st.session_state.analyse_terminee and os.path.exists(chemin_rapport):
        # 1. CHARGEMENT ET PREPARATION DES DONNEES
        donnees_vols = pd.read_csv(chemin_rapport)
        
        # Calcul de la volatilite (Ecart Tarifaire)
        donnees_vols['Ecart (Max - Min)'] = donnees_vols['Net Max'] - donnees_vols['Net Min']
        
        st.markdown("---")
        col1, col2, col3 = st.columns(3)
        
        tarif_min_global = donnees_vols['Net Min'].min()
        tarif_max_global = donnees_vols['Net Max'].max()
        moyenne_globale = donnees_vols['Moyenne Net'].mean()
        
        col1.metric("Tarif Net Minimum Global", f"{tarif_min_global} EUR")
        col2.metric("Tarif Net Maximum Global", f"{tarif_max_global} EUR")
        col3.metric("Moyenne des Tarifs", f"{moyenne_globale:.2f} EUR")
        
        # 2. SEPARATION EN DEUX ONGLETS POUR UNE UX PROPRE
        onglet_graphiques, onglet_donnees = st.tabs(["Visualisation Analytique", "Exploration des Donnees"])
        
        with onglet_graphiques:
            col_g1, col_g2 = st.columns(2)
            
            with col_g1:
                st.subheader("Comparaison : Min vs Max par Vol")
                st.bar_chart(donnees_vols.set_index('Numero de vol')[['Net Min', 'Net Max']])
                
            with col_g2:
                # NOUVEAU GRAPHIQUE : Volatilite
                st.subheader("Volatilite : Ecart Tarifaire par Vol")
                st.bar_chart(donnees_vols.set_index('Numero de vol')['Ecart (Max - Min)'])

        with onglet_donnees:
            # 3. FILTRES CROISES AVANCES
            st.subheader("Filtres Avances (O&D)")
            col_f1, col_f2, col_f3 = st.columns(3)
            
            with col_f1:
                filtre_origine = st.multiselect("Origine(s)", options=donnees_vols['Origine'].unique())
            with col_f2:
                filtre_destination = st.multiselect("Destination(s)", options=donnees_vols['Destination'].unique())
            with col_f3:
                prix_max_filtre = st.slider("Prix Moyen Maximum (EUR)", 
                                          min_value=float(donnees_vols['Moyenne Net'].min()), 
                                          max_value=float(donnees_vols['Moyenne Net'].max()), 
                                          value=float(donnees_vols['Moyenne Net'].max()))

            # Application des filtres
            donnees_filtrees = donnees_vols.copy()
            if filtre_origine:
                donnees_filtrees = donnees_filtrees[donnees_filtrees['Origine'].isin(filtre_origine)]
            if filtre_destination:
                donnees_filtrees = donnees_filtrees[donnees_filtrees['Destination'].isin(filtre_destination)]
                
            donnees_filtrees = donnees_filtrees[donnees_filtrees['Moyenne Net'] <= prix_max_filtre]
            
            # Affichage du tableau filtré
            st.dataframe(donnees_filtrees, use_container_width=True)
            
            # 4. EXPORTATION DES DONNEES
            csv_export = donnees_filtrees.to_csv(index=False).encode('utf-8')
            st.download_button(
                label="Telecharger cette selection (CSV)",
                data=csv_export,
                file_name='rapport_yield_management.csv',
                mime='text/csv',
            )

if __name__ == "__main__":
    main()