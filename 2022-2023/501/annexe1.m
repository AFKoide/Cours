netlist=...
    {'R1 Vin 1 R';
    'R2 1 e+ R';
    'C1 1 Vout C1';
    'C2 e+ 0 C2';
    'X1 e+ Vout Vout'
    'V1 Vin 0 Ve'};
[X,name]=fspice(netlist);
H=X(3)/X(2);
pretty(H)
