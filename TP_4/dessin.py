import sys 
import numpy as np 
import os
import matplotlib.pyplot as plt 


#-------------------------------------------------------------------------------------------
# ETAPE 1 : RECUPERER LE NOM DU FICHIER DE DONNEES PASSE EN ARGUMENT DE LA LIGNE DE COMMAND
if len(sys.argv) != 2 :
    print(" Nombre de paramètres incorrect ! \n il faut un paramètre en lign de commande : le nom de l'instance ")

    sys.exit()

f = sys.argv[1]
#---------------------------------------------
# ETAPE 2 : EXTRAIRE LES DONNEES DU FICHIER
if not os.path.exists(f):
    print("ERREUR Ouverture fichier " + f)
    sys.exit()

data = np.loadtxt(f)
plt.scatter(data[:,0], data[:,1], color='m')
plt.grid(visible=True,color='gray', linestyle='-', linewidth=0.2)
plt.savefig("graph.png")
plt.show()