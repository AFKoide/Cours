clc
close all

% 1
I = 6.13e-6 % A

% 2
I = 472e-9 % A
% A la sortie du miroir M3, la lumière est maintenant uniquement rouge.

% 3
lum_blanc = load('data1.txt');

figure()
plot(lum_blanc(:,3),-lum_blanc(:,4))
title('Spectre optique de la lumière blanche')
xlabel('Longueur d onde (nm)'); ylabel('Sensibilité (A/W)')
grid minor

% 4
lambda=linspace(400,800,41);
a=(0.58-0.14)/(750-400);
b=0.39-a*600;
y=a.*lambda+b;
figure()
plot(lambda,y);
title('Evolution sensibilité en fonction de la longueur onde')
xlabel('Longueur donde (nm)'); ylabel('Sensibilité (A/W)')
grid minor

Puissance = lum_blanc(:,4)./y';

figure()
plot(lambda,Puissance)
xlabel('La longueur d onde en nm')
ylabel('Puissance optique, W')
title('Spectre en puissance optique de la lumière blanche')
grid minor


% 5
% On peut en conclure que la lumière blanche est composée majoritairement
% de 


% 6
lum_filtre = load('data2.txt');

figure()
plot(lum_filtre(:,3),-lum_filtre(:,4))
title('Spectre optique de la lumière blanche filtrée')
xlabel('Longueur d onde (nm)'); ylabel('Sensibilité (A/W)')
grid minor


% 7


% 8
Transmission_Filtre = lum_filtre(:,4)./lum_blanc(:,4);

figure()
plot(lambda,Transmission_Filtre)
title('Coefficient de transmission du filtre selon lambda')
xlabel('Longueur d onde (nm)'); ylabel('Transmission')
grid minor

% 9
% En électronique, on peut apparenter ce filtre a un coupe bande.




%%%%% PARTIE 2
% 1
% En fermant la fente, on observe que la tache lumineuse s'étale sur l'axe
% horizontal: on a une diffraction.
