clc
close all

ft = logspace(1.699,5,1000);s=2*pi.*ft;

%%% Circuit RC
% Théorique
R1=1e3;C1=100e-9;

f0_RC = 1/(2*pi*R1*C1)
H_RC = 1./(1+1i*R1*C1.*s);

figure
semilogx(ft,20*log10(abs(H_RC)));
xline(f0_RC,'--')
ylabel('Module (dB)')
xlabel('Fréquence (Hz)')
title('Circuit RC')
subtitle('Bode Théorique')
grid on

% Expérimental
frc   = [50   100  200  500  1000 2000 5000 10000 20000 50000 100000];
Verc  = [2.04 2.04 2.04 2.04 2.04 2.04 2.04 2.04  2.04  2.04  2.04];
Vs_RC = [2.04 2.04 2.04 1.92 1.68 1.20 0.56 0.30  0.152 0.064 0.0308];

Hr_RC = Vs_RC./Verc;

figure
semilogx(frc,20*log10(abs(Hr_RC)),'-X');
xline(f0_RC,'--')
ylabel('Module (dB)')
xlabel('Fréquence (Hz)')
title('Circuit RC')
subtitle('Bode Expérimental')
grid on

% Mesure Impulsionnelle
% load [Nom du fichier].dat RC

figure
semilogx(ft,20*log10(abs(H_RC)));
hold on
semilogx(frc,20*log10(abs(Hr_RC)),'k-X');
% semilogx(RC([5:end],1),RC([5:end],2)+Offset,'r--'); % Offset = 55
hold off
xline(f0_RC,'--')
ylabel('Module (dB)')
xlabel('Fréquence (Hz)')
title('Circuit RC')
subtitle('Comparaison des Bodes')
legend('Théorique','Expérimental','Impulsionnel')
grid on



%%% Circuit RLC
%%% Circuit RC
% Théorique
R2=60;C2=68e-9;L2=4.7e-3;

f0_RLC = 1/(2*pi*sqrt(L2*C2))
H_RLC = 1./(1-L2*C2.*s.^2+1i*R2*C2.*s);

figure
semilogx(ft,20*log10(abs(H_RLC)));
xline(f0_RLC,'--')
ylabel('Module (dB)')
xlabel('Fréquence (Hz)')
title('Circuit RLC')
subtitle('Bode Théorique')
grid on

% Expérimental
frlc   = [50   100  200  500  1000 2000 3000 4000 5000 6000 7000 8000 9000 10000 20000 50000 100000];
Verlc  = [2.04 2.08 2.08 2.08 2.08 2.08 2.04 2.00 1.96 2.08 1.88 1.40 1.24 1.44  1.98  2.00  2.08]; 
Vs_RLC = [2.04 2.08 2.04 2.04 2.08 2.16 2.32 2.48 2.76 3.24 3.76 4.24 4.08 3.44  0.544 0.076 0.022];

Hr_RLC = Vs_RLC./Verlc;

figure
semilogx(frlc,20*log10(abs(Hr_RLC)),'-X');
xline(f0_RLC,'--')
ylabel('Module (dB)')
xlabel('Fréquence (Hz)')
title('Circuit RLC')
subtitle('Bode Expérimental')
grid on

% Mesure Impulsionnelle
% load [Nom du fichier].dat RLC

figure
semilogx(ft,20*log10(abs(H_RLC)));
hold on
semilogx(frlc,20*log10(abs(Hr_RLC)),'k-X');
% semilogx(RLC([5:end],1),RLC([5:end],2)+Offset,'r--'); % Offset = 30
hold off
xline(f0_RLC,'--')
ylabel('Module (dB)')
xlabel('Fréquence (Hz)')
title('Circuit RLC')
subtitle('Comparaison des Bodes')
legend('Théorique','Expérimental','Impulsionnel')
grid on



%%% Bruit
% load [Nom du fichier].dat Bruit

figure
hold on
% semilogx(Bruit([5:end],1),Bruit([5:end],2)+Offset,'k'); % Offset = 60
% semilogx(RLC([5:end],1),RLC([5:end],2)+Offset,'r--');  % Offset = 30
hold off
ylabel('Module (dB)')
xlabel('Fréquence (Hz)')
title('Bruit')
subtitle('Diagramme de Bodes')
legend('RLC','Bruit')
grid on
