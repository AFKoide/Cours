% Init %
clc 
close all
clear all

%%%- Exercice 1 -%%%
%Constante %

t = linspace(0,10,100);

% Calcul %
syms e 
syms i1 R1 L
i1 = dsolve('Di1 + (R1/L)*i1 = e/L', 'i1(0) = 0');


syms i2 R2 C
i2 = dsolve('Di2 + (1/(R2*C))*i2 = 0', 'i2(0)=e/R2');


R1 = 100;
R2 = 100;
C = 0.1;
L = R1^2*C; %L=power(R1,2)*C ; L=0.1
e = 10;


I1 = subs(i1);
I2 = subs(i2);

I = I1+I2;

% Affichage %
figure(1)
plot(t,I1, 'r', t,I2, 'g', t,I, 'b')
legend('I1', 'I2', 'I')

%%%- Exercice 2 -%%%
