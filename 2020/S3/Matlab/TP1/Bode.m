clc
close

f=logspace(0,6,1000);
fc = 300;
%syms f p;

p = 1i*(f/fc);

H = 1./(1+p);
%H = subs(H,p,li*f/fc);
%f=logspace(0,6,1000);
%H = subs(H);

G = 20 .* log10(abs(H));

semilogx(f,G)
figure
semilogx(f,angle(H))
