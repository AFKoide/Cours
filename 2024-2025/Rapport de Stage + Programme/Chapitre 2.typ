#set heading(numbering: (..nums) => {
  if nums.pos().len() == 1 { return none }
  nums.pos().slice(1).map(str).join(".") + "."
  }
)
#show heading: set block(above: 2em)


= Outils et notions fondamentales

== Mesure additive

=== Définition

Une mesure additive est une fonction qui associe à chaque sous-ensemble d'un ensemble de critère un poids. Cette mesure respecte la propriété suivante :
$ mu(A union B) = mu(A) + mu(B) "si" A subset.eq B $

=== Cas particuliers : #underline[Les Probabilités]

Les Probilités sont un cas particulier de mesure additive où la somme des poids de tous les sous-ensembles est égale à 1. On respecte donc la propriété suivante :
$ mu(Omega) = 1 $

#linebreak()

== Mesure non-additive

=== Définition & Motivation

Formellement, une mesure non-additive $mu$ sur une ensemble $Omega$ doit vérifier $mu(nothing) = 0$. \
Elle n'impose pas la propriété additive, c'est-à-dire que pour deux sous-ensembles $A$ et $B$ de $Omega$, on n'a pas nécessairement $mu(A union B) = mu(A) + mu(B)$.

#linebreak()

Contrairement aux mesures additives, les mesures non additives permettent de modéliser des situations où les critères interagissent entre eux (complémentarité ou redondance).

Par exemple, deux critères $A$ et $B$ peuvent, pris isolément, avoir des importances modérées ($mu(A)$ et $mu(B)$ faibles), mais ensemble former une synergie où $mu(A + B)$ est beaucoup plus grand que la somme de leurs importances individuelles. \
Inversement, si deux critères recouvrent un peu la même influence, la capacité peut attribuer à leur coalition une valeur inférieure à la somme, traduisant une redondance. Ainsi, la capacité « représente des poids d’importance pour chaque groupe de critères » plutôt que seulement pour chaque critère pris indépendamment.

=== Le Game

En mesure non-additive, on parle souvent de *Game* (jeu) pour désigner une fonction qui attribue une valeur à chaque sous-ensemble de critères. \
Un game est une fonction $mu : 2^N -> [0,1]$ qui, comme expliqué dans le point précédent, associe à chaque sous-ensemble de critères un poids, sans nécessairement respecter la propriété additive.

La seule condition pour un game est que l'ensemble vide ait une valeur nulle : $mu(nothing) = 0$.

=== La Monotonie

La monotonie est une propriété fondamentale des mesures non additives utilisées en décision multicritère. 

Intuitivement, une mesure est monotone si ajouter des critères (c’est-à-dire considérer un ensemble plus grand de critères satisfaits) ne peut pas réduire l’évaluation globale. Formellement, si $A$ et $B$ sont deux ensembles de critères tels que $A subset.eq B$, alors la mesure $mu$ vérifie $mu(A) <= mu(B)$. \
En d'autres termes, si une alternative remplit au moins toutes les mêmes exigences que $A$ et en plus d’autres critères (ensemble $B$), son "score" évalué par $mu$ ne sera pas inférieur. 

En termes simples, *plus de critères pris en compte (ou satisfaits) ne doit pas conduire à un moindre score*.

#linebreak()

Un exemple simple serait l'évaluation de plusieurs téléphones portables selon des critères tels que la qualité de l'appareil photo, la durée de vie de la batterie et le prix. \
Un premier téléphone qui satisfait le critère du prix obtient un certain score $mu("prix")$ ; si un autre téléphone satisfait à la fois le prix et la qualité de l'appareil photo, il devrait obtenir un score $mu("prix", "appareil")$ au moins aussi élevé. 

La monotonie empêche donc des situations contre-intuitives où l’ajout d’un avantage (un critère supplémentaire satisfait) ferait baisser l’évaluation globale. 

=== Cas particuliers : #underline[Les Capacités]

La capacité est un cas particulier dans le cadre des mesures non-additives. \
Il s'agit d'une fonction qui respectes les propriétés suivantes :
- Un ensemble vide $mu(nothing) = 0$
- L'ensemble entier de critère $mu(Omega) = 1$
- Monotonie respectée, tel que $mu(A) <= mu(B)$ dès que $A subset.eq B$
- Non-additivité, soit $mu(A union B) != mu(A) + mu(B)$.

Ainsi, une capacité est un game qui respecte la monotonie, la non-additivité et que l'ensemble de tous les critères aura toujours le poids maximal.

#linebreak()

== Intégrale de Choquet

=== Définition

L’intégrale de Choquet est un opérateur d’agrégation défini à partir d’une capacité. Elle permet de combiner les valeurs des critères tout en tenant compte des interactions possibles entre eux. Contrairement à une somme pondérée, l’intégrale de Choquet considère non seulement l’importance individuelle de chaque critère, mais aussi celle des coalitions de critères.

Formellement, pour un vecteur d’évaluations $x = (x_1, ..., x_n)$ trié par ordre croissant $(x_((1)) <= ... <= x_((n)))$, l’intégrale de Choquet par rapport à une capacité $mu$ est définie par :
$ C_mu(x) = sum_(i=1)^n (x_((i)) - x_((i-1))) * mu({(i), ..., (n)}) $
avec $x_((0)) = 0$.

#figure(
  image("./images/Choquet Integral.png", width: 80%),
  caption: [
    Comparaison entre une somme pondérée classique (à gauche) et une intégrale de Choquet pour la mesure additive (au centre) et pour la mesure non-additive (à droite). \ 
  ],
)

=== Cas Particulier

Si la capacité est additive (c’est-à-dire une mesure classique), l’intégrale de Choquet se réduit à une moyenne pondérée :
$ C_mu(x) = sum_(i=1)^n x_i * mu({i}) "     avec" sum_(i=1)^n mu({i}) = 1 $

=== La Transformation de Mobius

La transformée de Möbius offre une représentation alternative d’une capacité. Plutôt que d’assigner directement un poids à chaque coalition via $mu$, définit des coefficients de Möbius $m(A)$ tels que :
$ mu(A) = sum_(B subset.eq A) m(B) $

==== Lien entre l'intégrale de Choquet et la transformée de Mobius

En utilisant la transformée de Möbius, l’intégrale de Choquet peut s’écrire comme une somme linéaire :

$ C_mu(x) = sum_(A subset.eq N) m(A) * min_(i in A) x_i $

Cette écriture est importante car elle transforme le calcul, a priori complexe, en une combinaison linéaire de termes simples ($min_(i in A) (x_i)$). \
Cette représentation alternative facilite la mise en œuvre de traitements algorithmiques tels que l’optimisation, la régularisation ou encore l’apprentissage.