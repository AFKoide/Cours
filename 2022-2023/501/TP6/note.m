netlist={
    'Ve e+ 0 Ve'
    'X1 e+ e- out'
    'R1 e- 0 R'
    'R2 out e- R'
    'Rl out 0 Rl'};
[X,name]=fspice(netlist) 
H = X(3)/X(1) % Gain de 2.

% Montage avec AOP permet de coller des morceaux de circuit ensemble au
% lieu de faire bout pas bout car montage AOP a une impédance de sortie nulle 
% Fonction de transfert = produit de toutes les fonctions

netlist={
    'V2 e+ 0 V2'
    'Zs e+ 0 Z'};
[X,name]=fspice(netlist) % On voit que I=0 donc I=V/Z -> Z infini.

% Emetteur commun/Collecteur commun -> Suiveur
% Collecteur commun => impédance entrée grande - impédance sortie faible
% Emetteur commun 
