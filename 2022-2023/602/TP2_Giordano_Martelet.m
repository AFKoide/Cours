clear all;
close all;

angle=[0 20 40 50 60 80 90 100 120 130 140 160 180];
V=[17.2 16.4 12 9.3 5.6 1.4 0 1.9 6.6 10.3 12.8 16.8 18]*1e-3;
% V=[0.1 1.9 6.6 10.3 12.8 16.8 17.2 16.4 12 9.3 5.6 1.4 0]*1e-3;%%Photovoltage (mV)
%%% Ce sont les valeurs mesuré rellement mais afin de comparer la theorie et
%%% la pratique nous avons déphasé de 90°)
I0=18e-3;
I = I0*cosd(angle).^2;

plot(angle,V,angle,I);
title('Evolution de l intensite lumineuse en foncton de la polarisation');
xlabel('Angle du polariseur (degres)');
ylabel('Tension(Volt)');
legend('Pratique','Theorique');
grid;