%% Questions préliminaire
% La loi de Malus défini que I = I0*cos(θ)^2 
% I0 est l'intensité insidente et I l'intensité sortante
% θ l'angle crée par la polarisation.
  
% On positionne la lentille à 50mm de la photodiode pour avoir le maximum de puissance : à cette distance, la lentille focalisera la lumière au même endroit.

%% Diffraction et interférences
% I(x) = I0*sinc(x*(pi*a)/(D*lambda))^2 -> Sa forme est celle d'un sinus cardinal
% D la distance entre écran et fente
% a la taille de la fente
% I0 l'intensité insidente et I l'intensité sortante

a = linspace(0,1e-6,10);
x = linspace(
