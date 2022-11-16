clear all
close all

syms R C w Ve
Z1=R+1/(1i*C*w);
MNA=[1/Z1 -1/Z1 1; -1/Z1 1/Z1+1/R+1i*C*w 0; 1 0 0]; 
RHS = [0;0;Ve];
X=inv(MNA)*RHS % vecteur d'inconnues
H=X(2)/X(1) % Fonction de transfert isochrone

% Diagramme de Bode
R=1e3;
C=1e-9; 
f=logspace(2,8,1000);
w=2*pi*f;
H=subs(H);
G=20*log10(abs(H));

semilogx(f,G)
xlabel('Fréquence en Hz')
ylabel('Gain en dB')
figure
semilogx(f, angle(H)*180/pi)
xlabel('Fréquence en Hz')
ylabel('Phase en °')

