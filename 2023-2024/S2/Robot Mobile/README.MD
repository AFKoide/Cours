# PFE n°13:
## Acquisition d’un flux d’images RGBD depuis Microsoft Kinect 2 et détection des informations 3D des points clé du squelette humaine

# Problématique
On souhaite programmer un robot BAZARD pour qu'il puisse reconnaitre les mouvements d'un être humain et supporte ses tâches.

Ce robot fonctionne avec ROS2.
Openpose, la librairie utilisée pour reconnaitre le squelette, n'est pas compatible avec des versions d'Ubuntu supérieure à la 18 (à cause de la librairie CudNN) : il n'est donc pas compatible avec ROS2. De même pour iai_kinect2, une suite d'outils pour la caméra Kinect v2, et qui utilise Libfreenect2 comme pilote, ce dernier n'est pas compatible avec ROS2. 
Le projet consiste à faire fonctionner iai et openpose en tandem sous ROS2 pour pouvoir capturer et traiter en temps réel l'environnement, et ceux avec la Kinect.


#### On uilise OpenPose pour extraire le squelette. 
Pourquoi OpenPose? (Voir le pdf de la prof).



## Chose a faire :
0. [ ] Vérifier que deux versions différentes de ROS2 peuvent communiquer ensemble.
1. [ ] Voir comment OpenPose fonctionne.
2. [ ] Tester OpenPose avec ROS2 Dashing.
3. [ ] Faire fonctionner iai_kinect2 sur ROS2 Dashing.
4. [ ] Voir si tu peux .
5. [ ] Plugger iai et OpenPose ensemble pour que OpenPose fonctionne en temps réel.
6. [ ] Enregistrer le flux vidéo de la kinect et de openpose en meme temps.


TCP pour communiquer ensemble (par défaut, ROS2 utilise UCP).


## Info supplémentaires :
Framework pour coder : PID (outil de compilation comme catkin_make).



COMMENT TROUVER LES JOINTS : 
Utilise IA (CNN) pour détecter les parties corporelles et leur orientations (vecteur) et ensuite calcul (devine) les deux noeuds les plus proches à 
Utilise un algorithme de graph matching pour lier les noeuds ensemble.
