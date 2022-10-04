%% Calcul de la fonction de transfert avec R1,R2,R3 et R4 
netlist=...
    {'R1 Vin 1 R1'
    'R2 1 e+ R2'
    'C1 1 Vout C1'
    'C2 e+ 0 C2'
    'X1 e+ e- Vout'
    'R3 e- Vout R3'
    'R4 e- 0 R4'
    'V1 Vin 0 Ve'};
[X,name]=fspice(netlist);
H=X(3)/X(2);
pretty(H)
