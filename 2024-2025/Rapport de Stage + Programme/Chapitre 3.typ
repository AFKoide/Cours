#set heading(numbering: (..nums) => {
  if nums.pos().len() == 1 {  }
  nums.pos().slice(1).map(str).join(".")
  }
)
#show heading: set block(above: 2em)

= _Objectif et portée du projet_

== Objectif scientifique

Ce stage s'inscrit dans un projet de recherche initié par Mr. Strauss. \
Il a pour but de développer une méthode d'apprentissage capable de noter, sans biais, des objets selon plusieurs critères.

Dans le cadre de mon stage, j'ai travaillé sur le test et l'évaluation du modèle *MacSum*. \
L’un des objectifs du stage était également d’étudier les risques de surapprentissage associés à cette nouvelle méthode, les .

=== Comparer différentes méthodes d’apprentissage

L’objectif principal du projet est de *comparer différentes méthodes d’apprentissage* pour la prise de décision multicritère.
Plus précisément, l’algorithme étudié doit, à partir de plusieurs critères, être capable de comparer deux objets $A$ et $B$, et de déterminer soit une relation de *préférence* ($A > B$), soit une *indifférence* ($A approx B$).

Le stage a consisté à confronter plusieurs approches :
- *Méthode linéaire* : simple et rapide, mais limitée aux situations où la relation entre critères et préférence est linéaire.
- *Méthode Game* : elle permet de modéliser toutes les interactions entre critères. En revanche, son principal inconvénient est la complexité : le game complet nécessite $2^n$ paramètres pour $n$ critères, ce qui devient rapidement intractable ; même les versions restreintes (par exemple 2-additives) restent coûteuses. 
- *Méthode MacSum* _(développée dans l’équipe)_ : plus parcimonieuse, elle ne nécessite que $n$ paramètres pour $n$ critères. #underline[On cherche à savoir si cette méthode peut atteindre une précision comparable à la méthode *Game*].
- *Méthode MacSum Centré* _(développée dans l’équipe)_ : variante du MacSum qui conserve la même précision attendue, mais avec un paramètre en moins (le seuil n’est plus requis), ce qui simplifie l’apprentissage. Avec cette méthode, #underline[on cherche à savoir si l'on peut avoir la même précision que la méthode *MacSum*].
- *Méthode Capacitif* _(développée dans une autre équipe)_ : elle utilise une capacité pour modéliser les interactions entre critères. #underline[On va comparer cette méthode avec la méthode *MacSum* pour voir si elles ont des performances similaires].

=== Évaluer la précision et le comportement/évolution selon différents paramètres

L’évaluation scientifique porte ainsi à la fois sur la précision des méthodes et sur leur comportement selon différents paramètres (nombre de critères, taille de la base de préférences, contraintes normatives, etc.). \
On observera l'erreur moyenne au cours des itérations, ainsi que le Recall, la Precision et le F1-Score.

#linebreak()

== Application du projet

=== Intérêt pratique (classement, notation d’objets, robustesse, etc.)

D'un point de vue pratique, ce type d'algorithme peut être utilisé dans beaucoup de domaines.

Par exemple, il permettrait de déterminer si un traitement médical est préférable à un autre selon, par exemple, l'état de santé actuel du patient, son traitement en cours, son âge, etc. \
Il pourrait aussi être utilisé pour noter des produits, comme du vin, selon son prix, origine, composition chimique, etc.

=== Limitations identifiées (je ne suis pas sur de mettre cette partie)

Ce type d'algorithme à cependant des limites.

#linebreak()

La principale est le type de données en entrée. En effet, les critères doivent être que numérique. 

Un autre est vis-à-vis de l'entrainement : si les "scores" données en entrées sont trop proches, alors l'algorithme peut avoir du mal à apprendre, et se trompera plus souvent.

#pagebreak()