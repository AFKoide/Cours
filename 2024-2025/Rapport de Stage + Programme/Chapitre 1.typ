#set heading(numbering: (..nums) => {
  if nums.pos().len() == 1 { return none }
  if nums.pos().len() >= 4 { return none }
  nums.pos().slice(1).map(str).join(".") + "."
  }
)

= _*Introduction*_

== Présentation de l'environnement du stage

==== Le LIRMM

Le *_LIRMM_* (Laboratoire d’Informatique, de Robotique et de Microélectronique de Montpellier) est une unité mixte de recherche de _l’Université de Montpellier_ et du _CNRS_. Il regroupe plus de 400 personnes et couvre trois grands domaines scientifiques : l'informatique, la robotique et la microélectronique.

Le laboratoire développe des recherches allant des concepts fondamentaux aux applications industrielles et médicales, en collaboration avec de nombreux partenaires académiques et socio-économiques, en France comme à l’international.

#linebreak()

==== L'équipe ICAR

Dans le cadre de mon stage, j'ai intégré l'équipe _*ICAR*_ (Image & Interaction) du LIRMM.

Cette équipe rassemble des chercheurs en robotique et en informatique autour des thématiques liées à l’image et, plus largement, aux données visuelles. Elle compte actuellement neuf chercheurs permanents (Université de Montpellier et CNRS), rejoints par des collaborateurs réguliers issus du CHU de Montpellier (imagerie médicale), du laboratoire TETIS (télédétection) et du CIRAD (modélisation agronomique). \
L’équipe est dirigée par M. *Nicolas Strauss*, professeur des universités, assisté de Mme *Noura Faraj*, maître de conférences à l’Université de Montpellier.

Les travaux sont structurés en quatre axes principaux :
- _Analyse & Traitement_
- _Sécurité Multimédia_
- _Modélisation & Visualisation_
- _Intelligence Artificielle pour les données visuelles_

Ces recherches couvrent des thématiques telles que l’imagerie médicale, la sécurité et l’authentification des images, le traitement statistique des données visuelles, ainsi que l’apprentissage profond appliqué à l’analyse et à la modélisation d’images.

#linebreak()

Mon encadrant durant ce stage fut Mr. *Nicolas Strauss*, et s'articule dans le cadre de l'axe *Modélisation & Visualisation*.

#linebreak()
#linebreak()

== Objectif scientifique du projet

Le projet s’inscrit dans le domaine de la décision multicritère. \
Contrairement aux modèles additifs classiques, ce stage a pour but d'utiliser une mesure non-additive (appelée _*capacité*_) afin de prendre en compte les interactions entre critères et de refléter différentes attitudes d’agrégation. \
L’objectif est de concevoir et tester un algorithme online capable d’apprendre cette capacité à partir d’exemples de paires de préférences fournis de manière séquentielle & aléatoire. \
La méthode repose sur le _Regularized Dual Averaging_ (RDA) et se distingue par sa capacité à gérer un grand nombre de critères ou d’exemples, tout en intégrant, si nécessaire, des contraintes normatives (monotonicité, supermodularité) grâce à l’ADMM (Alternating Direction Method of Multipliers).