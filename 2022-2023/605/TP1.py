import numpy as np

print("TP Algebre Lineaire!\n\n")

#scalaire
s = 5
print("s vaut : ",s) #Affichage de s

#vecteur
v = np.array([1,2,3,4,5,5,6,7])
w = np.array([[1],[2],[3],[4],[5],[6],[7]])
print("\nVecteur\n",v)

zeros = np.zeros((2,2))
print("\nVecteur de zéros:\n",zeros)

# Produit
M = np.arange(1,13)
M.shape = (3,4)
print("\nProduit des matrices:\n",s*M)

print("\nDimension de M: ",np.shape(M))

print(v[0:6])
print(M[2,1]) #Colonne:4 ; Ligne:3 -> 4*3+2
# Il compte les lignes et colonnes à partir de 0 (0=1° ligne/colonne)