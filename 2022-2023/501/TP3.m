clc
close all

%%%------------------------------ RLC
%- Théorie -%
syms w C R L Ve

netlist = {'Vsource in 0 Ve';'R1 in tmp R';'L1 tmp out L';'C1 out 0 C'};
[X, name] = fspice(netlist);

Htrlc = X(2)/X(1);

I = ilaplace(X(4));

R1=10;R2=180;R3=330;
C=100e-9;L=4.7e-3;

Htrlc1 = subs(Htrlc,R,R1);
Htrlc2 = subs(Htrlc,R,R2);
Htrlc3 = subs(Htrlc,R,R3);

%%% Bode %%%
ft=logspace(2,5.7,500);
s=2*pi*ft*1i;


%- Pratique -%
f = [100 200 500 700 1000 2000 5000 7000 10000 20000 50000 70000];

Ve10 = [2.16 2.16 2.16 2.12 2.16 2.16 2.16 2.16 2.16 2.16 2.24 2.08]; % Il y a une résonnance à 7000Hz
Vs10 = [2.16 2.16 2.20 2.12 2.08 1.96 1.36 2.9 0.68 0.32 0.084 0.0352];
H10 = Vs10./Ve10;

Ve180 = [2.16 2.16 2.16 2.14 2.16 2.08 1.92 1.88 1.92 2.08 2.08 2.08];
Vs180 = [2.14 2.14 2.14 2.10 2.14 2.06 1.66 1.22 0.9 0.32 0.064 0.06];
H180 = Vs180./Ve180;

Ve330 = [2.12 2.12 2.12 2.12 2.1 2.08 1.96 1.92 2 2.04 2.08 2.06];
Vs330 = [2.12 2.12 2.12 2.12 1.92 1.6 1.18 0.992 0.456 0.308 0.12 0.0368];
H330 = Vs330./Ve330;

w = 2*pi.*f;

G10 = 20.*log10(abs(H10));
G180 = 20.*log10(abs(H180));
G330 = 20.*log10(abs(H330));

%- Affichage -%
i = 1;
figure(1)
set(gcf, 'Name', 'RLC : Comparaison Bode Expérimental & Théorique')

for i=1:3
    subplot(1,3,i)
    switch i
        case 1
            title(R=10)
            semilogx(w, G10,'k');
            hold on
            semilogx(ft,20*log10(abs(subs(Htrlc1))),'r')
        case 2
            title(R=180)
            semilogx(w, G180,'k');
            hold on
            semilogx(ft,20*log10(abs(subs(Htrlc2))),'r')
        case 3
            title(R=330)
            semilogx(w, G330,'k');
            hold on
            semilogx(ft,20*log10(abs(subs(Htrlc3))),'r')
            ylim([-80 1])
    end
    legend('Pratique','Théorique');
    xlabel('Fréquence (Hz)')
    ylabel('Module (dB)')
    grid on
end


%% 
%%%------------------------------ LCR
%- Théorie -%
syms w C R L Ve

ftlcr=logspace(2,5.7,500);
s=2*pi*ft*1i;

netlist = {'Vsource in 0 Ve';'L1 in tmp L';'C1 tmp out C';'R1 out 0 R'};
[X, name] = fspice(netlist);
I = ilaplace(X(4));

R=330;C=100e-9;L=4.7e-3;

Htlcr = X(2)/X(1);
Gtlcr = 20*log(abs(subs(Htlcr)));

% - Pratique - %

flcr = [100 200 500 700 1000 2000 5000 7000 10000 20000 50000];
wlcr = 2*pi.*flcr;

Velcr = [1.14 1.14 1.14 1.14 1.12 1.12 1.04 1.02 1.02 1.08 1.12];
Vslcr = [246e-3 472e-3 744e-3 840e-3 900e-3 1.02 1.06 1.04 1.06 1.02 900e-3];

I = Vslcr/330; % Ir = Vr/R avec Vr = Vs ; Vslcr est deja une amplitude. Vs/R donne le module.
% Z = Ve/I = (Ve/Vs)*R

Zlcr = (Velcr./Vslcr)*330;
Hlcr = Vslcr./Velcr;
Glcr = 20.*log10(Hlcr);

figure(2)
set(gcf, 'Name', 'Diagramme de Bode LCR')
semilogx(wlcr, Glcr,'k');
hold on
semilogx(ftlcr,Gtlcr,'r');
legend('Pratique','Théorique');
xlabel('Fréquence (Hz)')
ylabel('Module (dB)')
grid on
