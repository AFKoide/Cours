clc
close all
clear all

%------------------ Normal
syms w C R z1 z2 Ve

MNA = [1/z1 -1/z1 1;-1/z1 (1/z1)+(1/R)+1i*C*w 0; 1 0 0];
RHS = [0;0;Ve];
MNA1 = inv(MNA);

X = MNA1*RHS;

X = subs(X, z1, R+1/(1i*C*w));
X = subs(X, z2, (1/R)+1i*C*w);


X = subs(X, R, 1e3);
X = subs(X, C, 1e-9);

e1 = X(1);
e2 = X(2);
ik = X(3);


%%% Bode %%%

w = logspace(0,10,1000);
% Calculs

H = subs(e2/e1);
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

%% 
%------------------ Fspice
netlist={'Vsource1 1 0 Ve';'R1 1 2 R';'C1 2 3 C';'R2 3 0 R';'C2 3 0 C'};

[X, name] = fspice(netlist)

H = X(3)/X(1);

R = 1e3;
C = 1e-9;

%%% Réponse %%%
syms s
vs=ilaplace(subs(H/s))
t=linspace(0,5*R*C,1000);
plot(t,subs(vs))
xlabel('temps (s)')
ylabel('V_s(t), Volts')
title('Réponse à un échelon')

%%% Bode %%%
R=1e3;C=10e-9;
f=logspace(2,6,100);
s=2*pi*f*i;
subplot(2,1,1)
semilogx(f,20*log10(abs(subs(H))))
xlabel('Fréquence (Hz)')
ylabel('Module (dB)')
subplot(2,1,2)
semilogx(f,angle(subs(H)))
xlabel('Fréquence (Hz)')
ylabel('Angle (rad)')
