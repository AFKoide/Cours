clc
close all

syms Rref

netlist={
 'X1 e2+ out2 out2'
 'R3 out2 e1+ R1'
 'R4 e1+ 1 R2'
 'X2 e1+ e1- out1'
 'R1 e1- 0 R3'
 'R2 e1- out1 R4'
 'Rrref out1 e2+ Rref'
 'V1 1 0 Vref'
 'Rpt e2+ 0 R_TD'
 };
[X name]=fspice(netlist);
Vout=X(4);
Irref=simplify((X(4)-X(3))/Rref);


Vref=2.5;R=25e3;Rref=2.5e3;
R1=R;R2=R;R3=R;R4=R;

T = linspace(0,100,1000);
R_TD = 100*(1+0.00385.*T); 


Vout=subs(Vout);
Irref=subs(Irref);
Vout1=0.1*(1+0.00385.*T);

plot(Irref)

R-TD = [100 200 300 400 500]
Vout = [  ]
V1 = []
V2 = []

% T = ((R_TD/100)-1)/0.00385;
% T = (Vout-0.1)/0.00385;

% Platine => coef de resistance à la chaleur plus élevé que les autres métaux normalement utilisés.
% Thermocouple : effet silbet != effet peltier => Courant dans jonction PN crée diff de température, cette diff permet de réguler température. 
% Donc Silbet crée tension avec diff de température.
% Sinon silicium : principe embalement thermique, + chaud, + rapidement Vmax
% ELECTRONIQUE DE CONDITIONNEMENT

% pont de wheatstone 


% Modele parfait ne prend pas en compte de l'offset, les pertes des AOPs et
% les différentes inconsistances qui causent des parasites (soudures, rouille etc.)
% parfait change la valeur de R_TD

% Théorème de Shanon : fréquence échantillonnage doit être inférieur à la fréquence du signal.
% L'échantillonnage amplifie le bruit du signal -> filtre d'


% Relation entre Vsortie et Ventree reste toujours la même pour un AOP sauf
% si on (dé)branche un dipôle.
