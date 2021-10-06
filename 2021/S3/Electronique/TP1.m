% Init
clc
close all
clear all

% Constante
E = 100;

R1 = 100;
R2 = 200;
R3 = 400;
R4 = 300;
R = 500;

% Matrice
A = [(1/R1)+(1/R2)+(1/R3) -(1/R3) ; (1/R3) -(1/R3)-(1/R4)-(1/R5)];
B = [I0+(E/R1) ; I0];

% Calcul
X = (A)^-1*B;

% Résultat 
Va = X(2);
I = Va/R5;
