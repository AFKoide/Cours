clear all;
close all;

IDEL=[-50 -20 -10 0 5 10 15 20 25 30 35 40 45 50]
V_trans=[0 0 0 74e-3 -212e-3 -625e-3 -1.02 -1.42 -1.80 -2.21 -2.60 -3 -3.40 -3.87];
plot(IDEL,-V_trans);
title('Evolution de la tension de sortie transimpedance en fonction du courant d alimentaion de la DEL');
xlabel('Courant DEL (mA)');
ylabel('Tension transimpedance (V)');
grid;

load 4b.dat
load 4ba.dat
load 4c.dat

plot(X4b(:,1),X4b(:,2),X4ba(:,1),X4ba(:,2),'r');
title('FFT du signal lumineux pour F=1000Hz');
xlabel('Fréquence (Hz)');
ylabel('Décibel (dB)');
legend('Sans Atténuateur','Atténuateur oc=0.6')
grid;

plot(X4c(:,1),X4c(:,2));
title('FFT du signal lumineux pour F=1000Hz');
xlabel('Fréquence (Hz)');
ylabel('Décibel (dB)');
legend('Atténuateur oc=5')
grid;