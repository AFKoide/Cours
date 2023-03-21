import numpy as np

print("TP Algebre Lineaire!\n\n")

# Scalaire
s = 5
print("s vaut : ",s) #Affichage de s


# Vecteur
v = np.array([1,2,3,4,5,6,7])
w = np.array([[1],[2],[3],[4],[5],[6],[7]])
print("\nVecteur\n",v)

zeros = np.zeros((2,2))
print("\nVecteur de zéros:\n",zeros)


# Produit
M = np.arange(1,13) #Crée vecteur à partir de 1 et s'arrete à 13
M.shape = (3,4)     #Il arrange le tableau en 3 lignes x 4 colonnes
print("\nProduit des matrices:\n",s*M)

print("Dimension de M: ",np.shape(M))

print("\nVecteur: première ",v[0]," et dernière",v[-1])
print("Valeur à 3x2: ",M[2,1]) #Colonne:4 ; Ligne:3 -> 4*3+2
# Il compte les lignes et colonnes à partir de 0 (0=1° ligne/colonne)


# Diagonale
m = np.diag([0,2,4])
n = np.eye(4)


# Manipulation Mathématique Matrice
A = np.arange(0,4); A.shape = (2,2)
B = np.array([[1,1],[1,1]])
print("\nA+B\n",A+B)
print("A*B\n",A*B)
print("AB\n",np.dot(A,B))
print("BA\n",np.dot(B,A))
print("Transposé:\nA\n",A.T)
print("B\n",B.T)


C = A+B
print("\nDéterminant de C: ",np.linalg.det(C))

F = np.array([[1,2,7],[-2,-4,-14]])
print("Rang de F: ",np.linalg.matrix_rank(C))

X = np.array([[3,1],[0,1]])
print("\nInverse de X:\n",np.linalg.inv(X) @ X)
print(X @ np.linalg.inv(X))

Y = np.array([[0,1],[1,1],[1,0]])
print("Pseudoinverse de Moore:\n",Y @ np.linalg.pinv(Y))
print(np.linalg.pinv(Y) @ Y)




# FONCTIONS (a executer depuis la console)
def compteur5():
    i=0
    while(i <= 5):
        print(i)
        i=i+1

def cube(be):
    cu = be*be*be
    return cu