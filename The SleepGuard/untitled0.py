import pandas as pd
import matplotlib.pyplot as plt

# Charger le fichier CSV avec Pandas
nom_fichier_csv = 'heart_rate_data.csv'
donnees = pd.read_csv(nom_fichier_csv)

# Créer un graphique avec Matplotlib
plt.figure(figsize=(10, 6))
plt.plot(donnees.iloc[:, 0], donnees.iloc[:, 1], marker='o', linestyle='-', color='b')

# Ajouter des étiquettes et un titre
plt.xlabel('Colonne X')
plt.ylabel('Colonne Y')
plt.title('Graphique de la première colonne en X et de la deuxième colonne en Y')

# Afficher le graphique
plt.show()
