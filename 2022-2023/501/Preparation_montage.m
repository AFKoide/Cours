clc
close all

% Sallen-Key 1
netlist={
    'R1 in 1 R1'
    'R2 1 e+ R2'
    'C1 1 out C1'
    'C2 e+ 0 C2'
    'X1 e+ out out'
    'V1 in 0 Ve'};

[X, name]=fspice(netlist);
H = X(4)/X(3);

C1=1e-9;C2=1e-9;R1=10*1e3;R2=10*1e3;

H_sallen1 = 20*log10(abs(subs(H)));

% ft=logspace(2,6,1000);
% s=2*pi*ft*1i;
% semilogx(ft,20*log10(abs(subs(H))),'b')
% xlabel('Fréquence (Hz)')
% ylabel('Module (dB)')
% grid on


% Sallen-Key 2
netlist={
    'R1 1 out R1'
    'R2 e+ 0 R2'
    'C1 in 1 C1'
    'C2 1 e+ C2'
    'X1 e+ out out'
    'V1 in 0 Ve'};

[X, name]=fspice(netlist);
H = X(4)/X(3);

C1=1e-9;C2=1e-9;R1=10*1e3;R2=10*1e3;

H_sallen2 = 20*log10(abs(subs(H)));

% ft=logspace(2,6,1000);
% s=2*pi*ft*1i;
% semilogx(ft,20*log10(abs(subs(H))),'b')
% xlabel('Fréquence (Hz)')
% ylabel('Module (dB)')
% grid on


% Sallen-Key 3
netlist={
    'R1 in 1 R1'
    'R2 e+ 0 R2'
    'R3 1 out R3'
    'C1 1 0 C1'
    'C2 1 e+ C2'
    'X1 e+ out out'
    'V1 in 0 Ve'};

[X, name]=fspice(netlist);
H = X(4)/X(3);
R1=10*1e3;R3=R1;R2=20*1e3;
C1=10*1e-9;C2=C1;

H_sallen3 = 20*log10(abs(subs(H)));

% ft=logspace(2,6,1000);
% s=2*pi*ft*1i;
% semilogx(ft,20*log10(abs(subs(H))),'b')
% xlabel('Fréquence (Hz)')
% ylabel('Module (dB)')
% grid on


% Rauch 1
netlist={
    'R1 in 1 R1'
    'R2 1 out R2'
    'R3 1 e- R3'
    'C1 1 0 C1'
    'C2 e- out C2'
    'X1 0 e- out'
    'V1 in 0 Ve'};

[X, name]=fspice(netlist);
H = X(4)/X(3);

R1=10*1e3;R3=R1;R2=R1;
C1=10*1e-9;C2=C1;

H_rauch1 = 20*log10(abs(subs(H)));

% ft=logspace(0,6,1000);
% s=2*pi*ft*1i;
% semilogx(ft,20*log10(abs(subs(H))),'b')
% xlabel('Fréquence (Hz)')
% ylabel('Module (dB)')
% grid on


% Rauch 2
netlist={
    'R1 1 0 R1'
    'R2 e- out R2'
    'C1 in 1 C1'
    'C2 1 out C2'
    'C3 1 e- C3'
    'X1 0 e- out'
    'V1 in 0 Ve'};
[X, name]=fspice(netlist);
H = X(4)/X(3);

R1=10*1e3;R2=R1;
C1=10*1e-9;C2=C1;C3=C1;

H_rauch2 = 20*log10(abs(subs(H)));

% ft=logspace(0,6,1000);
% s=2*pi*ft*1i;
% semilogx(ft,H_rauch2,'b')
% xlabel('Fréquence (Hz)')
% ylabel('Module (dB)')
% grid on


% Rauch 3
netlist={
    'R1 e- out R1'
    'R2 1 0 R2'
    'C1 in 1 C1'
    'C2 1 e- C2'
    'C3 1 out C3'
    'X1 0 e- out'
    'V1 in 0 Ve'};
[X, name]=fspice(netlist);

H = X(4)/X(3);

R1=10*1e3;R2=R1;
C1=10*1e-9;C2=C1;C3=C2;

H_rauch3 = 20*log10(abs(subs(H)));

% ft=logspace(0,6,1000);
% s=2*pi*ft*1i;
% semilogx(ft,H_rauch3,'b')
% xlabel('Fréquence (Hz)')
% ylabel('Module (dB)')
% grid on



% PLOT
ft=logspace(0,8,1000);
s=2*pi*ft*1i;

figure
semilogx(ft,H_sallen1,'k')
hold on
semilogx(ft,H_sallen2,'b')
hold on
semilogx(ft,H_sallen3,'r')
hold on
semilogx(ft,H_rauch1,'c')
hold on
semilogx(ft,H_rauch2,'m')
hold on
semilogx(ft,H_rauch3,'g')

xlabel('Fréquence (Hz)')
ylabel('Module (dB)')
legend('Sallen-Key 1','Sallen-Key 2','Sallen-Key 3','Rauch 1','Rauch 2','Rauch 3')
grid on
set(gcf,'Visible','on')
