clear all
close all
clc

%------------------ Normal
syms w R1 R2 R3 R4 R5 E0 I0

MNA = [(1/R1)+(1/R2)+(1/R3) -1/R3;-1/R3 (1/R3)+(1/R4)+(1/R5)];
RHS = [(E1/R1)+I0;-I0];
MNA1 = inv(MNA);

X = MNA1*RHS;

E0 = 10; I0 = 0.1; R1 = 10; R2 = 15; R3 = 8; R4 = 15; R5 = 5;

X = subs(X);

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
netlist = {'Vsource 1 0 E0';'Isource0 2 3 I0';'R1 1 2 R1';'R2 2 0 R2';'R3 2 3 R3';'R4 3 0 R4';'R5 3 0 R5'};

[X, name] = fspice(netlist)

H = X(3)/X(1);

E1 = 10;
I0 = 0.1;
R1 = 10;
R2 = 15;
R3 = 8;
R4 = 15;
R5 = 5;

%%% Réponse %%%
syms s
vs=ilaplace(subs(H/s))
t=linspace(0,5*R*C,1000);
plot(t,subs(vs))
xlabel('temps (s)')
ylabel('V_s(t), Volts')
title('Réponse à un échelon')

%%% Bode %%%
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