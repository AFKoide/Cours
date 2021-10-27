% Init
clc
clear all
close all

% Constante
R1= 10;
R2= 8;
C= 100e-6;
E= 10;

% Calcul
syms t p;

% syms R1 R2 E C;

Sp= (E/p)-((R1*E)/(R1+R2))*(1/(p+1/(R1*C+R2*C)));
Ip= (E/(R1+R2))*(1/(p+1/(R1*C+R2*C)));

St= ilaplace(Sp, p, t);
It= ilaplace(Ip, p, t);

% Affichage
disp('It=')
disp(It);
disp('St=');
disp(St);


t= linspace(0,0.5e-3,1000);

figure(1);
set(gcf, 'Name', 's(t)')
st= subs(St, t);
plot(t, st);
figure(2);
it= subs(It, t);
set(gcf, 'Name', 'i(t)')
plot(t, it);
