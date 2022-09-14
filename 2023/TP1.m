clc
close all


syms w C R z1 z2 Ve

MNA = [1/z1 -1/z1 1;-1/z1 (1/z1)+(1/R)+1i*C*w 0; 1 0 0];
RHS = [0 0 Ve];
MNA1 = inv(MNA);

X = RHS*MNA1;

X = subs(X, z1, R+1/(1i*C*w));
X = subs(X, z2, (1/R)+1i*C*w);


X = subs(X, R, 1e3);
X = subs(X, C, 1e-9);

e1 = X(1)
e2 = X(2)
ik = X(3)




syms p

h = e2/e1
H = laplace(h)
