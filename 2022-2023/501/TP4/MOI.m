close all
clc

%%%- Théorie -%%%
netlist={
    'R1 in 1 R'
    'R2 1 e+ R'
    'C1 1 out C1'
    'C2 e+ 0 C2'
    'X1 e+ out out'
    'V1 in 0 Ve'};
[X,name]=fspice(netlist)
H=X(4)/X(3);
H

syms Ca Cb
%[Q,w0] = solve(1/(Q*w0) == 2*Ra*Ca*Cb , 1/(w0) == Ra*sqrt(Ca*Cb),w0,Q)
w0 = 2*pi*1e4;Q=2;R=1e3;
[Ca,Cb] = solve([1/(Q*w0) == 2*R*Cb , 1/(w0^2) == R^2*Ca*Cb],[Ca,Cb]);

C2 = 3.3e-9;
C1 = 47e-9;

%%%- Pratique -%%%

%.% Première partie
% Créer un tableau
% Premiere colonne fréquence
% Seconde colonne Ve
% Troisième colonne Vs
% save [nom] [nom fichier]
% Tracer a chaque ajout de points.
% semilogx(m(:,1),20*log10(m(:,3)./m(:,2)),'s')

%.% Seconde partie
% Régler w0: régler déphasage vs/ve = 90° car w0 quand déphasage = 90°
% Régler m: Dépassement(%) = 100*exp(-pi*m/(sqrt(1-m^2))
% avec Q=2, m=1/4 => D=100*exp(-pi*0.25/sqrt(1-.25^2))=44.1%
% Donc pour Ve = 920mV, Vsdépassement = 0.920*(1+0.44) = 1.33V


%%% Bode
% Gain max et fréquence pour cette valeur max
% G = double(20*log10(abs(subs(H))))
% [Gmax,id] = max(G)
% fmax = ft(id)

ft=logspace(2,6,1000);
s=2*pi*ft*1i;
semilogx(ft,20*log10(abs(subs(H))),'b')
hold on
% semilogx(fp,20*log10(abs(Hp)),'r')
xlabel('Fréquence (Hz)')
ylabel('Module (dB)')
legend('Théorique','Pratique')
grid on

