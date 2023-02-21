- [Astuce](#astuce)
- [TP1 : Introduction](#tp1--introduction)
- [TP2 : L'oscillateur](#tp2--loscillateur)
      - [PAGE 211.](#page-211)
- [TP3 : Le PLL](#tp3--le-pll)
      - [PAGE 217 \& 219](#page-217--219)


## Astuce

- On met les capacités le plus proche possible du circuit pour retirer ce qui est "caca".

## TP1 : Introduction
https://www.analog.com/media/en/technical-documentation/data-sheets/ad633.pdf <br>
https://www.youtube.com/watch?v=x_APePdtrfQ <br>

_Régime Harmonique_ :  => Si on sait ce qui se passe en harmonique, alors on saura dans tout cas de figure.



Oscillateur resistance negative <br>
circuit LC <br>

LC + Diode -. <br>
Charge quand Vc > Ve donc diode bloque. <br>
Décharge petit a petit quand Ve > Vc <br>


Diode Schottky -> Seuil de passage environ 0V & temps pour passage environ 0 <br>


Ordre des Modules :
- E1. Oscillateur <br>
Crée une modulation avec une charge et une décharge.
$f=1/(2*pi*sqrt(R*C))$ (environ 84kHz.)

- E2. Multiplicateur <br>
Multiplie le signal en sortie de l'oscillateur (le porteur) avec un signal que l'on souhaite transporter (le moduleur).

$s(t)= k*cos(2*pi*f*t)*[1+alpha*cos(2*pi*f0*t)]$
alpha compris [-1;1] -> Niveau

- E3. Émetteur <br>
Multiplie courant (sans toucher tension) puis ca va dans antenne. Antenne trop petite -> champ magnétique (et pas electromagnetique)

- R1. Reception <br>
Receptionne le signal et met un filtre passe bas avec une amplification.

- R2. Filtre <br>
Passe Haut le signal

- R3. Gain de Puissance
Augmen

- R4. Haut Parleur <br>
Émet le signal en son audible et fait office de passe bas (bobine) (c'est pas fait pour).




## TP2 : L'oscillateur
##### PAGE 211.
On utilise un oscillateur à pont de Wien. <br>

- On doit avoir un gain de 3. On fait amplificateur inverseur (1+R2/R1) et on veut un petit courant sur le - donc R2 et R1 >= 1k.<br>
- On met en contre réaction (coté positif) une capacité et une résistance suivi d'un RC. C'est ce coté qui va générer l'oscillation.


On utilise un détecteur d'enveloppe (circuit RC) pour connaître la valeur max du signal qui ondule. On relie ce détecteur à un JFET  qui fera office d'une résistance et, avec un potentiomètre, va remplacer R2. Ainsi, la valeur de R2 variera en fonction de la tension de sortie (si tension trop haute, alors R2 diminue ; inversement).<br>

LE PRODUIT GAIN BANDE<br>
$GB = Gain*BandePassante = cste$ en V/V (tension par tension).
Slew Rate = V*W



## TP3 : Le PLL
##### PAGE 217 & 219
Pour démoduler le signal, on a deux méthodes : détecteur d'enveloppe et modulation de fréquence.

On n'utilise pas la modulation d'amplitude a cause du rapport signal/bruit. La transmission fait que Signal Envoyé = Signal Recu + Bruit. Le bruit fait varier l'amplitude ce qui fausse. 

-> On utilise donc modulation de fréquence.

Le montage de la modulation de fréquence est composé d'un multiplexeur et d'un filtre. On multiplie le signal en entrée par la porteuse (OL) puis on filtre. <br>
On a le signal porté a 10k et le portant a 2ft.
Si on utilise un filtre RC simple, on risque de ne pas assez atténuer arrivé a 2ft, donc on utilise un filtre passe bas d'ordre 2 (deux RC en série font le taf).

Problème : il faut que la modulation se fasse a la meme fréquence que la porteuse. Même avec un GBF réglé le plus précisément possible, on ne peut pas faire la modulation. <br>
On doit utiliser pour le signal du OL la meme fréquence que la porteuse. Sauf que c'est pratiquement impossible : même avec une frequence précise a 5 chiffres apres la virgule, on n'a pas un filtrage correct. <br>
On va alors récupérer la fréquence de la porteuse avec le PFF.

Le PFF est composé d'un VCO, d'une porte logique XOR et d'un filtre RC.

CD4046 -> VCO.
Le VCO génère une tension avec une fréquence proportionnelle à la tension en entrée. On utilise une résistance et une capacité pour déterminer le paramètre linéaire du VCO. <br>
$f_0 = Vdd/2$ & $2f_0 = Vdd$

Dans le 4046, il y a aussi un OU EXCLUSIF. Il est utilisé avec le VCO. Il compare la tension d'entrée, qui passe avant dans une capacité de liaison centrer le signal sur Vdd/2, et la tension de sortie du VCO.
On doit avoir en sortie du filtre une tension ayant une fréquence deux fois supérieure au signal d'entrée, et on l'a si le signal en sortie du VCO a un déphasage de $pi/2$.


Plage de capture. Bruit de phase.
On veut Plage petite.  
$f_l=228e3; ? = sqrt((2*pi*fl)/(1000*10e-9))/(2*pi)$

On a un signal sinusoidal -> on fait un signal carré avec une frequence deux fois plus grande que le signal sinusoidal.

S'accroche sur la porteuse car les autres ont une trop grosse fréquence.

Trigger ne doit pas etre trop grand car sinon on ne risque pas de pouvoir générer le signal carré, faut alors baisser le taux de modulation.

## TP4
On multiplie une fonction de transfert par 1/p pour faire la réponse impulsionnelle d'un système dans laplace. <br>
Le signal en sortie du PLL est de la forme carrée et pas un sinus. Cela a un importance car ce signal est moins pur qu'un sinus (car le carré est composé de plusieurs sinus). On a juste à filtrer avec un meilleur filtre. <br>
Cependant, le signal en sortie du PLL est déphasé de -pi/2. Donc on utilise un AOP déphaseur pour annuler le déphasage (+pi/2). Problème, le montage avec l'AOP souffre du slow-rate,ce qui fait que l'on a pas des carrés parfaits.
