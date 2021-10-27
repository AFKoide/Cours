% Init
clc
clear all
close all

%%%- Exercice 1 -%%%

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


%%%- Exercice 2 -%%%
% Constante
R= 1e3;
C= 1e-6;


% Calcul
syms t p;

H= (1+power(R*C*p, 2))/(1+4*R*C*p+power(R*C*p, 2));

e1= cos(2*pi*20*t)*heaviside(t);
E1= laplace(e1, t, p);
S1= E1*H;
s1= laplace(S1, p, t);

% e2= cos(2*pi*170*t)*heaviside(t);
% E2= laplace(e2, t, p);
% S2= E2*H;
% s2= laplace(S2, p, t);

% e3= cos(2*pi*10e4*t)*heaviside(t);
% E3= laplace(e3, t, p);
% S3= E3*H;
% s3= laplace(S3, p, t);


% Affichage
t1= linspace(0,5/20,500);
t2= linspace(0,5/170,500);
t3= linspace(0,5/10e4,500);

figure(1);
set(gcf, 'Name', 's1(t)')
s1= subs(s1, t1);
plot(t1, s1, 'r', e1, t1, 'b');

% figure(2);
% s2= subs(s2, t2);
% set(gcf, 'Name', 's2(t)')
% plot(t2, s2, 'r', e2, t2, 'b');

% figure(2);
% s3= subs(s3, t3);
% set(gcf, 'Name', 's3(t)')
% plot(t3, s3, 'r', e3, t3, 'b');
