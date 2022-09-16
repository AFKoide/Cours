clear all
close all
clc
 
%------------------ Exercice 1
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

%% 

clear all
close all
clc

%------------------ Exercice 2
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