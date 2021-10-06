%% Initialisation
clear all
load("mesures_capteur.mat");    % Ajoute deux variables.

%% Variables

T = [48.8 78.1 90.0 112.5 149.1 173.7 204.8 235.7 298.2 347.4];
R = [9.2 20.5 25.4 34.5 50.7 60.9 73.3 88.1 109.2 129.3];

%% Calculs 

p = polyfit(T,R,1); 
% [a b] a est la pente et b l'origine. R0 = b et R0alpha = a
RT = p(1)*T+p(2);
% RT = p(2)*(1+T*p(1)/p(2))

p2 = polyfit(T2,R2,1);
RT2 = p2(1)*T2+p2(2);


p3 = polyfit(T2,R2,2);
R03 = p3(3);
alpha3 = p3(2)/R03;
beta3 = p3(1)/R03;

RT3 = R03*(1 + alpha3*T2 + T2.^2*beta3);

% aT + b = valeur approximative
erreur_relative_lineaire = (p(1)*T2(end)+p(2)-R2(end))/R2(end) ;

% aT² + bT + c = valeur approximative 
erreur_relative_quadratique = (p3(1)*T2(end).^2+p3(2)*T2(end)+p3(3)-R2(end))/R2(end) ;

%% Affichage

figure(1)
plot(T,R,'*');
hold on
plot(T,RT,'r');


hold on
plot(T2,RT2,'g');


plot(T2,RT3,'y');

display(erreur_relative_lineaire*100);

display(erreur_relative_quadratique*100);