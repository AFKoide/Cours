clear all
close all
clc

f = 50;
t = linspace(0,10,1000);

w = 2*pi*f;
ve = 230*sqrt(2)*sin(w.*t);

Rf = 2900;
Lm = 0.63;