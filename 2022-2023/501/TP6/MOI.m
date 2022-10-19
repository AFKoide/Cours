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
R_TD = 100*(1+0.00385.*T); % Platine => coef de resistance à la chaleur plus élevé que les autres métaux normalement utilisés.
% Thermocouple : effet silbet != effet peltier => Courant dans jonction PN crée diff de température, cette diff permet de réguler température. 
% Donc Silbet crée tension avec diff de température.
% Sinon silicium : principe embalement thermique, + chaud, + rapidement Vmax

Vout=subs(Vout);
Vout1=0.1*(1+0.00385.*T);

% T = ((R_TD/100)-1)/0.00385;
% T = (Vout-0.1)/0.00385;

subplot(2,1,1)
plot(T,R_TD)
subplot(2,1,2)
plot(T,Vout1)
