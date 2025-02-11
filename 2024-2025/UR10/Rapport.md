# Rapport sur le Contrôle et la Simulation du Robot UR10

**Auteur**: Curtis Martelet  
**Date**: 29/11/2024  

---

- [Rapport sur le Contrôle et la Simulation du Robot UR10](#rapport-sur-le-contrôle-et-la-simulation-du-robot-ur10)
- [I. Introduction](#i-introduction)
- [II. Modélisation du Robot UR10](#ii-modélisation-du-robot-ur10)
  - [1. Présentation de la méthode DH](#1-présentation-de-la-méthode-dh)
  - [2. Schéma du robot](#2-schéma-du-robot)
  - [3. Paramètres DH](#3-paramètres-dh)
    - [Simulation](#simulation)
    - [Robot Réel](#robot-réel)
- [III. Formules et modélisation mathématique](#iii-formules-et-modélisation-mathématique)
  - [1. Générateur de Trajectoire](#1-générateur-de-trajectoire)
    - [Calcul des positions et vitesses désirées](#calcul-des-positions-et-vitesses-désirées)
    - [Calcul de l'orientation désirée](#calcul-de-lorientation-désirée)
      - [Calcul de la matrice de rotation relative](#calcul-de-la-matrice-de-rotation-relative)
      - [Calcul de l'angle de rotation ( \\theta )](#calcul-de-langle-de-rotation--theta-)
      - [Calcul de l'axe de rotation ( \\mathbf{u} )](#calcul-de-laxe-de-rotation--mathbfu-)
      - [Interpolation de la rotation désirée](#interpolation-de-la-rotation-désirée)
  - [2. Modèle Géométrique](#2-modèle-géométrique)
    - [Les Matrices de Transformation](#les-matrices-de-transformation)
    - [Calcul de la Jacobienne](#calcul-de-la-jacobienne)
      - [Calcul de la Jacobienne ( n )](#calcul-de-la-jacobienne--n-)
      - [Calcul de la matrice ( D )](#calcul-de-la-matrice--d-)
      - [Calcul de la Jacobienne Finale](#calcul-de-la-jacobienne-finale)
  - [3. L'Erreur](#3-lerreur)
    - [a. **Erreur de Position**](#a-erreur-de-position)
    - [b. **Erreur d'Orientation**](#b-erreur-dorientation)
    - [c. **Matrice ( L )**](#c-matrice--l-)
        - [Définition de la Matrice Antisymétrique ( S(v) )](#définition-de-la-matrice-antisymétrique--sv-)
    - [d. Utilisation de ( e\_p ) et ( e\_o )](#d-utilisation-de--e_p--et--e_o-)
  - [4. Le Contrôleur](#4-le-contrôleur)
    - [Calcul de la vitesse angulaire désirée](#calcul-de-la-vitesse-angulaire-désirée)
    - [Utilisation de la Jacobienne pseudo-inverse](#utilisation-de-la-jacobienne-pseudo-inverse)
    - [Composition de la commande](#composition-de-la-commande)
  - [5. Commande des Moteurs](#5-commande-des-moteurs)
    - [Mise à jour des positions articulaires](#mise-à-jour-des-positions-articulaires)
- [Implémentation](#implémentation)
  - [Langages et bibliothèques utilisés](#langages-et-bibliothèques-utilisés)
  - [Étapes de la simulation](#étapes-de-la-simulation)
    - [1. Connexion à CoppeliaSim](#1-connexion-à-coppeliasim)
    - [2. Génération de la trajectoire](#2-génération-de-la-trajectoire)
    - [3. Calcul des commandes des articulations](#3-calcul-des-commandes-des-articulations)
    - [4. Envoi des commandes à CoppeliaSim](#4-envoi-des-commandes-à-coppeliasim)
- [Courbes et résultats](#courbes-et-résultats)
  - [Position désirées et actuelles des joints au cours du temps](#position-désirées-et-actuelles-des-joints-au-cours-du-temps)
  - [Erreur normalisée](#erreur-normalisée)
  - [Vitesse désirées et actuelles des joints au cours du temps](#vitesse-désirées-et-actuelles-des-joints-au-cours-du-temps)
  - [Angles désirées et actuelles des joints au cours du temps](#angles-désirées-et-actuelles-des-joints-au-cours-du-temps)
- [Conclusion](#conclusion)


---

# I. Introduction

Ce rapport sert de conclusion au travail réalisé sur la simulation et le contrôle du robot UR10.  
Nous y développons le calcul et l'implémentation d'un système de commande déterminé avec la méthode de Denavit-Hartenberg modifiée.  
Nous décrivons également les étapes de simulation sous *CoppeliaSim* ainsi que les résultats obtenus.  

---

# II. Modélisation du Robot UR10

## 1. Présentation de la méthode DH

La méthode Denavit-Hartenberg (DH) permet de décrire la géométrie d'un robot manipulateur.  
Chaque articulation est modélisée par une matrice de transformation homogène qui exprime la position et l'orientation d'un repère local par rapport au repère précédent.  

## 2. Schéma du robot

![Schéma du robot avec repères](repere.png)

## 3. Paramètres DH

### Simulation

Dans le cas de la simulation dans *CoppeliaSim*, les paramètres DH sont modifiés pour aligner les axes des repères du simulateur.  

| Joint | σᵢ | αᵢ₋₁ (rad) | aᵢ₋₁ (m) | θᵢ (rad)      | dᵢ (m)  |
|-------|----|------------|----------|--------------|---------|
| 1     | 0  | 0          | 0        | θ₁           | r₁      |
| 2     | 0  | π/2        | 0        | θ₂ - π/2     | r₂      |
| 3     | 0  | 0          | -a₂      | θ₃           | 0       |
| 4     | 0  | 0          | -a₃      | θ₄           | 0       |
| 5     | 0  | π/2        | 0        | θ₅ - π/2     | r₅      |
| 6     | 0  | -π/2       | 0        | θ₆           | 0       |

### Robot Réel

Pour le robot réel, les repères respectent directement la configuration physique du robot, ce qui entraîne une différence au niveau des angles des joints :  

| Joint | σᵢ | αᵢ₋₁ (rad) | aᵢ₋₁ (m) | θᵢ (rad)  | dᵢ (m)  |
|-------|----|------------|----------|----------|---------|
| 1     | 0  | 0          | 0        | θ₁       | r₁      |
| 2     | 0  | π/2        | 0        | θ₂       | r₂      |
| 3     | 0  | 0          | -a₂      | θ₃       | 0       |
| 4     | 0  | 0          | -a₃      | θ₄       | 0       |
| 5     | 0  | π/2        | 0        | θ₅       | r₅      |
| 6     | 0  | -π/2       | 0        | θ₆       | 0       |

Nous avons retiré les décalages de π/2 sur les articulations 2 et 5.

---

# III. Formules et modélisation mathématique

Dans cette seconde partie du rapport, nous allons détailler les étapes et calculs utilisés dans la commande du robot.  
La commande du robot est basée sur le modèle géométrique. On y trouve :
- Un bloc **Générateur de Trajectoire**, qui fournit la vitesse et la position désirée au cours du temps.
- Un bloc **Modèle Géométrique**, qui calcule la pose de l'effecteur final en fonction des angles des articulations.
- Un bloc **Erreur**, qui calcule la différence entre la position et l'orientation désirée et celles réelles.
- Un bloc **Contrôleur**, qui détermine la vitesse angulaire désirée de chaque articulation.

Selon que l'on commande le robot ou sa simulation, il est nécessaire d'intégrer la vitesse angulaire désirée pour obtenir les angles de chaque articulation : la simulation se commande en position tandis que le robot réel se commande en vitesse.

<br>

## 1. Générateur de Trajectoire

Le générateur de trajectoire permet de calculer la position et l'orientation de l'organe terminal au cours du temps en fonction.  
Nous avons utilisé un polynôme de degré cinq pour garantir des trajectoires continues en position, vitesse et accélération.

### Calcul des positions et vitesses désirées

Les positions et vitesses désirées sont calculées à partir des paramètres de la trajectoire (position initiale et position finale) et de la durée de simulation.  
On utilise un polynôme de cinquième degré pour calculer \( r \) et sa dérivée \( r_{\text{point}} \), qui servent à déterminer la position et la vitesse.

\[
r(t) = 10 \left( \frac{t}{t_f} \right)^3 - 15 \left( \frac{t}{t_f} \right)^4 + 6 \left( \frac{t}{t_f} \right)^5
\]

\[
r_{\text{point}}(t) = 30 \frac{t^2}{t_f^3} - 60 \frac{t^3}{t_f^4} + 30 \frac{t^4}{t_f^5}
\]

Une fois \( r \) et \( r_{\text{point}} \) déterminées, la position et la vitesse désirées sont calculées ainsi :

\[
x_{\text{désirée}} = x_{\text{init}} + r(t) (x_{\text{final}} - x_{\text{init}})
\]

\[
x_{\text{point désirée}} = r_{\text{point}}(t) (x_{\text{final}} - x_{\text{init}})
\]

### Calcul de l'orientation désirée

La génération de l'orientation désirée pour l'effecteur se base sur l'interpolation entre la rotation actuelle et la rotation finale. Le calcul suit les étapes suivantes.

#### Calcul de la matrice de rotation relative

La matrice de rotation relative \( R \) est calculée à l'aide de cette formule :

\[
R = R_{\text{init}}^T \cdot R_{\text{final}}
\]

Elle représente la transformation nécessaire pour passer de l'orientation initiale à l'orientation finale.

#### Calcul de l'angle de rotation \( \theta \)

L'angle de rotation \( \theta \) est extrait de la matrice de rotation relative \( R \).  
Il est déterminé à partir du cosinus et du sinus, puis de leur rapport :

\[
\cos(\theta) = \frac{\text{Tr}(R) - 1}{2}
\]

\[
\sin(\theta) = \frac{\sqrt{(R_{32} - R_{23})^2 + (R_{13} - R_{31})^2 + (R_{21} - R_{12})^2}}{2}
\]

\[
\theta = \tan^{-1}\left(\frac{\sin(\theta)}{\cos(\theta)}\right)
\]

- \( \text{Tr}(R) \) est la trace de la matrice \( R \) (la somme de ses éléments diagonaux).  
- Les éléments \( R_{ij} \) correspondent aux composantes de la matrice \( R \).

#### Calcul de l'axe de rotation \( \mathbf{u} \)

Une fois \( \theta \) calculé, l'axe de rotation \( \mathbf{u} \) peut être déterminé :

\[
\mathbf{u} = \frac{1}{2\sin(\theta)} 
\begin{bmatrix}
R_{32} - R_{23} \\
R_{13} - R_{31} \\
R_{21} - R_{12}
\end{bmatrix}
\]

#### Interpolation de la rotation désirée

La rotation désirée \( \mathbf{R}_{\text{désirée}} \) est obtenue en appliquant une interpolation basée sur le facteur \( r \), issu de la trajectoire temporelle :

\[
\mathbf{R}_{\text{désirée}} = \mathbf{R}_{\text{init}} \cdot \mathbf{rot}(\mathbf{u}, r \cdot \theta)
\]

La matrice \( \mathbf{rot}(\mathbf{u}, r \cdot \theta) \) est une matrice de rotation autour de l'axe \( \mathbf{u} \) d'un angle \( r \cdot \theta \), donnée par :

\[
\mathbf{rot}(\mathbf{u}, r \cdot \theta) =
\begin{bmatrix}
u_x^2 (1 - \cos(r \cdot \theta)) + \cos(r \cdot \theta) & u_x u_y (1 - \cos(r \cdot \theta)) - u_z \sin(r \cdot \theta) & u_x u_z (1 - \cos(r \cdot \theta)) + u_y \sin(r \cdot \theta) \\
u_x u_y (1 - \cos(r \cdot \theta)) + u_z \sin(r \cdot \theta) & u_y^2 (1 - \cos(r \cdot \theta)) + \cos(r \cdot \theta) & u_y u_z (1 - \cos(r \cdot \theta)) - u_x \sin(r \cdot \theta) \\
u_x u_z (1 - \cos(r \cdot \theta)) - u_y \sin(r \cdot \theta) & u_y u_z (1 - \cos(r \cdot \theta)) + u_x \sin(r \cdot \theta) & u_z^2 (1 - \cos(r \cdot \theta)) + \cos(r \cdot \theta)
\end{bmatrix}
\]

Dans cette formule :
- \( u_x, u_y, u_z \) sont les composantes de l'axe de rotation \( \mathbf{u} \).  
- \( r \cdot \theta \) est l'angle interpolé correspondant au temps actuel.  

La rotation désirée \( \mathbf{R}_{\text{désirée}} \) guide l'effecteur vers sa rotation finale tout en assurant une transition douce.

## 2. Modèle Géométrique

Le modèle géométrique permet de déterminer les positions cartésiennes à partir des angles de chaque articulation.  
Les coordonnées cartésiennes réelles du robot sont utilisées pour calculer l'erreur de position et d'orientation entre la pose désirée et celle actuelle.

Pour réaliser le modèle géométrique du robot, nous avons besoin des matrices de transformation.

### Les Matrices de Transformation

La formule générale d'une matrice de transformation homogène selon DH modifié est la suivante :  

\[
T_j = 
\begin{bmatrix}
\cos\theta_j & -\sin\theta_j \cos\alpha_{j-1} & \sin\theta_j \sin\alpha_{j-1} & a_{j-1} \\
\sin\theta_j & \cos\theta_j \cos\alpha_{j-1} & -\cos\theta_j \sin\alpha_{j-1} & -r_j \sin\alpha_{j-1} \\
0 & \sin\alpha_{j-1} & \cos\alpha_{j-1} & r_j \cos\alpha_{j-1} \\
0 & 0 & 0 & 1
\end{bmatrix}
\]

Une fois les matrices de transformation de chaque articulation déterminées, nous devons calculer la matrice de transformation entre l'origine et la dernière articulation du robot :

\[
T_{06} = T_1 \cdot T_2 \cdot T_3 \cdot T_4 \cdot T_5 \cdot T_6
\]

La position de l'effecteur final est déterminée en multipliant cette matrice par le vecteur homogène représentant l'effecteur :  

\[
\mathbf{O_{07}} = T_{06} \cdot \mathbf{O_{67}}
\]

où :
- \( T_{06} \) est la matrice de transformation de la base au repère final.  
- \( \mathbf{O_{67}} \) est le vecteur décrivant la position de l'effecteur par rapport à l'articulation 6.  

Avec cette matrice \( \mathbf{O_{07}} \), nous obtenons la position de l'effecteur. Cette donnée est utilisée pour calculer l'erreur de position et d'orientation entre la position désirée et la position actuelle de l'effecteur.

### Calcul de la Jacobienne

#### Calcul de la Jacobienne \( n \)

La Jacobienne est une matrice de taille \( 6 \times n \), où \( n \) est le nombre d'articulations. Chaque colonne de la Jacobienne correspond à une articulation du robot.  
La méthode pour calculer ces colonnes dépend de la nature des articulations (rotatives ou prismatiques).

| Articulation Rotoïde                                | Articulation Prismatique                     |
|-----------------------------------------------------|----------------------------------------------|
| \( J_p = Z_i \times (P_n - P_i) \)                  | \( J_p = Z_i \)                              |
| \( J_o = Z_i \)                                     | \( J_o = 0_{3 \times 3} \)                   |

où :
- \( Z_i \) : Vecteur de l'axe de rotation \( z \) pour l'articulation \( i \). C'est la dernière colonne de la matrice de rotation de l'articulation.
- \( P_n \) : Vecteur position de la dernière articulation.
- \( P_i \) : Vecteur position de l'articulation \( i \).

Dans le cas du robot UR10, toutes les articulations sont rotatives.

#### Calcul de la matrice \( D \)

Pour le robot UR10, il existe un décalage \( r_7 \) entre la dernière articulation et le centre de l'effecteur.  
Ce décalage est pris en compte avec une matrice antisymétrique \( D \), calculée à partir des éléments de la matrice de rotation \( R_{06} \) de l'effecteur final.

\[
\mathbf{D} = 
\begin{bmatrix}
0 & a_n x_z + r_{n+1} z_z & -a_n x_y - r_{n+1} z_y \\
-a_n x_z - r_{n+1} z_z & 0 & a_n x_x + r_{n+1} z_x \\
a_n x_y + r_{n+1} z_y & -a_n x_x - r_{n+1} z_x & 0
\end{bmatrix}
\]

où :
- \( a_n \) et \( r_{n+1} \) sont trouvés dans \( \mathbf{O_{67}} \).
- \( x_x, x_y, x_z, z_x, z_y, z_z \) proviennent de la matrice de rotation \( R_{06} \), définie par :

\[
^0 R_n = 
\begin{bmatrix}
x_x & y_x & z_x \\
x_y & y_y & z_y \\
x_z & y_z & z_z
\end{bmatrix}
\]

#### Calcul de la Jacobienne Finale

Une fois \( J_p \), \( J_o \), et \( D \) calculés, la Jacobienne finale \( J \) est déterminée par :

\[
J = 
\begin{bmatrix}
I_3 & D \\
0_{3 \times 3} & C
\end{bmatrix}
\cdot J_{06}
\]

où :
- \( I_3 \) est une matrice identité \( 3 \times 3 \).
- \( 0_{3 \times 3} \) est une matrice de zéros \( 3 \times 3 \).
- \( C \) est une matrice identité choisie dans notre calcul.
- \( D \) est la matrice antisymétrique calculée précédemment.


## 3. L'Erreur

Le calcul de l'erreur permet de mesurer la différence entre la position et l'orientation actuelle du robot et celles désirées à un moment \( t \).  
Cette étape est essentielle pour guider l'effecteur vers sa trajectoire et son orientation souhaitées.  

### a. **Erreur de Position**

L'erreur de position, notée \( e_p \), représente la différence entre la position actuelle de l'effecteur (\( x_{\text{actuel}} \)) et la position désirée (\( x_{\text{désiré}} \)) :  

\[
e_p = x_{\text{désiré}} - x_{\text{actuel}}
\]

Cette erreur est exprimée en coordonnées cartésiennes et indique à quel point l'effecteur est éloigné de la position cible.

### b. **Erreur d'Orientation**

L'erreur d'orientation, notée \( e_o \), exprime la différence entre l'orientation actuelle de l'effecteur (\( R_{\text{actuel}} \)) et l'orientation désirée (\( R_{\text{désiré}} \)).  
Elle est calculée comme une combinaison des produits vectoriels entre les colonnes des matrices de rotation actuelles et désirées :  

\[
e_o = \frac{1}{2} \left( R_{\text{actuel}}[:, 0] \times R_{\text{désiré}}[:, 0] + 
                         R_{\text{actuel}}[:, 1] \times R_{\text{désiré}}[:, 1] + 
                         R_{\text{actuel}}[:, 2] \times R_{\text{désiré}}[:, 2] \right)
\]

Chaque terme représente une contribution liée à l'écart entre les axes correspondants des orientations actuelle et désirée.

---

### c. **Matrice \( L \)**

La matrice auxiliaire \( L \) est utilisée pour faciliter le calcul de l'erreur d'orientation.  
Elle relie les rotations souhaitées et actuelles en modifiant leur structure :  

\[
L = -\frac{1}{2} \left( S(R_{\text{désiré}}[:, 0]) S(R_{\text{actuel}}[:, 0]) + 
                         S(R_{\text{désiré}}[:, 1]) S(R_{\text{actuel}}[:, 1]) + 
                         S(R_{\text{désiré}}[:, 2]) S(R_{\text{actuel}}[:, 2]) \right)
\]

##### Définition de la Matrice Antisymétrique \( S(v) \)

La matrice \( S(v) \) est une matrice antisymétrique associée à un vecteur \( v \).  
Elle est définie pour un vecteur donné \( v = \begin{bmatrix} v_x \\ v_y \\ v_z \end{bmatrix} \) comme suit :  

\[
S(v) = 
\begin{bmatrix}
0 & -v_z & v_y \\ 
v_z & 0 & -v_x \\ 
-v_y & v_x & 0
\end{bmatrix}
\]

Cette matrice est utilisée pour représenter des relations croisées entre les vecteurs d'orientation.



### d. Utilisation de \( e_p \) et \( e_o \)

Les erreurs de position (\( e_p \)) et d'orientation (\( e_o \)) sont utilisées dans le contrôleur pour ajuster les commandes des moteurs.  
En corrigeant ces erreurs, le robot peut suivre la trajectoire désirée tout en respectant les orientations prévues.


## 4. Le Contrôleur

Le contrôleur de notre chaîne de commande calcule les vitesses articulaires (\(\dot{q}\)) nécessaires pour permettre au robot de suivre la trajectoire désirée.

### Calcul de la vitesse angulaire désirée

Dans un premier temps, la vitesse angulaire désirée pour l'effecteur est calculée en utilisant la rotation initiale et les paramètres de la trajectoire.  
La vitesse angulaire est définie comme le produit de trois éléments : la vitesse normalisée (\(r_{\text{point}}\)), l'angle de rotation (\(\theta\)), et l'axe de rotation (\(u\)). Cette vitesse est exprimée dans le repère initial grâce à la matrice de rotation initiale \(R_{\text{init}}\) :

\[
\mathbf{w}_d = R_{\text{init}} \cdot (r_{\text{point}} \cdot \theta \cdot \mathbf{u})
\]

---

### Utilisation de la Jacobienne pseudo-inverse

La commande est obtenue via la pseudo-inverse de la Jacobienne, calculée pour éviter les singularités.  
Nous utilisons la formule suivante :

\[
\mathbf{J}^* = \mathbf{J}^T \cdot (\mathbf{J} \cdot \mathbf{J}^T + k^2 \cdot \mathbf{I})^{-1}
\]

avec :
- \(k = 0.01\) : constante de régularisation permettant d'éviter les singularités.
- \(\mathbf{I}\) : matrice identité.
- \(\mathbf{J}\) : Jacobienne calculée dans le modèle géométrique.

---

### Composition de la commande

La commande articulaire combine deux composantes :
1. **Commande de position** : 
   Inclut la vitesse cartésienne désirée (\( \mathbf{x}_p^{\text{désire}} \)) et une correction proportionnelle à l'erreur de position (\(K_p \cdot e_{\text{position}}\)).
2. **Commande d'orientation** :
   Inclut la correction proportionnelle à l'erreur d'orientation (\(K_o \cdot e_{\text{orientation}}\)) et les termes liés à la vitesse angulaire désirée (\(L^T \cdot \mathbf{w}_d\)).

La commande finale est donnée par :

\[
\mathbf{q}_p^{\text{desire}} = \mathbf{J}^* \cdot 
\begin{bmatrix}
\mathbf{x}_p^{\text{désire}} + K_p \cdot \mathbf{e}_{\text{position}} \\
L^{-1} \cdot \left(L^T \cdot \mathbf{w}_d + K_o \cdot \mathbf{e}_{\text{orientation}}\right)
\end{bmatrix}
\]

avec :
- \(L\) : matrice auxiliaire calculée lors du calcul de l'erreur.
- \(\mathbf{x}_p^{\text{désire}}\) : vitesse cartésienne désirée.
- \(e_{\text{orientation}}\) et \(e_{\text{position}}\) : erreurs en orientation et position.
- \(K_o\) et \(K_p\) : coefficients de proportionnalité, fixés ici à \(1.5\).

---

## 5. Commande des Moteurs

Contrairement au robot réel, les articulations de l'UR10 dans *CoppeliaSim* sont contrôlées en position, pas en vitesse.  
Les vitesses articulaires désirées (\(\mathbf{q}_p^{\text{desire}}\)) calculées par le contrôleur doivent donc être intégrées pour déterminer les nouvelles positions articulaires.

### Mise à jour des positions articulaires

Les nouvelles positions sont obtenues par intégration discrète :

\[
\mathbf{q}_{\text{new}} = \mathbf{q}_p^{\text{desire}} \cdot \Delta t + \mathbf{q}_{\text{current}}
\]

Où :
- \(\mathbf{q}_{\text{new}}\) : nouvelles positions articulaires.
- \(\mathbf{q}_p^{\text{desire}}\) : vitesses articulaires désirées.
- \(\Delta t\) : intervalle de temps écoulé depuis la dernière mise à jour.
- \(\mathbf{q}_{\text{current}}\) : positions actuelles des articulations.

Cette méthode simple et linéaire est adaptée aux cycles de commande fréquents et garantit des transitions fluides dans les trajectoires calculées.



# Implémentation

## Langages et bibliothèques utilisés

Pour programmer la simulation et le contrôle du robot UR10, nous avons utilisé les outils suivants :
- **Python** pour les calculs et la commande.
- **Numpy** pour les manipulations matricielles.
- **Matplotlib** pour afficher les courbes.
- **CoppeliaSim** pour la simulation du robot.

## Étapes de la simulation

### 1. Connexion à CoppeliaSim

Une connexion avec *CoppeliaSim* est établie pour contrôler le robot. Les *handles* des articulations sont récupérés afin de pouvoir les piloter. Nous avons utilisé le mode `simx_opmode_blocking` pour récupérer l'angle des moteurs.

### 2. Génération de la trajectoire

La trajectoire est générée en interpolant entre la position initiale et la position finale en utilisant des polynômes de degré 5. Cela garantit des transitions douces et continues en position, vitesse et accélération.

### 3. Calcul des commandes des articulations

Pour chaque instant \( t \), les étapes suivantes sont effectuées :
1. Calcul de la cinématique directe pour déterminer la position actuelle de l'effecteur.
2. Comparaison avec la position désirée pour obtenir l'erreur.
3. Utilisation d'une loi de commande pour déterminer les vitesses articulaires désirées.
4. Intégration pour obtenir les nouvelles positions des joints.

### 4. Envoi des commandes à CoppeliaSim

Les positions calculées sont envoyées aux articulations du robot dans *CoppeliaSim* via les commandes API. Nous avons utilisé le mode `simx_opmode_blocking` pour commander les moteurs.

---

# Courbes et résultats

## Position désirées et actuelles des joints au cours du temps

![Position des joints](position.png)

## Erreur normalisée

![Erreur normalisée](erreur.png)

## Vitesse désirées et actuelles des joints au cours du temps

![Vitesse des joints](vitesse.png)

## Angles désirées et actuelles des joints au cours du temps

![Angles des joints](angle.png)

---

# Conclusion

En conclusion, la simulation du robot UR10 sur *CoppeliaSim* fonctionne, même si elle passe par une singularité.  
Dans ce projet, nous avons été amenés à :
1. Utiliser la méthode Denavit-Hartenberg modifiée pour modéliser le robot.
2. Calculer le modèle géométrique directe pour piloter l'effecteur en position et en orientation.
3. Implémenter un contrôleur pour garantir le suivi de la trajectoire.

Je n'ai cependant pas eu le temps de tester ce programme sur le robot réel, bien que j'aie écrit le code pour cela.
