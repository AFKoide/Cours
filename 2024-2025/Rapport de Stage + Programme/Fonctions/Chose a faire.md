A faire dans l'ordre : 

- Faire une moyenne des dérivée de q_phi.
  - On devrait avoir des meilleurs résultats.
- Comprendre pourquoi on a pas de régularité : 
  - Voir avec un var_phi constant et des paires aléatoires.
  - Voir avec un var_phi aléatoire et des paires constantes.
- Programmer l'algorithme "Online Learning of Capacity-Based Preference Models" et le comparer au game
- Si j'ai le temps, programmer un game imprécis (qui n'utilise pas de delta).
  - Pour ce faire, on prend l'algorithme `CalculPhiX_game` et on calculera y_inf et y_sup : 
    - y_inf se calcul en faisant le produit vectoriel entre var_phi et X_min
    - y_sup se calcul en faisant le produit vectoriel entre var_phi et X_max
      - X_min et X_max se calculent en faisant :
        - X_min[cpt] = MIN(x[i], X_min[q]);
        - X_max[cpt] = MAX(x[i], X_max[q]);
Ya un délire aussi avec var_phi : 
  - pour X_min, si var_phi est négatif, alors on prend le minimum entre 0 et un truc, et si on est positif, on prend l'inverse
  - Pour X_max, on fait l'inverse et on utilise max au lieu de min.
x
- Plotter a l'échelle logarithme le recall, la precision et l'accuracy de L1 et L2, Linéaire, Game (complet et 2-additives), MacSum et MacSum Imprécis, pour des learning rates différents, un delta différent, et une population différente.
  - On choisira comme delta : 1e-1, 1e-2, 1e-5, 1e-8, 1e-9, 1e-10, 1e-12, 1e-15
  - On choisir comme learning rate : 1e-1, 1e-2, 1e-5, 1e-8, 1e-9
```python
# Recall: TP / (TP + FN)
preference_recall   = nb_pref_vrai   / (nb_pref_vrai + nb_pref_fausse + nb_pref_inverse) if (nb_pref_vrai + nb_pref_fausse + nb_pref_inverse) > 0 else 0
indifference_recall = nb_indiff_vrai / (nb_indiff_vrai + nb_indiff_fausse) if (nb_indiff_vrai + nb_indiff_fausse) > 0 else 0

# Precision: TP / (TP + FP)
preference_precision    = nb_pref_vrai   / (nb_pref_vrai + nb_pref_fausse) if (nb_pref_vrai + nb_pref_fausse) > 0 else 0
indifference_precision  = nb_indiff_vrai / (nb_indiff_vrai + nb_pref_inverse) if (nb_indiff_vrai + nb_pref_inverse) > 0 else 0

# Accuracy: (TP + TN) / Total
total = nb_indifferences + nb_preferences
correct = nb_pref_vrai + nb_indiff_vrai
precision_total = correct / total if total > 0 else 0
```

