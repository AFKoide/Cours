clc
close all
clear all

%------------------ Exercice 1
C = 150e-12;
R = 470e3;


V1 = [2.05 ; 2.05 ; 2.05 ; 2.05 ; 2.05 ; 2.05 ; 2.05 ; 2.05 ; 2.05];
V1s = V1;

V2 = [1.37 ; 1.37 ; 1.33 ; 1.21 ; 0.930; 0.480; 0.270; 0.140; 0.07];
V2s= [1.97 ; 1.97 ; 1.93 ; 1.79 ; 1.45 ; 0.800; 0.460; 0.240; 0.14];

%%% Bode %%%
% Calculs
f = [100;200;500;1e3;2e3;5e3;10e3;20e3;50e3];

w = 2*pi.*f;
wt = logspace(2,5.5,1000);

H  = V2./V1;
Hs = V2s./V1s;
Ht = 1./(1+1i*R*C.*wt);

G  = 20.*log10(abs(H));
Gs = 20.*log10(abs(Hs));
Gt = 20.*log10(abs(Ht));

% Affichage
figure(1)
set(gcf, 'Name', 'Gain')
semilogx(w, G, 'r');
hold on
semilogx(w, Gs, 'b');
hold on
semilogx(wt, Gt, 'k');
set(get(gca, 'XLabel'), 'String', 'Pulsation (rad/s)');
set(get(gca, 'YLabel'), 'String', 'dB');
legend('Gain Oscilloscope', 'Gain Sonde', 'Gain Théorique');


%------------------ Exercice 2

