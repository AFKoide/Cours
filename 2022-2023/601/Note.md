{:toc}

## TP1) Introduction
https://www.analog.com/media/en/technical-documentation/data-sheets/ad633.pdf <br>
https://www.youtube.com/watch?v=x_APePdtrfQ <br>

_Modulation amplitude_ :

l'amplitude de l'intégralité du signal varie en fonction d'un paramètre (exe: le temps).
Si on génere un signal et qu'il varie en fonction de cette modulation, on aura alors un signal
<br><br>
Oscillateur resistance negative <br>
circuit LC <br>

LC + Diode -. <br>
Charge quand Vc > Ve donc diode bloque. <br>
Décharge petit a petit quand Ve > Vc <br>


Diode Schottky -> Seuil de passage environ 0V & temps pour passage environ 0 <br>


Ordre des Modules <br>
E1. Oscillateur
Crée une modulation avec une charge et une décharge.
`f=1/(2*pi*sqrt(R*C))` (environ 84kHz.)

E2. Multiplicateur
Multiplie l'oscillateur (le porteur) avec un signal (moduleur).

`s(t)= k*cos(2*pi*f*t)*[1+alpha*cos(2*pi*f0*t)]`
alpha compris [-1;1] -> Niveau

E3. Gain Puissance
Multiplie courant (sans toucher tension) puis ca va dans antenne. Antenne trop petite -> champ magnétique (et pas electromagnetique)

R1. Reception
Receptionne le signal et 

R2. Filtre
Passe Haut le signal

R3. 

R4. Haut Parleur
Emet le signal et fait office de passe bas (bobine).




## TP2) L'oscillateur
##### PAGE 211.
On utilise un oscillateur à pont de Wien. <br>
<br>
On doit avoir un gain de 3. On a amplificateur inverseur (1+R2/R1) et on veut un petit courant donc R2 et R1 >= 1k.




## TP3) Le PLL
##### PAGE 217 & 219
Detecteur d'enveloppe => Pour démoduler le signal.

Transmission -> Signal envoyé = Signal recu (+Bruit)
On n'utilise pas la modulation d'amplitude car le bruit fait varier l'amplitude (Rapport signal/bruit) -> On utilise donc modulation de fréquence.
Problème : 

On a le signal porté a 10k et le portant a 2ft.
Si on utilise un filtre RC simple, on risque de ne pas assez atténuer arriver a 2ft, donc on prend ordre 2.

On doit utiliser pour le signal du OL la meme frequence que la porteuse. Sauf que c'est pratiquement impossible : même avec une frequence précise a 5 chiffres apres la virgule, on a pas le filtrage parfait.
On va récupérer alors la fréquence de la porteuse avec le PFF.


PFF -> CD4046 -> VCO.
Le VCO génère une tension avec une fréquence proportionnelle à la tension en entrée. On utilise une résistance et une capacité pour déterminer le parametre lineaire du VCO. <br>
`f0 = Vdd/2`
<br>
Dans le 4046, il y a aussi un OU EXCLUSIF. Il est utilisé avec le VCO. 
<br>
On met une capacité de liaison avant le OU EXCLUSIF pour avoir le signal centré sur Vdd/2.
Plage de capture. Bruit de phase.
On veut Plage petite.  
`fl=228e3; sqrt(2*pi*fl/(1000*10e-9))/(2*pi)`
