clear all;close all;clc

Vin = -10:1:10;
R11=33e3 ; R1=22e3 ; R2=R1;

Ve = 0.6+Vin;
Ie1 = ((12-Ve)/R11)/2;
Ie2 = Ie1;
Ib1=Ie1 ; Ib2=Ie2;

figure
plot(Vin,Ib1)
xlabel('V_i_n')
ylabel('I_b_1')
xline(0,'--')
title('Evolution Courant Collecteur')
subtitle('Mode Continue')


Vin1=0:1:10 ; Vin2=-Vin1;

Ve2 = 0.6+Vin2;
Ve1 = 0.6+Vin1;
Ie1 = ((12-Ve1)/R11)/2;
Ie2 = ((12-Ve2)/R11)/2;
Ib1=Ie1 ; Ib2=Ie2;

figure
plot(Vin1,Ib1)
hold on
plot(Vin2,Ib2)
xlabel('V_i_n')
ylabel('Courant')
legend('I_b_1','I_b_2','location','best')
xline(0,'--')
title('Evolution Courant Collecteur')
subtitle('Mode Différentiel')