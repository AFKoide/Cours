%% Initialisation
clc

%% Variable
R1 = 3000;
R2 = 5000;
R3 = 10000;
R4 = 15000;

syms R

E = 20;

%% Calculs
A = [ R1+R2 -R1 -R2 ; R1 -(R1+R+R3) R3 ; R2 R3 -(R2-R3-R4)];
B = [E ; 0 ; 0];

X = inv(A)*B;
I1 = X(1);
I2 = X(2);
I3 = X(3);

% I = I1*R1-I2*(R1+R+R3)+I3*R3 == 0;
I = I3-I2;

S = solve(I,R);
%% Affichage

fprintf("I1=%d I2=%d I3=%d\n",I1,I2,I3);
fprintf("La résistance R est égale à %d",R);