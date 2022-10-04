clear all
close all
clc
%%%------------------------------ LCR

%%%- Théorie -%%%
syms w C R L Ve

netlist = {'Vsource in 0 Ve';'L1 in 2 L';'C1 2 out C';'R1 out 0 R'};
[X, name] = fspice(netlist)

Htlcr = X(2)/X(1)

%%%- Transfert -%%%
R1=10;R2=180;R3=330;
C=100e-9;L=4.7e-3;

Htlcr1 = subs(Htlcr,R,R1);
Htlcr2 = subs(Htlcr,R,R2);
Htlcr3 = subs(Htlcr,R,R3);

%%% Bode %%%
% ft=logspace(2,6,1000);
% s=2*pi*f*1i;
% semilogx(ft,20*log10(abs(subs(Hrlc1))),'k')
% hold on
% semilogx(ft,20*log10(abs(subs(Hlcr2))),'g')
% hold on
% semilogx(ft,20*log10(abs(subs(Hlcr3))),'r')
% xlabel('Fréquence (Hz)')
% ylabel('Module (dB)')
% legend('R=100', 'R=300', 'R=450');

netlist = {'Vsource in 0 Ve';'C1 in 2 C';'L1 2 out L';'R1 out 0 R'};
[X, name] = fspice(netlist);

Htlcr = X(2)/X(1);

R=330;C=100e-9;L=4.7e-3;

ft=logspace(2,6,1000);
s=2*pi*1i.*ft;
figure(2)
semilogx(ft,20*log10(abs(subs(Htlcr))))
xlabel('Fréquence (Hz)')
ylabel('Module (dB)')
grid on



ftlcr = [100 200 500 700 1000 2000 5000 7000 10000 20000 50000];
wtlcr = 2*pi.*ftlcr;

Velcr = [1.14 1.14 1.14 1.14 1.12 1.12 1.04 1.02 1.02 1.08 1.12];
Vslcr = [246e-3 472e-3 744e-3 840e-3 900e-3 1.02 1.06 1.04 1.06 1.02 900e-3];

I = Vslcr/330; % Ir = Vr/R avec Vr = Vs ; Vslcr est deja une amplitude. Vs/R donne le module.
% Z = Ve/I = (Ve/Vs)*R

Zlcr = (Velcr./Vslcr)*330


Glcr = Vslcr./Velcr;

figure(3)
set(gcf, 'Name', 'LCR : Bode Expérimental')
semilogx(wtlcr, Glcr,'r');
set(get(gca, 'XLabel'), 'String', 'Pulsation (rad/s)');
set(get(gca, 'YLabel'), 'String', 'dB');
legend('Valeur Réel');
grid on
