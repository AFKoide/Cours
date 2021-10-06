% Init
clc
close all
clear all

% Constante
R1 = 10;
R2 = 15;
R3 = 8;
R4 = 15;
R5 = 5;
I0 = 0.1;
E = 10;

% Matrice
A = [(1/R1)+(1/R2)+(1/R3) -(1/R3) ; (1/R3) -(1/R3)-(1/R4)-(1/R5)];
B = [I0+(E/R1) ; I0];

% Calcul
X = (A)^-1*B;

disp(X);
