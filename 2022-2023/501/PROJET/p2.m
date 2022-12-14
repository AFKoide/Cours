clc;close all;clear all;


%%%%% Partie 2
Vin=0:1:10;
%------ Montage sans miroir de courant
% Mode Commun
Vc1sc=[-7.93 -8.25 -8.57 -8.89 -9.21 -9.52 -9.84 -10.16 -10.47 -10.79 -11.11];
Vc2sc=[-7.80 -8.13 -8.46 -8.79 -9.13 -9.46 -9.79 -10.12 -10.45 -10.78 -11.11];
Voutsc=[296e-3 1.821 3.35 4.88 6.40 7.90 9.38 10.80 11.40 11.40 11.40 ];
Vesc=[544e-3 1.538 2.53 3.53 4.52 5.51 6.50 7.49 8.48 9.46 10.44];

Vc1sd = [-10.85 -10.46 -9.97 -9.4 -8.77 -8.1 -7.45 -6.84 -6.29 -5.84 -5.47];
Vc2sd = [-5.53 -5.93 -6.42 -7 -7.64 -8.31 -8.96 -9.58 -10.13 -10.59 -10.96];
Vesd =  [515.8e-3 524e-3 531e-3 538e-3 544e-3 549.7e-3 554e-3 557.7e-3 560.7e-3 563e-3 564.8e-3];
Voutsd =[11.87 11.88 11.88 11.88 11.88 11.88 11.88 11.88 11.88 11.88 11.88];

Vc1mc = [8.41  8.42  8.44  8.43  8.43  8.43  8.43  8.43  8.44  8.44  8.43];Vc1mc=-1.*Vc1mc;
Vc2mc = [8.439 8.444 8.449 8.445 8.449 8.444 8.448 8.452 8.451 8.457 8.455];Vc2mc=-1.*Vc2mc;
Vemc =  [.498   .508   .518   .528   .538   .548   .558   .569   .578   .589   .598];
Voutmc =[-1.354 -1.345 -1.347 -1.352 -1.348 -1.341 -1.332 -1.328 -1.327 -1.323 -1.32];

Vc1md = [10.99 10.62 10.14 9.58  8.92  8.27  7.61  6.98  6.43   5.95   5.58];Vc1md=-1.*Vc1md;
Vc2md = [7.08  7.1   7.1   7.127 7.877 8.412 8.991 9.605 10.145 10.610 10.978];Vc2md=-1.*Vc2md;
Vemd =  [.513 .521   .529 0.536  .542  0.548 .554 .557 .560 .563  .565];
Voutmd =[-7.72 -7.72 -7.71  -7.3 -4.15 -.91  2.22 5.43 8.23 10.54 11.76];


%Sans miroir
%Mode commun et mode différentiel
%Vc1 et Vc2
Vin = -50e-3:10e-3:50e-3;
figure
plot(Vin,Vc1sc,'k',Vin,Vc2sc,'r',Vin,Vc1sd,'b',Vin,Vc2sd,'g')
legend('V_c_1_s_c','V_c_2_s_c','V_c_1_s_d','V_c_2_s_d','location','best')
ylabel('Tension')
title({'Sans Miroir';'Vc1 et Vc2'})
ylim([-12 -5])
grid on


%Sans miroir
%Mode commun et mode différentiel
%Ve et Vout
Vin = -50e-3:10e-3:50e-3;
figure
plot(Vin,Vesc,'k',Vin,Voutsc,'r',Vin,Vesd,'b',Vin,Voutsd,'g')
legend('V_e_s_c','V_o_u_t_s_c','V_e_s_d','V_o_u_t_s_d','location','best')
xlabel('Tension V_i_n')
ylabel('Tension')
title({'Sans Miroir';'Ve et Vout'})
ylim([-5 15])
grid on

%Avec miroir
%Mode commun et mode différentiel
%Vc1 et Vc2
Vin = -50e-3:10e-3:50e-3;
figure
plot(Vin,Vc1mc,'k',Vin,Vc2mc,'r',Vin,Vc1md,'b',Vin,Vc2md,'g')
legend('V_c_1_m_c','V_c_2_m_c','V_c_1_m_d','V_c_2_m_d','location','best')
ylabel('Tension')
title({'Avec Miroir';'Vc1 et Vc2'})
ylim([-12 -5])
grid on

%Avec miroir
%Mode commun et mode différentiel
%Ve et Vout
Vin = -50e-3:10e-3:50e-3;
figure
plot(Vin,Vemc,'k',Vin,Voutmc,'r',Vin,Vemd,'b',Vin,Voutmd,'g')
legend('V_e_m_c','V_o_u_t_m_c','V_e_m_d','V_o_u_t_m_d','location','best')
xlabel('Tension V_i_n')
ylabel('Tension')
title({'Avec Miroir';'Ve et Vout'})
ylim([-12 15])
grid on


