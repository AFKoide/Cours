= Rapport De Travail Semaine 28 _(07-13 Juillet)_
#let colred(x) = text(fill: red, $#x$) 
\

== Résumé :

Au cours de la semaine précédente, j'ai travaillé sur l'ajout du MacSum Imprécis à mon programme.  
Cependant, après avoir programmé suivant les instructions & notes de Mr. Strauss, mes courbes d’entraînements avec données réelles et données linéaires ne sont pas bonnes.  \
J'ai donc, dans un premier temps, vérifier si les équations étaient correctes, puis j'ai voulu vérifier que la fonction utilisées pour calculer le MacSum Imprécis était correcte (sous les indications de Mr. Strauss).

\
\
\
\
== MacSum Imprécis

Après avoir ajouté avec succès la méthode _Linéaire_, _Game_ (complet et 2-additives), et _MacSum_, j'ai programmé le _MacSum Imprécis_.
\
Cette méthode, en plus d'avoir les manières L1 & L2 comme les autres, dispose de ce que l'on va appeler deux "modes" : #text(weight: "bold")[FAIBLE] et #text(weight: "bold")[FORT].
\
\
Dans un premier temps, il a fallu que j'incorpore à mon programme la manière L2 selon les équations fournies par Mr Strauss, puis il m'a été demandé de déterminer les équations de la manière L1 pour les deux modes.
\
\
=== L2
Voici les équations que j'ai utilisé : 
- Mode #text(weight: "bold")[FAIBLE] : 
  - Calcul de l'Erreur : \
    Si $x ~ x'$ : 
    $L_2(x, x', phi) = cases(
      0 "si" (underline(y) - underline(y)').(overline(y) - overline(y)') <= 0,
      max((underline(y) - underline(y)'), (overline(y) - overline(y)'))^2 "sinon"
    )$
    
    Si $x > x'$ : 
    $L_2(x, x', phi) = ([underline(y)' - underline(y)]^+)^2 + ([overline(y)' - overline(y)]^+)^2$

  - Mise à jour : \
    - Si $x ~ x'$ et que $(underline(y) - underline(y)').(overline(y) - overline(y)') <= 0$ : \
    
    $phi = phi - beta . cases(
      (underline(Phi)(x) - underline(Phi)(x')).(underline(y) - underline(y)') "si" abs(overline(y) - overline(y)') <= abs(underline(y) - underline(y)'),
      (overline(Phi)(x) - overline(Phi)(x')).(overline(y) - overline(y)')
    )$

    - Si $x > x'$ : $phi = phi - beta . ((underline(Phi)(x') - underline(Phi)(x)).[underline(y)' - underline(y)]^+ + (overline(Phi)(x') - overline(Phi)(x)).[overline(y)' - overline(y)]^+)$
\

- Mode #text(weight: "bold")[FORT] : 
  - Calcul de l'Erreur : \
    Si $x ~ x'$ : 
    $L_2(x, x', phi) = cases(
      0 "si" (underline(y) - overline(y)').(overline(y) - underline(y)') <= 0,
      max((underline(y) - overline(y)'), (overline(y) - underline(y)'))^2 "sinon"
    )$
    
    Si $x > x'$ : 
    $L_2(x, x', phi) = ([overline(y)' - underline(y)]^+)^2$

  - Mise à jour : \
    - Si $x ~ x'$ et que $(overline(y) - underline(y)').(underline(y) - overline(y)') <= 0$ : \
    
    $phi = phi - beta . cases(
      (overline(Phi)(x) - underline(Phi)(x')).(overline(y) - underline(y)') "si" abs(overline(y) - underline(y)') <= abs(underline(y) - overline(y)'),
      (underline(Phi)(x) - overline(Phi)(x')).(underline(y) - overline(y)')
    )$

    - Si $x > x'$ : $phi = phi - beta . ((overline(Phi)(x') - underline(Phi)(x)).[overline(y)' - underline(y)]^+)$

==== L2 FORT
#table(
  columns: (auto, auto),
  inset: 10pt,
  align: center,
  table.header(
    [*Données d’Entraînement*], [*Données de Test*],
  ),
  image("Images/L2 FORT training_logs.png"),
  image("Images/L2 FORT test_logs.png")
)
Sur le tableau ci-dessus, nous pouvons voir l'évolution de l'erreur moyenne et des différentes prédictions au cours des itérations. \
Si l'erreur moyenne tend vers 0, les prédictions sont cependant dans la majorité fausses : 
- A la fin de l’entraînement, plus aucune indifférence n'est prédites, le programme détermine que l'on a que des préférences.
- Si le nombre d'inversion (soit $y' > y$) est d'environ 0 au début de l’entraînement, la proportion augmente significativement par la suite pour atteindre autour de 50% pour certaine exécution.
- Enfin, on peut voir un manque de 'régularité' entre les différentes exécutions du programme.
\
\

==== L2 FAIBLE
#table(
  columns: (auto, auto),
  inset: 10pt,
  align: center,
  table.header(
    [*Données d’Entraînement*], [*Données de Test*],
  ),
  image("Images/L2 FAIBLE training_logs.png"),
  image("Images/L2 FAIBLE test_logs.png")
)

Avec le mode FAIBLE, il y a quelques différences :
- Si la courbe de l'erreur moyenne tend toujours vers 0, la courbe est beaucoup moins régulière.
- On a des prédictions aléatoires pour les indifférences et les préférences. En effet, toutes sont centrées sur 50%.
- A la différence du mode FORT, les prédictions inversées (pour rappel, $y' > y$), diminuent avec les itérations.


==== L2 FAIBLE Modifié

Après avoir montré ces résultats à Mr Strauss, il m'a proposé une modification : 
- Calcul de l'Erreur : \
  Si $x ~ x'$ : 
  $L_2(x, x', phi) = cases(
    0 "si" (underline(y) - underline(y)').(overline(y) - overline(y)') <= 0,
    colred(min)((underline(y) - underline(y)')^colred(2), (overline(y) - overline(y)')^colred(2)) "sinon"
  )$

- Mise à Jour :
  - Si $x ~ x'$ et que $(underline(y) - underline(y)').(overline(y) - overline(y)') <= 0$ : \
  
  $phi = phi - beta . cases(
    (underline(Phi)(x) - underline(Phi)(x')).(underline(y) - underline(y)') "si" abs(overline(y) - overline(y)') colred(>=) abs(underline(y) - underline(y)'),
    (overline(Phi)(x) - overline(Phi)(x')).(overline(y) - overline(y)')
  )$

Cependant, cette modification n'a donné de meilleurs résultats :
#table(
  columns: (auto, auto),
  inset: 10pt,
  align: center,
  table.header(
    [*Données d’Entraînement*], [*Données de Test*],
  ),
  image("Images/L2 FAIBLE MODIFIE training_logs.png"),
  image("Images/L2 FAIBLE MODIFIE test_logs.png")
)

\
\
\

=== L1
En parallèle au précédent points, j'ai également chercher les équations de L1 pour _MacSum Imprécis_. \
Voici les équations que j'ai utilisé : 
- Mode #text(weight: "bold")[FAIBLE] : 
  - Calcul de l'Erreur : \
    Si $x ~ x'$ : 
    $L_1(x, x', phi) = cases(
      0 "si" (underline(y) - underline(y)').(overline(y) - overline(y)') <= 0,
      max(abs(underline(y) - underline(y)'), abs(overline(y) - overline(y)')) "sinon"
    )$
    
    Si $x > x'$ : 
    $L_1(x, x', phi) = [underline(y)' - underline(y)]^+ + [overline(y)' - overline(y)]^+$

  - Mise à jour : \
    - Si $x ~ x'$ et que $(underline(y) - underline(y)').(overline(y) - overline(y)') <= 0$ : \
    
    $Gamma = cases(
      (underline(Phi)(x) - underline(Phi)(x')).s i g n(underline(y) - underline(y)') "si" abs(overline(y) - overline(y)') <= abs(underline(y) - underline(y)'),
      (overline(Phi)(x) - overline(Phi)(x')).s i g n(overline(y) - overline(y)')
    )$

    - Si $x > x'$ : $Gamma = (underline(Phi)(x') - underline(Phi)(x))chi_(underline(y)' - underline(y)) + (overline(Phi)(x') - overline(Phi)(x))chi_(overline(y)' - overline(y))$
    avec $chi_a = a "si" a > 0, 0$ sinon.
\

- Mode #text(weight: "bold")[FORT] : 
  - Calcul de l'Erreur : \
    Si $x ~ x'$ : 
    $L_1(x, x', phi) = cases(
      0 "si" (underline(y) - overline(y)').(overline(y) - underline(y)') <= 0,
      max(abs(underline(y) - overline(y)'), abs(overline(y) - underline(y)')) "sinon"
    )$
    
    Si $x > x'$ : 
    $L_1(x, x', phi) = [underline(y) - overline(y)']^+ + [overline(y) - underline(y)']^+$

  - Mise à jour : \
    - Si $x ~ x'$ et que $(overline(y) - underline(y)').(underline(y) - overline(y)') <= 0$ : \
    
    $Gamma = cases(
      (overline(Phi)(x) - underline(Phi)(x')).s i g n(overline(y) - underline(y)') "si" abs(overline(y) - underline(y)') <= abs(underline(y) - overline(y)'),
      (underline(Phi)(x) - overline(Phi)(x')).s i g n(underline(y) - overline(y)')
    )$

    - Si $x > x'$ : $Gamma = (overline(Phi)(x') - underline(Phi)(x))chi_(overline(y)' - underline(y))$

==== L1 FAIBLE
#table(
  columns: (auto, auto),
  inset: 10pt,
  align: center,
  table.header(
    [*Données d’Entraînement*], [*Données de Test*],
  ),
  image("Images/L1 FAIBLE training_logs.png"),
  image("Images/L1 FAIBLE test_logs.png")
)

==== L1 FORT
#table(
  columns: (auto, auto),
  inset: 10pt,
  align: center,
  table.header(
    [*Données d’Entraînement*], [*Données de Test*],
  ),
  image("Images/L1 FORT training_logs.png"),
  image("Images/L1 FORT test_logs.png")
)


== Conclusion

Après avoir étudié le programme en profondeur, il semblerait qu'il ait un problème avec $phi$. En effet, ce dernier tend vers 0, ce qui fait que les bornes de $Y$ et $Y'$ tendent également vers 0, à tel point que l'algorithme n'arrive plus à déterminer si $overline(y)$ est supérieur à $underline(y)$.
\
Pour déterminer si le problème ne viendrait du calcul du MacSum Imprécis, je vais le comparer à l'Intégrale de Choquet classique. 
#pagebreak()

= Comparaison MacSum Imprécis & _Intégrale de Choquet_

Le but de cette partie est de verifier si l'on obtient les mêmes résultats avec une Intégrale de Choquet et la fonction `CalculPhiX_MacSum_Intervalle`.

Mon test est le suivant : après avoir généré aléatoirement un vecteur de poids appelé $phi$, mon programme va générer aléatoirement un vecteur de critère $x$. Ces deux vecteurs seront alors transmis aux fonctions, qui calculeront $underline(y) "et" overline(y)$. Ces derniers seront alors comparé (en ajoutant un facteur d'erreur de l'ordre de $1e^(-10)$ environ).

Mr Strauss m'a clairement indiqué qu'il m'avait envoyé un programme contenant une fonction calculant l'Intégrale de Choquet, mais je ne l'ai pas trouvée... J'ai donc essayer de la programmer.
\
\

== Calcul de l'intégrale de Choquet

Ci-dessous, j'ai décrit étape par étape le calcul effectué dans la fonction `ChoquetIntegral`.

=== Étape 1 : Tri des entrées

On commence par trier les éléments du vecteur de critère $x$ dans l’ordre croissant. \
Chaque élément est mémorisé avec son indice initial, ce qui nous sera utile pour l'étape d'après.

=== Étape 2 : Construction des ensembles $A_i$ et calcul des capacités $phi(A_i)$

Pour chaque `i` de `0` à `n-1`, on construit l’ensemble :

$A_i = { pi(i), \pi(i+1), ..., pi(n-1) }$



où $pi$ est la permutation correspondant au tri croissant de $x$.

Ensuite, on calcule :

$
phi(A_i) = sum_(j=i)^(n-1) 
    φ_(pi(j))
$

Ce calcul se traduit dans le code par une accumulation des valeurs de $phi$ en fonction de l’indice d'origine de `x`.

=== Étape 3 : Calcul de l’Intégrale de Choquet

J'ai choisi l'Intégrale de Choquet Additive. Elle est évaluée à l’aide de cette  formule :

$
C_(φ(x)) = sum^(i: 1..n)
    (x_(i) - x_(i-1)) . phi(A_i)
$

où $x_(i)$ désigne le $i$-ème plus petit élément de `x`, et $x_(0) = 0$.

Dans le code, cette somme est accumulée dans la variable `choquet` :

```c
double delta = xi - prev;
double terme = delta * A_phi[i];
choquet += terme;
```


== Résultats

Je pense que mon programme pour calculer l'Intégrale de Choquet est faux. Ma fonction et `CalculPhiX_MacSum_Intervalle` ne retournent pas des valeurs semblables, et ceux à partir de la première itération.