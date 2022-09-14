clc
close all
clear all


%------------------ Exercice 1
syms w C R z1 z2 Ve

MNA = [1/z1 -1/z1 1;-1/z1 (1/z1)+(1/R)+1i*C*w 0; 1 0 0];
RHS = [0;0;Ve];
MNA1 = inv(MNA);

X = MNA1*RHS;

X = subs(X, z1, R+1/(1i*C*w));
X = subs(X, z2, (1/R)+1i*C*w);


X = subs(X, R, 1e3);
X = subs(X, C, 1e-9);

e1 = X(1)
e2 = X(2)
ik = X(3)


%%% Bode %%%

w = logspace(0,10,1000);
% Calculs

H = subs(e2/e1)
% H = laplace(h)

G = 20.*log10(abs(H));
Phi = angle(H)*180/pi;

% Affichage
figure(1)
semilogx(w, G);
set(gcf, 'Name', 'Gain')
set(get(gca, 'XLabel'), 'String', 'Pulsation (rad/s)');
set(get(gca, 'YLabel'), 'String', 'dB');

figure(2)
semilogx(w, Phi);
set(gcf, 'Name', 'Phase')
set(get(gca, 'XLabel'), 'String', 'Pulsation (rad/s)');
set(get(gca, 'YLabel'), 'String', '°');
