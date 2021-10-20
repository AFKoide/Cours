% Init
clc
clear all
close all

%%%%%- Exercice 2 -%%%%%
% Constante
R = 1e3;
C = 10e-6;
L = 0.1;

w = logspace(0,6,1000);
% Calculs
H = ((R*C*L)*w.^2)./(R+(1i*L*w)-((R*C*L)*w.^2));
% H = 1./(1+1i*R*C*w)

G = 20*log10(abs(H));

Phi = angle(H)*180/pi;

% Affichage
figure(1)
semilogx(w, G);
set(gcf, 'Name', 'Gain')

figure(2)
semilogx(w, Phi);
set(gcf, 'Name', 'Phase')



%%%%%- Exercice 3 -%%%%%
