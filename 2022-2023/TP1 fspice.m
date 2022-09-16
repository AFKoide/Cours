clear all
close all
clc

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
