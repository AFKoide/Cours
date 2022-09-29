clear all
close all
clc

ft = [100 200 500 700 1000 2000 5000 7000 10000 20000 50000 70000];

Ve10 = [2.16 2.16 2.16 2.12 2.16 2.16 2.16 2.16 2.16 2.16 2.24 2.08]; % Il y a une résonnance à 7000Hz
Vs10 = [2.16 2.16 2.20 2.12 2.08 1.96 1.36 2.9 0.68 0.32 0.084 0.0352];
H10 = Vs10./Ve10;

Ve180 = [2.16 2.16 2.16 2.14 2.16 2.08 1.92 1.88 1.92 2.08 2.08 2.08];
Vs180 = [2.14 2.14 2.14 2.10 2.14 2.06 1.66 1.22 0.9 0.32 0.064 0.06];
H180 = Vs180./Ve180;

Ve330 = [2.12 2.12 2.12 2.12 2.1 2.08 1.96 1.92 2 2.04 2.08 2.06];
Vs330 = [2.12 2.12 2.12 2.12 1.92 1.6 1.18 0.992 0.456 0.308 0.12 0.0368];
H330 = Vs330./Ve330;

w = 2*pi.*ft;

G10 = 20.*log10(abs(H10));
G180 = 20.*log10(abs(H180));
G330 = 20.*log10(abs(H330));

% Affichage
figure(1)
set(gcf, 'Name', 'RLC : Bode Expérimental')
semilogx(w, G10,'k');
hold on
semilogx(w, G180,'g');
hold on
semilogx(w, G330,'r');
set(get(gca, 'XLabel'), 'String', 'Pulsation (rad/s)');
set(get(gca, 'YLabel'), 'String', 'dB');
legend('R=10', 'R=180', 'R=330');
grid on

% figure(2)
% set(gcf, 'Name', 'RLC : Bode Théorique')
% semilogx(w, G10,'k');
% hold on
% semilogx(w, G180,'g');
% hold on
% semilogx(w, G330,'r');
% set(get(gca, 'XLabel'), 'String', 'Pulsation (rad/s)');
% set(get(gca, 'YLabel'), 'String', 'dB');
% legend('R=10', 'R=180', 'R=330');
% grid on

%%%- LCR -%%%
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
