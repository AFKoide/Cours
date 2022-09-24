clear all
close all
clc

t = linspace(0,10,1000);

% f = 50;
% w = 2*pi*f;
% ve = 230*sqrt(2)*sin(w.*t);

% Rf = 2900;
% Lm = 0.63;

syms Rf Lm w

Zeq = 1/((1/Rf)+(1/(1i*Lm*w)))
Zeq = solve(Zeq)