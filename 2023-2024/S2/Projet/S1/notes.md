# Projet Driver Kinect v2



## Cahier des charges
On connecte la kinect à l'ordinateur puis on charge les données de calibrations.
Après avoir donné l'ordre, la caméra enregistre toutes les frames IR et couleurs capturées pendant un délai. A la fin du délai, le programme se termine.
On superpose les deux images sur un seule image que l'on enregistre dans un dossier.

## Proposition
3 nodes:
- Le bridge -> S'occupe de détecter la caméra et de charger les données de calibration.
- Le Record -> Récupère et enregistre les images.
- Le Register -> Superpose les deux caméras.
Les fichiers de configurations devront être chargé à partir de fichiers locaux, comme cela est réalisé jusqu'à présent .



## La Kinect 2:
Dans le cadre de notre projet, nous nous sommes servi d'une caméra type Kinect version 2. Cette caméra est très populaire dans le domaine de la robotique pour son rapport qualité/prix


### Caractéristique techniques
La Kinect v2 est la kinect commercialisé avec la Xbox One. Elle est équipée de deux caméras :
- une caméra *Couleur* de résolution **1920x1080px** ayant une portée optimale comprise entre 0,5m à 4,5m. Dans la pratique, la caméra peine à distinguer les outils de calibrations à partir de 0,2m à 3m.
- une caméra *InfraRouge* de résolution **512x424px** ayant une portée optimale de équivalente à l'autre caméra. En pratique, la portée maximale optimale de cette dernière est de 2m environ.
Toutes deux ont un frame rate de 30 fps avec une latence de 20ms.
Le périphérique utilise une prise USB 3.0.
Comparé à la version v1 commercialisée avec la Xbox 360, la version v2 offre une bien meilleure qualité d'image pour une consommation énergétique plus importante.


### Caméra Couleur
La caméra couleur de la kinect est principalement utilisée pour faire de la reconnaissance : reconnaissance faciale, détection d'objets et de couleur...


### Caméra IR.
La caméra x est utilisée pour mesurer la profondeur de champ de l'environnement.

La Kinect mesure la profondeur de champ à l'aide de la technologie ToF (Time of Flight). Comme son nom l'indique, cette technologie repose sur le principe de mesurer le temps qu'il faut à un signal lumineux pour faire un aller-retour entre le capteur et la cible : après qu'un signal ait été émis, on va mesurer le temps qu'il a fallu pour que ce dernier soit relevé par le capteur après avoir été réfléchi par l'environnement. Il suffira alors de faire le produit de ce temps avec la vitesse du signal pour ainsi obtenir une mesure de la profondeur.

Dans le cas de la Kinect, ce processus est réalisé avec un signal infrarouge, une lumière invisible pour l’œil humain.


### Avantage / Inconvenients
Bien loin de son utilisation d'origine, la kinect est très utilisé dans le domaine de la recherche robotique.
Un de ses principale avantage est son coût : 200€ pour une caméra embarquant une caméra couleur et un capteur infrarouge. Séparément, chacun de ses composants coutent déjà plus de 100€, et pour des capteurs ayant des résolutions inférieures.
La méthode utilisée pour calculer la profondeur de champ est extrêmement fiable, pouvant fonctionner dans n'importe quel milieu, ou sur des surfaces non-rigides. 

Cependant, de nombreux inconvénients apparaissent causée par le détournement de l'utilisation première de cette caméra. Deux des inconvénients sont liés à l’ergonomie de l'appareil : ainsi, le boîtier de la caméra à lui seul pèse plus d'un kilo (1.4kg sans les câbles), et ses dimensions imposantes (66 x 249 x 67 mm) font que cette caméra n'est pas adapté à tous les robots mobiles. Un autre est la consommation énergétique de la Kinect v2, importante (environ 30W) contre environ 15W pour une combinaison d'une caméra couleur et un capteur IR.
La Kinect nécessite une distance minimale d'affichage, ce qui la disqualifie d'applications où le capteur devra être très proche des obstacles. Enfin, le champ de vision des caméras restreindra encore plus les cas d'applications des caméras.


## Libfreenect2
*Libfreenect2* est une librairie open-source utilisée pour interagir avec les capteurs Kinect v2. Elle offre un accès programmatique aux données provenant de ces capteurs. Développée en C++, elle propose une interface pour l'acquisition de flux vidéo RGB et de données de profondeur à partir de la Kinect v2. La librairie prend en charge divers systèmes d'exploitation, notamment Linux, macOS et Windows, ainsi que l'accélération matériel pour accélérer le traitement des images.
Les fonctionnalités de libfreenect2 incluent la gestion des données de profondeur, des images infrarouges, et des images couleur provenant de la Kinect v2. Elle offre également des fonctionnalités avancées telles que la possibilité de capturer des images synchronisées et de configurer des paramètres spécifiques du capteur.

Le logiciel a cependant quelques problèmes, notamment la gestion de plusieurs caméras ou encore son obsolescence : Libfreenect2 n'est pas compatible avec les versions d'opencv supérieure à la 3.0 (nous sommes actuellement à la 4.0), ainsi qu'avec ROS2.
Dans le cadre de notre projet, nous nous sommes exclusivement servi de libfreenect2 pour contrôler la caméra (nous avons modifié le programme pour le rendre compatible avec OpenCV 4.0).




## La Calibration
La calibration dans le contexte de la vision par ordinateur et de l'utilisation de caméras vise à corriger les distorsions géométriques et photométriques afin d'obtenir des mesures et des représentations précises de la scène. Nous nous sommes servi d'OpenCV, une bibliothèque populaire de vision par ordinateur, pour réaliser la calibration de la Kinect.
Ce processus consiste à déterminer les paramètres intrinsèques et extrinsèques de la caméra, tels que la matrice de caméra, les coefficients de distorsion et la transformation entre l'espace caméra et le monde réel. La calibration garantit une meilleure qualité d'image et une interprétation plus précise de la scène par les applications de vision par ordinateur.

Dans le cadre de notre projet, nous nous sommes servis de **iai_Kinect2** pour réaliser la calibration.
Pour réaliser la calibration, nous avons réalisé la capture d'un grand nombre d'images contenant un damier (fourni par le logiciel) sous différentes angles et distances par rapport à la caméra. Nous avons fait attention que le motif était bien reconnu dans chaque image capturée.


### OpenCV
Le but est de trouver la matrice de transformation 3x3, la matrice de rotation 3x3 et la matrice de translation 3x1 à l'aide de coordonnée 3d (que l'on appelle coordonnées monde) et des coordonnées 2d de l'image.
Les coordonnées monde sont obtenues à l'aide du damier dont on connaît les dimensions : on va définir des points qui sont les coins de chaque carré noir.

#### Le damier
*insérer le damier*


## Le Code



# Sources
### Kinect
https://www.research-collection.ethz.ch/bitstream/handle/20.500.11850/104272/eth-48073-01.pdf;jsessionid=584053728D2C43DAEA2B0A0869749117?sequence=1
https://www.ncbi.nlm.nih.gov/pmc/articles/PMC9002889/

### OpenCV
https://learnopencv.com/camera-calibration-using-opencv/
https://docs.opencv.org/4.x/d9/d0c/group__calib3d.html