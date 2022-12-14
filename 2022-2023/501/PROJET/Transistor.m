clear all;close all;clc

%%%% Mode Commun
Vb = 0:1:10;
R1=33e3 ; R2=22e3 ; R3=R1;

Ve = 0.6+Vb;
Ie1 = ((12-Ve)/R1)/2;
Ie2 = Ie1;
Vc1 = (R2.*Ie1)-12;Vc2 = (R3.*Ie2)-12;

% Vc1 = (R1.*Ie1)-12 => (R1.*(((12-Ve)/R11)/2))-12 <=> (R1.*(((12-0.6+Vb)/R11)/2))-12

figure
plot(Vb,Vc1,'k',Vb,Vc2,'r')
title('Evolution Tension Collecteur')
legend('V_c_1','V_c_2')


%%%% Mode Différentiel
Vin=linspace(-50e-3,50e-3,100)
Vb1=Vin;Vb2=-Vin;

Ve2 = 0.6+Vb2;
Ve1 = 0.6+Vb1;
Ie1 = ((12-Ve1)/R1)./(1+exp(0.025.*Vb1));
Ie2 = ((12-Ve2)/R1)./(1+exp(0.025.*Vb2));

figure
plot(Vin,Vc1,'k',Vin,Vc2,'r')
legend('V_c_1','V_c_2','location','best')
title('Evolution Tension Collecteur')
