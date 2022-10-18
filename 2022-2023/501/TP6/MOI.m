clc
close all

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
% pretty(Vout)
% T = (Vout-0.1)/0.00385