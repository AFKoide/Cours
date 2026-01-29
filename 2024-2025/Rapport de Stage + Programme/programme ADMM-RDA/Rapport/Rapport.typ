= Rapport du _14_ au _30 Juillet_
#let colred(x) = text(fill: red, $#x$) 
#set heading(numbering: (..nums) => {
  if nums.pos().len() == 1 { return none }
  nums.pos().slice(1).map(str).join(".") + "."
  }
)
#import "@preview/algorithmic:1.0.3"
\

== Résumé du rapport :

Au cours de ces deux dernières semaines, j'ai principalement travaillé sur une méthode pour automatiser l'execution d'apprentissage de l'algorithme avec des paramètres différents.  \
En parallèle, j'ai également tenté d'analyser les causes de l'irrégularité de _MacSum Imprécis_ en examinant les sources d'aléa dans le programme.  \
Enfin, j'ai implémenté les deux algorithmes décrits dans l'article _*Online Learning of Capacity-Based Preference Models*_, afin de pouvoir les comparer ultérieurement à _MacSum Imprécis_ et _MacSum_.

\
\
\
== Irrégularité de MacSum Imprécis

Pour chercher à savoir pourquoi _MacSum Imprécis_ était irrégulier, j'ai commencé par voir du côté des éléments "aléatoires" de l'algorithme, c'est à dire les paires de phrases et le paramètre $Phi$.

J'ai donc commencé par faire des tests avec un var_phi constant et des paires de phrases aléatoires.
J'ai ensuite fait des tests avec un var_phi aléatoire et des paires constantes.

Voici un tableau récapitulatif des résultats obtenues en faisant la moyenne de 5 execution de l'algorithme à l'ordre FORT et FAIBLE, pour un $phi$ aléatoires et des paires aléatoires, un $phi$ constant et des paires aléatoires, un $phi$ aléatoire et des paires constantes, et un $phi$ constant et des paires constantes. Les simulations ont été réalisée avec le mode L2, 20 simulations différentes, et la base de données _*Housing*_.
#let frame(stroke) = (x, y) => (
  left: if x > 1 { 0pt } else { stroke },
  right: stroke,
  top: if y < 2 { stroke } else { 0pt },
  
  bottom: stroke,
)
#set table(
  fill: (rgb("EAF2F5"), none),
  stroke: frame(rgb("21222C")),
)
#table(
  columns: 7,
  inset: 7pt,
  align: center + horizon,
  table.header(
    [Type], [Ordre], [Préférence Recall], [Préférence Precision], [Indifférence Recall], [Indifférence Precision], [Total Recall]
  ),
  "Hasard", "Faible",[$0.399±0.04$],[$0.454±0.039$],[$0.488±0.081$],[$0.587±0.115$],[$0.421±0.035$],
  "Hasard", "Fort",  [$0.76±0.159$],[$1.0±0.0$],[$0.0±0.0$],[$0.0±0.0$],[$0.537±0.118$],
  "Paires Fixes", "Faible",[$0.279±0.196$],[$0.359±0.261$],[$0.342±0.133$],[$0.859±0.199$],[$0.327±0.148$],
  "Paires Fixes", "Fort"  ,[$0.223±0.251$],[$0.455±0.478$],[$0.547±0.492$],[$0.626±0.464$],[$0.483±0.341$],
  [$phi$ Fixe], "Faible",[$0.478±0.137$],[$0.514±0.138$],[$0.345±0.079$],[$0.674±0.135$],[$0.445±0.079$],
  [$phi$ Fixe], "Fort"  ,[$0.728±0.114$],[$1.0±0.0$],[$0.0±0.0$],[$0.0±0.0$],[$0.487±0.062$],
)

Le tableau montre que, sauf pour le cas des _Paires Fixes_, les modèles d'ordre FORT ont tendance à prédire que les paires sont uniquement *indifférentes*. \
En revanche, pour l'ordre FAIBLE, on constate que le type ayant le recall le plus élevé est l'_Hasard_.
\
Malheureusement, je ne sais pas encore comment expliquer ces résultats. Si les résultats étaient meilleurs pour les paires fixes dans tous les cas de figure (que ce soit pour l'ordre FORT ou FAIBLE), on pourrait penser que le modèle a un problème avec cette méthode de tirage des données. Ce n'est cependant pas le cas. \

Dans un second temps, j'ai modifié dans l'algorithme les calculs utilisant $Phi$. Ainsi, au lieu de faire simplement la différence entre $Phi$ et $Phi'$, on fait la moyenne des deux.

#table(
  columns: 7,
  inset: 7pt,
  align: center + horizon,
  table.header(
    [Type], [Ordre], [Préférence Recall], [Préférence Precision], [Indifférence Recall], [Indifférence Precision], [Total Recall]
  ),
  "Moyenné", "Faible"  ,[$0.450±0.102$],[$0.5±0.109$],[$0.466±0.114$],[$0.62±0.081$],[$0.444±0.061$],
  "Moyenné", "Fort"    ,[$0.742±0.083$],[$0.990±0.023$],[$0.011±0.025$],[$0.015±0.034$],[$0.516±0.073$],
  "Classique", "Faible",[$0.399±0.04$],[$0.454±0.039$],[$0.488±0.081$],[$0.587±0.115$],[$0.421±0.035$],
  "Classique", "Fort"  ,[$0.76±0.159$],[$1.0±0.0$],[$0.0±0.0$],[$0.0±0.0$],[$0.537±0.118$],
)

A la lecture de ce tableau, on constate que faire la moyenne des $Phi$ améliore les résultats de l'algorithme, que ce soit pour l'ordre FORT ou FAIBLE. \
Cependant, on constate toujours que pour l'ordre FORT, l'algorithme ne prédit que les paires sont uniquement *indifférentes*. \

Cependant, j'ai été pris de doutes une fois le tableau précédent dressé... \
Vous m'avez demander de faire la "_moyenne des dérivée de q_phi_", j'ai donc fait la moyenne de la soustraction de $Phi(x)$ et de $Phi(x')$. \
Est-ce le bon calcul à faire ?


#pagebreak()
== Implémentation des Algorithmes

Une fois les irrégularités étudiées, j'ai commencé à convertir en .c les algorithmes "1" et "2" présentés dans l'article _*Online Learning of Capacity-Based Preference Models*_. \
Dans un premier temps, je vais faire un rappel rapide des deux algorithmes, puis je vais développer le calcul du gradient de l'erreur, avant finalement présenter les résultats obtenues. \

\
=== L'algorithme RDA

Ce premier algorithme est un algorithme de type RDA (Red Dear Algorithm). \

L'algorithme a la structure suivante : 
#import "@preview/algorithmic:1.0.3"
#import algorithmic: style-algorithm, algorithm-figure
#show: style-algorithm
#algorithm-figure(
  "RDA",
  vstroke: 1pt + luma(45%),
  inset: 0.3em,
  {
    import algorithmic: *
    Procedure(
      "RDA",
      ("γ", "λ", "T"),
      {
        Assign[$t$][$1$]
        Assign[$m_1$][$(0, ..., 0)$]
        LineBreak
        While(
          $t < T$,
          {
            Comment[Receive pairwise example ($x_t$, $y_t$)]
            Comment[compute loss gradient $g_t in partial ell_t (m_t)$]
            Comment[update average gradient $overline(g)_t$]
            
            Assign[$m_(t+1)$][$- sqrt(t)/gamma [abs(overline(g)_t) - lambda]_+ "sign"(overline(g)_t)$]
          },
        )
        Return(
          $m_T$,
        )
      },
    )
  }
)
Avec les paramètres suivants :
- λ est un hyperparameter de taille unique.
- γ est un hyperparameter de taille unique.
- T est le nombre d'iterations.


#pagebreak()
=== L'algorithme ADMM-RDA

#import "@preview/algorithmic:1.0.3"
#import algorithmic: style-algorithm, algorithm-figure
#show: style-algorithm
#algorithm-figure(
  "RDA with Bregman Projection",
  vstroke: 1pt + luma(45%),
  inset: 5pt,
  {
    import algorithmic: *
    Procedure(
      "ADMM-RDA",
      ("γ", "λ", "ρ", "T"),
      {
        Assign[$t$][$1$]
        Assign[$m_1, mu_1, z_1$][$(0, ..., 0)$]
        LineBreak
        While(
          $t < T$,
          {
            Comment[receive pairwise example $(x_t, y_t)$]
            Comment[compute loss gradient $g_t in partial ell_t (m_t)$]
            Comment[update average gradient $bar{g}_t$]
            Assign[$g_t^(mu)$][$g_t - B^(T) (overline(mu)_t - rho (B overline(m)_t - overline(z)_t))$]

            Assign[$m_(t+1)$][$- sqrt(t)/gamma [abs(overline(g)_t) - lambda]_+ "sign"(overline(g)_t)$]
            
            Assign[$z_(t+1)$][$- [mu_t / rho - B m_(t+1)]_+$]

            Assign[$mu_(t+1)$][$mu_t - rho (B m_(t+1) - z_(t+1))$]
          },
        )
        Return(
          $m_T$
        )
      },
    )
  }
)
Avec les paramètres suivants :
- λ est un hyperparameter qui contrôle le degrés de régulation.
- γ est un hyperparameter, mais je sais pas à quoi il sert...
- ρ est un hyperparameter qui contrôle le niveau de pénalisation.
- T est le nombre d'itération.

La matrice $B$ est construite de la même façon que pour la génération d'une ligne de critère avec la manière _GAME_ :
Pour $n=4$, \
$ B = mat(
  0,0,0,1;
  0,0,1,0;
  dots.v, dots.v, dots.v, dots.v;
  1,1,0,1;
  1,1,1,0) $
J'ai également modifié la fonction de création de la matrice $B$ afin d'y intégrer la possibilité de régler le _k-additivité_, de la même manière que pour la fonction GAME.

#pagebreak()
=== Fonction d'Erreur

Le calcul du loss gradient $g_t$ est présentée sous la forme $partial ell_t (m_t)$. \
Elle comprend une fonction $l_t$, cette dernière était :
$ l_t = cases(
  [delta - ⟨m, Phi(x_t) - Phi(y_t)⟩]_+ "si" t in P,
  [abs(⟨m, Phi(x_t) - Phi(y_t)⟩) - delta]_+ "si" t in I,
) $

On doit donc calculer le gradient de cette fonction. Voici la retranscription de la fonction en un algorithme (la fonction C se trouve dans le fichier `algorithm.c`) :
#import "@preview/algorithmic:1.0.3"
#import algorithmic: style-algorithm, algorithm-figure
#show: style-algorithm
#algorithm-figure(
  "Fonction Loss Gradient",
  vstroke: .5pt + luma(200),
  {
    import algorithmic: *

    Procedure(
      "Compute Gradient",
      ($x_t$, $y_t$, $m$, $delta$, $P$),
      {
        Comment[Initialize the used variables]
        Assign[$s$][0]
        Assign[coeff][$0$]
        LineBreak

        Assign[$s$][$angle.l m, (x_t - y_t)angle.r$]
        LineBreak

        IfElseChain(
          $P = "true" "and" s < delta$, 
          {
            Assign[coeff][$-1$]
            LineBreak
          },
          $P = "false" "and" abs(s) > delta$, {
            Assign[coeff][$"sign"(s)$]
            LineBreak
          },
          {
            Assign[coeff][$0$]
            LineBreak
          }
        )
        LineBreak

        Comment[$g_"out"$ will be different of $0$ only if there is a violation]
        Assign[$g_"out"$][$"coeff" * (x_t - y_t)$]

        LineBreak
        Return[$g_"out"$]
      },
    )
  }
)