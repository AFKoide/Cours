clear;

% R1/R = 29 <=> R1=29k et R=1k
% C fixe frequence car R fix donc f = 1k <=> C = 6.4975e-08 ou 6.5e-8

netlist={
    uA741('0','e-','out')
    'R1 e- out 29e3'
    'C1 out d1 6.5e-8'
    'R3 0 d1 1e3'
    'C2 d1 d2 6.5e-8'
    'R4 0 d2 1e3'
    'C3 d2 d3 6.5e-8'
    'R2 d3 e- 1e3'
    '.tran tstop=10e-3 dtmax=1e-5 initTRAN=CI'
    '.probe out d3'
    };
sim=uacs(netlist)

R=1e3; R1=29e3;
C=6.5e-8;

f = 1/(2*pi*R*C*sqrt(6))

plot(sim.t,sim.X_t(sim.probe_id(1:2),:))
grid
legend('Vout','Vin')

% On a un offset de 2e-3V causé par l'AOP. On pourrait le corriger avec un générateur.
