clc;close all;clear all;

%partie 1

Vbe=[0.63 0.67 0.68 0.71 0.72]
bet=[260 174 173 173 172];
Ic=[0.91 5.05 9.2 13.5 17.6];Ic=Ic*1e-3;
Ib=[3 25 48 71 94 ];Ib=Ib*1e-6;

figure
plot(Ic,bet,'k')
xlabel('Courant de collecteur')
ylabel('Gain statique')
title('β-Ic')
grid on

figure
semilogy(Vbe,Ic,'k')
xlabel('Tension de Vbe')
ylabel('Courant Ic')
title('Ic-Vbe')
grid on

figure
semilogy(Vbe,Ib,'k')
xlabel('Tension de Vbe')
ylabel('Courant Ib')
title('Ib-Vbe')
grid on

delta=(log(4.8e-5)-log(3e-6))/(0.68-0.63)
