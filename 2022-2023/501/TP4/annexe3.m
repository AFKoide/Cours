%% Diagramme de Bode avec valeurs théoriques, normalisées et prise en compte tolérance
f=logspace(3,5,1000);s=2*pi*1i*f;
% theo
R=1e3;
C1=63.6e-9;
C2=3.97e-9;
 
h_theo=1./(C1*C2*R^2*s.^2 + 2*C2*R*s + 1);
% normalisé
R=1e3;
C1=47e-9;
C2=3.3e-9;
 
h_norm=1./(C1*C2*R^2*s.^2 + 2*C2*R*s + 1);
 
%tol
%  nb: les 2 résistances R1 et R2 doivent être dissociées pour ce calcul
% Calcul de la fonction de transfert avec R1=R2=R
netlist=...
    {'R1 Vin 1 R1';
    'R2 1 e+ R2';
    'C1 1 Vout C1';
    'C2 e+ 0 C2';
    'X1 e+ Vout Vout'
    'V1 Vin 0 Ve'};
[X,name]=fspice(netlist);
H=X(3)/X(2);
 
% H= 1/(C2*R1*s + C2*R2*s + C1*C2*R1*R2*s^2 + 1)
 
 
R10=1e3;tol_R1=5/100;
R20=1e3;tol_R2=5/100;
C10=47e-9;tol_C1=20/100;
C20=3.3e-9;tol_C2=20/100;
 
n=10000; % nb de tirages
R1=R10*(1+tol_R1*(2*rand(n,1)-1));
R2=R20*(1+tol_R2*(2*rand(n,1)-1));
C1=C10*(1+tol_C1*(2*rand(n,1)-1));
C2=C20*(1+tol_C2*(2*rand(n,1)-1));
 
h_tol=zeros(n,length(f));
for i=1:n
    h_tol(i,:)=1./(C2(i)*R1(i)*s + C2(i)*R2(i)*s + C1(i)*C2(i)*R1(i)*R2(i)*s.^2 + 1);
end
h_tol_max=max(abs(h_tol));
h_tol_min=min(abs(h_tol));
semilogx(f,20*log10(abs(h_theo)),'r--',f,20*log10(abs(h_norm)),'b-','linewidth',3)
hold on
semilogx(f,20*log10(abs(h_tol_max)),'b',f,20*log10(abs(h_tol_min)),'g')
xlabel('Frequence (Hz)')
ylabel('Gain (dB)')
legend('Théorique','Normalisé','Tolérance max','Tolérance min');
