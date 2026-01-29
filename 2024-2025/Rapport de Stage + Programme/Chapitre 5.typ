= Expérimentations & Résultats

== Protocoles expérimentaux

Le protocole expérimental est le suivant :
- Utilisation d'une base de données de données réelles sur le prix de maisons à Boston (https://www.cs.toronto.edu/~delve/data/boston/bostonDetail.html)
- Séparation aléatoire de la base en deux groupes : 80% pour l'entraînement et 20% pour le test
  - Le groupe d'entrainement sera utilisé pour l'apprentissage du modèle
  - Le groupe de test sera utilisé pour évaluer la performance du modèle appris
- 20 simulations réalisées pour chaque modèle :
  - Chaque simulation fera 5000 itérations de : 
    1) Apprentissage (1000 itérations) ; 2) 
- Taux d'apprentissage fixé à $1e-5$
- $Gamma$ fixé à $1000$
- 7 deltas testés : $$


== Résultats obtenus et analyse comparative

=== Game "classique" et Game "2-Additive" vs ADMM-RDA

=== MacSum vs MacSum Centré

=== Comparaison pour différents deltas de Game, MacSum & MacSum Centré


=== Notes autres
- Le nombre d'itérations importe énormément pour la convergence : 
  - Si le modèle a peu d'itérations d'apprentissage, il aura plus de chance de diverger
  - Si le modèle a trop peu d'itérations totales mais suffisamment d'itérations d'apprentissage, il convergera mais ce sera faux.