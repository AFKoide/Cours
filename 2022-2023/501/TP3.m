clc
close all
clear all

%%%- RLC -%%%
syms w C R L Ve

netlist = {'Vsource in 0 Ve';'R1 in tmp R';'L1 tmp out L';'C1 out 0 C'};
[X, name] = fspice(netlist)

Hrlc = X(2)/X(1)

I = ilaplace(X(4))


%%%- Transfert -%%%
R1=100;R2=300;R3=450;
C=100e-9;L=4.7e-3;

Hrlc1 = subs(Hrlc,R,R1);
Hrlc2 = subs(Hrlc,R,R2);
Hrlc3 = subs(Hrlc,R,R3);

%%% Bode %%%
f=logspace(2,6,1000);
s=2*pi*f*1i;
semilogx(f,20*log10(abs(subs(Hrlc1))),'k')
hold on
semilogx(f,20*log10(abs(subs(Hrlc2))),'g')
hold on
semilogx(f,20*log10(abs(subs(Hrlc3))),'r')
xlabel('Fréquence (Hz)')
ylabel('Module (dB)')
legend('R=100', 'R=300', 'R=450');
