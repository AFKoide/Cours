#set heading(numbering: (..nums) => {
  if nums.pos().len() == 1 { return none }
  if nums.pos().len() >= 4 { return none }
  nums.pos().slice(1).map(str).join(".")
  },
  
)
#show heading: set block(above: 2em)
#import "@preview/algorithmic:1.0.3"



= _Calculs mises en oeuvre_

Dans cette quatrième partie du rapport, nous présentons les différents modèles d’apprentissage testés. Chaque modèle est associé à une fonction de perte qui guide son apprentissage en mesurant l’écart entre les prédictions du modèle et les données d'apprentissage. \
Ainsi, l’évaluation repose sur un couplage systématique : à chaque méthode d'apprentissage, on paire une des deux fonction de perte.

#linebreak()

== *Modèle d’apprentissage testées*

=== #underline("Linéaire")
Le modèle linéaire constitue la référence la plus simple parmi les approches testées. \
Son principe est direct : il consiste à utiliser les critères tels quels, sans transformation ni enrichissement de l’espace des variables. Concrètement, le vecteur des critères d’entrée est simplement copié et transmis, ce qui revient à considérer chaque critère indépendamment, avec un poids propre.

#import "@preview/algorithmic:1.0.3"
#import algorithmic: style-algorithm, algorithm-figure
#show: style-algorithm

#algorithm-figure(
  "Modèle linéaire",
  vstroke: 1pt + luma(45%),
  inset: 5pt,
  {
    import algorithmic: *
    Procedure(
      "Linear",
      ("x"),
      {
        Comment[x : vecteur des critères d'entrée]
        Assign[$X$][$0(n)$]
        For($i <= n$, {
            Assign[$X_i$][$x_i$]
          })
        Return($X$)
      },
    )
  }
)



=== #underline("Game")
Le modèle Game construit une nouvelle représentation des critères en listant toutes les coalitions possibles.
Pour chaque sous-ensemble de critères $S$, on calcule :

$ Phi_S(x) = min_(i in S) (x_i) $

On obtient ainsi un vecteur $Phi(x)$ de taille $2^n - 1$.

On détermine les coalition en utilisant une matrice binaire $B$ de taille $(2^n - 1) times n$, où chaque ligne correspond à une coalition et chaque colonne à un critère. \
Par exemple, pour $n=4$ : \
$ B = mat(
  0,0,0,1;
  0,0,1,0;
  dots.v, dots.v, dots.v, dots.v;
  1,1,0,1;
  1,1,1,0) $

=== Game à 2-Additive

Une variante, appelée Game 2-Additive, limite la construction aux coalitions de taille 1 et 2 (critères seuls + paires). Cela réduit la taille du vecteur à $n + binom(n, 2)$, ce qui conserve la plupart des interactions tout en restant calculable. \
Ainsi, si l'on reprend l'exemple avec $n=4$, on obtient : \
$ B = mat(
  0,0,0,1;
  dots.v, dots.v, dots.v, dots.v;
  0,1,1,0;
  1,0,0,0;
  dots.v, dots.v, dots.v, dots.v) $


=== #underline("MacSum")





#import "@preview/algorithmic:1.0.3"
#import algorithmic: style-algorithm, algorithm-figure
#show: style-algorithm
#algorithm-figure(
  "MacSum (construction de X)",
  vstroke: 1pt + luma(45%),
  inset: 5pt,
  {
    import algorithmic: *
    Procedure(
      "MACSUM",
      ("ϕ", "x"),
      {
        Comment[X : sortie, taille n, initialisée à 0]
        Assign[$X$][$"zero"(n)$]

        Comment[1) Ordonner les indices selon $Phi$ (croissant)]
        Assign[$"idx"$][Ranger $Phi$ dans l'ordre croissant]

        LineBreak
        Comment[2) Passe avant : traiter $Phi$ <= 0]
        Assign[$x_min$][x[idx[1]]]
        Assign[$x_max$][x[idx[1]]]
        If($Phi["idx"[1]] <= 0$,
          { Assign[$X["idx"[1]]$][$X["idx"[1]] + x_min + x_max$] }
        )
        For($t = 2 -> n$,
          {
            If($Phi["idx"[t]] <= 0$,
              {
                Assign[$X["idx"[t]]$][$X["idx"[t]] - x_min - x_max$]
              }
            )
            Assign[$x_min$][min(x_min, x["idx"[t]])]
            Assign[$x_max$][max(x_max, x["idx"[t]])]
            If($Phi["idx"[t]] <= 0$,
              {
                Assign[$X["idx"[t]]$][$X["idx"[t]] + x_min + x_max$]
              }
            )
          }
        )
        LineBreak
        Comment[3) Passe arrière : traiter $Phi$ >= 0]
        Assign[$"idx"$][inverser l'ordre de idx]
        Assign[$x_min$][x[idx[1]]]
        Assign[$x_max$][x[idx[1]]]
        If($Phi["idx"[1]] >= 0$,
          { Assign[$X["idx"[1]]$][$X["idx"[1]] + x_max + x_min$] }
        )
        For($t = 2 .. n$,
          {
            If($Phi["idx"[t]] >= 0$,
              {
                Assign[$X["idx"[t]]$][$X["idx"[t]] - x_max - x_min$]
              }
            )
            Assign[$x_min$][min(x_min, x[idx[t]])]
            Assign[$x_max$][max(x_max, x[idx[t]])]
            If($Phi["idx"[t]] >= 0$,
              {
                Assign[$X["idx"[t]]$][$X["idx"[t]] + x_max + x_min$]
              }
            )
          }
        )

        Return($X$)
      },
    )
  }
)



=== #underline("MacSum Centré")

Un des principaux inconvénients des modèles précédents réside dans la *sensibilité aux paramètres d’apprentissage*. Pour chaque base de données, il est nécessaire d’ajuster finement les hyperparamètres  : le taux d’apprentissage, $Delta$ (tolérance de l’apprentissage), $delta$ (tolérance de la comparaison) ou encore $Gamma$ (utilisé uniquement pour la fonction L1). \
Avec la méthode du _MacSum Centré_, nous éliminons un de ces paramètres : $Delta$. 

L’idée est de ne plus produire une valeur unique comme score, mais *un intervalle* représentant la zone d’incertitude autour de la prédiction. \
Ainsi, au lieu de comparer directement deux valeurs, l’algorithme compare deux intervalles associés aux alternatives. La préférence ou l’indifférence est alors décidée selon trois situations possibles :
- *Inclusion* : l’un des intervalles est entièrement contenu dans l’autre.
- *Chevauchement* : les deux intervalles se superposent partiellement.
- *Dominance* : un intervalle est strictement plus grand que l’autre.

C’est la fonction de perte choisie qui déterminera quelles situations est une préférence ou une indifférence.

=== #underline("Capacitif (ADMM-RDA)")

Pour l’apprentissage *Capacitif*, nous avons implémenté une méthode combinant _Regularized Dual Averaging (RDA)_ et _{INSERER SIGNIFICATION ADMM} (ADMM)_. \
Le principe du _RDA_ est de mettre à jour les paramètres à partir de la moyenne des sous-gradients accumulés au cours du temps, ce qui garantit une meilleure stabilité dans un contexte d’apprentissage en ligne. L’ADMM complète ce schéma en projetant les solutions après chaque mise à jour afin de respecter les contraintes propres aux capacités (monotonie et normalisation). Nous appelons l'algorithme obtenus *_ADMM-RDA_*. 

Avant l’apprentissage, les données sont soumises à une normalisation spécifique afin d’améliorer la convergence. \
Chaque variable est transformée en trois nouvelles composantes correspondant à des niveaux bas, moyen et haut, obtenus par interpolation linéaire entre les quartiles de la distribution. Cette transformation garantit que toutes les variables d’entrée sont bornées dans $[0, 1]$. 

#linebreak()

== *Fonctions de perte*

=== L1 (perte absolue)

La fonction de perte L1 mesure l’erreur absolue entre la prédiction et la valeur réelle.
De manière analogue, on définit :

$ L_1 (x, x', phi) = cases(
  [delta - y + y']_+ "si" x > x',
  [abs(y - y') - delta]_+ "sinon",
) $

La mise à jour des paramètres se fait en deux temps. Dans un premier temps, on va calculer $Gamma$ :

$ Gamma = cases(
  "Si" x > x',
  (Phi(x') - Phi(x)) "si" Delta < delta,
  0 "sinon",   

  #linebreak()
  "Si" x approx x', 
  0 "si" abs(Delta) < delta,
  "sign"(Delta) . (Phi(x) - Phi(x')) "sinon",
) $
avec $Delta = y - y'$, $0$ un vecteur nul de la même taille que $phi$. 

Une fois $Gamma$ calculé, on pose :
$ overline(Gamma) = overline(Gamma) + frac(1, t) . (Gamma - overline(Gamma)) $
avec $t$ l'itération actuelle.

Finalement, on peut calculer la mise à jour des paramètres :
$ phi <- -"sign"(overline(Gamma)).frac(sqrt(t),gamma) . [abs(overline(Gamma)) - beta]_+ $

=== ADMM-RDA

La fonction de perte utilisée par la méthode ADMM-RDA repose sur la même base que pour la fonction L1. \
Si le calcul de l'erreur est identique, la mise à jour des paramètres diffère :

#import "@preview/algorithmic:1.0.3"
#import algorithmic: style-algorithm, algorithm-figure
#show: style-algorithm
#algorithm-figure(
  "ADMM-RDA",
  vstroke: 1pt + luma(45%),
  inset: 5pt,
  {
    import algorithmic: *
    Procedure(
      "ADMM-RDA",
      ("γ", "λ", "ρ", "T"),
      {
        Assign[$t$][$1$]
        Assign[$phi_1, mu_1, z_1$][$(0, ..., 0)$]
        LineBreak
        While(
          $t < T$,
          {
            Comment[receive pairwise example $(x_t, y_t)$]
            Comment[compute loss gradient $g_t in partial ell_t (m_t)$]
            Comment[update average gradient $overline(g_t)$]
            Assign[$g_t^(mu)$][$g_t - B^(T) (overline(mu)_t - rho (B overline(phi)_t - overline(z)_t))$]

            Assign[$phi_(t+1)$][$- sqrt(t)/gamma [abs(overline(g)_t) - lambda]_+ "sign"(overline(g)_t)$]
            
            Assign[$z_(t+1)$][$- [mu_t / rho - B m_(t+1)]_+$]

            Assign[$mu_(t+1)$][$mu_t - rho (B m_(t+1) - z_(t+1))$]
          },
        )
        Return(
          $phi_T$
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

La matrice $B$ est construite de la même façon que pour la génération d'une ligne de critère avec la manière _GAME_, mais l'on remplie les cellules avec des $-1$. 


=== L2 (perte quadratique)

La fonction de perte *L2* mesure l’erreur au carré entre la prédiction et la valeur réelle.
Formellement, pour deux objets $x$ et $x'$ et une agrégation $phi$, on note $y = angle.l phi, Phi(x) angle.r$ et $y' = angle.l phi, Phi(x') angle.r$.
La perte s’écrit :

$ L_2 (x, x', phi) = cases(
  (delta - (y - y'))^2 "si" x > x',
  ([|y - y'| - delta]_+)^2 "sinon",
) $

La mise à jour des paramètres s’effectue en utilisant ce calcul :

$ phi_(L_2) = cases(
  phi - beta . (Phi(x') - Phi(x)) . [delta - Delta]_+ "si" x > x',
  phi - beta . "sign"(Delta) . (Phi(x) - Phi(x')) . [|Delta| - delta]_+ "sinon",
) $
avec $Delta = y - y'$ et $beta$ le taux d’apprentissage.

#linebreak()

== Scores calculés

Afin d’évaluer la performance des modèles, plusieurs indicateurs standards de classification ont été utilisés :
- *_Précision (Precision)_* : proportion de prédictions correctes parmi toutes les prédictions positives du modèle.

$ "Precision" = frac("TP", "TP" + "FP") $

Si la Précision = 1, toutes les prédictions positives sont correctes.

- *_Rappel (Recall)_* : proportion de cas positifs correctement identifiés parmi l’ensemble des cas positifs.

$ "Recall" = frac("TP", "TP" + "FN") $

Si le Recall = 1, le modèle ne rate aucun cas positif.

- _*Exactitude (Accuracy)*_ : proportion de prédictions correctes (positives et négatives) sur l’ensemble des données. C’est une mesure globale de performance.

$ "Accuracy" = frac("TP", "TP" + "FP" + "TN" + "FN") $


Où _TP_ (True Positive) est les positifs correctement prédits, _FP_ (False Positive) les faux positifs (prédits positifs mais en réalité négatifs), _TN_ (True Negative) le nombre de négatifs correctement prédit et _FN_ (False Negative) les positifs que le modèle a ratés.