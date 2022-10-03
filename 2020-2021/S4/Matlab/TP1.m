clear all
close all
clc

%% Étude numérique de la
%% Barrière de potentiel


%% QUESTION 1

%% Constantes
m=9.1e-31;  %Masse de l'électron
eV=1.6e-19; %1 eV = 1.6e-19 Joules
hbar=(6.62e-34)/(2*pi);
V0=2*eV;       %Potentiel de la barri?re de 1 eV
a=2e-10;    %?paisseur de la barri?re de 1 ?

%Amplitude de l'onde incidente
A1=1;

%Espace
x=linspace(-5*a,5*a,10000);
dx=x(2)-x(1);
i12=min(find(x>=0));
i23=min(find(x>=a));


%% Représentation de la marche de potentiel
V=x*0;
%V(1:i12-1)=V(1:i12-1)*0;
V(i12:i23-1)=V(i12:i23-1)*0+V0;
%V(i23:end)=V(i23:end)*0;

figure
plot(x*1e10,V/eV,'linewidth',2);
grid on
xlabel('Position, A')
ylabel('Potential, eV')



%% Énergie de l'électron incident (éviter cas particulier E=V0 !!)
E=input('Electron energie (eV) [1 eV]: \n');
if isempty(E)
    E = 1*eV;
else
    E=E*eV;
end
%Eviter cas particulier E=V0 !!
if E==V0
    E=V0+V0/1000;
end

hold on
plot(x*1e10,V*0+E/eV,'r--');
legend('Barrier potential','Incoming Energy')


%% QUESTION 2


% Eq Diff phi''=-k^2*phi
k=sqrt(2*m*(E-V)/(hbar^2));


%Amplitude de l'onde (évanescente) propagative phi_p et dérivée
phi_p=exp(i*k.*x);
deriv_p=gradient(phi_p,dx);
%Amplitude de l'onde (évanescente) contra-propagative et dérivée
phi_r=exp(-i*k.*x);
deriv_r=gradient(phi_r,dx);


%% On se rend compte que les valeurs de dérivées évoluent sur 4 points au niveau des interfaces => prendre les valeurs à +/- 2 points de l'interface
figure
subplot 211
plot(x*1e10,real(phi_p),'o-')
xlabel('Position, A')
ylabel('Propagative term, arb. units')
legend('Real part')
grid on
subplot 212
plot(x*1e10,real(deriv_p),'o-')
xlabel('Position, A')
ylabel('First-derivative of propagative term, arb. units')
legend('Real part')
grid on



%% Conditions limites
%Systeme eq. lin. B1;A2;B2;A3;
%ATTENTION, pour les dérivées se décaler d'un point de plus pour éviter "fct
%appareil" de matlab !

A1=1;

C=[phi_r(i12-1) -phi_p(i12) -phi_r(i12) 0;
    0 phi_p(i23-1) phi_r(i23-1) -phi_p(i23);
    deriv_r(i12-3) -deriv_p(i12+2) -deriv_r(i12+2) 0;
    0 deriv_p(i23-3) deriv_r(i23-3) -deriv_p(i23+2)];


D=[-phi_p(i12-3);
    0;
    -deriv_p(i12-3);
    0];


sol=linsolve(C,D);

B1=sol(1,1);
A2=sol(2,1);
B2=sol(3,1);
A3=sol(4,1);

%% Solutions 
phi1=A1*phi_p+B1*phi_r;
proba1=abs(phi1).^2;

%Fonction d'onde dans la barrière :
phi2=A2*phi_p+B2*phi_r;
proba2=abs(phi2).^2;

%Fonction d'onde après la barrière :
phi3=A3*phi_p;
proba3=abs(phi3).^2;


%% Traçés
figure
plot(x(1:i12-1)*1e10,proba1(1:i12-1))
hold on
plot(x(i12:i23-1)*1e10,proba2(i12:i23-1))
plot(x(i23:end)*1e10,proba3(i23:end))
maximumabsolu=max([max(proba1(1:i12-1)) max(proba2(i12:i23-1)) max(proba3(i23:end))]);
hold on
plot([0 0],[0 maximumabsolu],'k')
plot([a a]*1e10,[0 maximumabsolu],'k')
axis tight
grid on
xlabel('Position, A')
ylabel('Probability (w/o normalization)')


% R=abs(B1/A1)^2 
% T=abs(A3/A1)^2
% Ttheorique=4*E*(V0-E)/(4*E*(V0-E)+(V0^2)*((sinh(sqrt(2*m*(V0-E))*a/hbar))^2))
