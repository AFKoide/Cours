Compiler le code : 
```sh
gcc -fdiagnostics-color=always -g -Og *.c -o programme
```


Executer :
```sh
./programme.exe
```

Fais en sorte que les accents apparaissent dans le terminal :
```sh
chcp 65001
```


# Housing
- learning rate : choisir 1e-2, ca suffit

- Delta : 0.0001 est la limite. Très bon résultat pour 0.001 et 0.0005. A choisi 0.0008

Faire varier le learning rate a comme impact de faire augmenter ou diminuer le temps avant que l'algorithme ne se stabilise. Donc, si l'on choisi le learning rate trop petit, alors on risque d'avoir besoin de beaucoup d'itération, tandis que si le learning rate est trop grand, on risque, soit, de partir en oscillation (ou chaotiquement), ou de "mal" apprendre. On pourrait alors avoir une inversion entre les prédictions correctes et fausses, avoir toutes les prédictions qui serait indifférentes (ou inversement).

Faire varier le delta a comme impact de faire varier la distance entre les prédictions correctes et incorrectes, ainsi que le nombre d'itération avant que l'on soit stable.

- Itérations :
  - 5000 itérations globales.
  - 1000 itérations d'apprentissages.
  - 1000 itérations de test.


# StressLevelDataset
- learning rate : **1e-2**

- Delta : **0.005**, mais on a des bons résultats a 0.01 aussi

- Itérations 



## Learning
### L1

### L2

## GAME
Avant propos :  
De par la nature de la base de donnée, nous n'avons pas pu faire un game complet : pour 14 colonnes, nous obtenons 2¹³ soit 8192 colonnes. La taille étant si grande que le programme fini par planter.  
Les observations suivantes ont donc été réalisée avec 2-additives

### L1

### L2


## MacSum
### L1
Les prédictions sont très bruités.  
Aussi, les prédictions d'inversions (objet A serait inférieur à objet B), bien que majoritaire dans les première itérations, diminuent à un rytme régulier, jusqu'à représenter entre 1% et 5% des prédictions au final.

### L2
A la différence de L1, le signal est beaucoup moins bruité.  
Aussi, l'algorithme ne fait quasiment aucune préférence inversé. 



### Comparaison L1 et L2
Sur cet base de donnée, L2 est largement meilleure que L1.  
Dans un premier temps, l'apprentissage est beaucoup plus rapide avec L1 : si il a fallu plus de 3000 itérations de 1000 itérations d'apprentissages + test, L2 a eu besoin de moins de 50.  
Ensuite, les résultats : pour les memes delta et beta, L2 obtient une précision bien supérieure à L1. Avec 75% en moyenne de prédictions correctes, L2 domine L1, qui n'a que 60 à 65%, selon les runs.