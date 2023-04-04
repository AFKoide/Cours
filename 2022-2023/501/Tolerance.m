clc

n = 10000;                          % Nombre de composants (+ il y en aura, plus le script sera long).
f=logspace(3,5,1000);               % Fréquence a choisir (je conseil une petite valeur).
s=2*pi*1i*f;                        % On déclare s
R1=1e3;R2=R1;C1=63.6e-9;C2=3.97e-9; % Définition des composants

% TOLERANCE
R1_reel = R1*(1+5/100*(2*rand(n,1)-1)); % Resistance*(1+% d'erreur de la résistance*(2*rand(n,1)-1));
R2_reel = R2*(1+5/100*(2*rand(n,1)-1));
C1_reel = C1*(1+10/100*(2*rand(n,1)-1)); % Capacité*(1+% d'erreur de la capacité*(2*rand(n,1)-1));
C2_reel = C2*(1+10/100*(2*rand(n,1)-1));

% CIRCUIT
netlist={'R1 Vin 1 R1';
    'R2 1 e+ R2';
    'C1 1 Vout C1';
    'C2 e+ 0 C2';
    'X1 e+ Vout Vout'
    'V1 Vin 0 Ve'};
[X,name]=fspice(netlist);
H=X(3)/X(2);

% CALCUL DE BODE
H_reel=zeros(n,length(f));  % Prédéfini la matrice pour que la boucle prenne moins de temps.
for i=1:n
    H_reel(i,:)=1./(C2_reel(i)*R1_reel(i)*s + C2_reel(i)*R2_reel(i)*s + C1_reel(i)*C2_reel(i)*R1_reel(i)*R2_reel(i)*s.^2 + 1);
% On calcul le bode. Comme on prend en compte les erreurs des composants,
% on a calculé plus tôt la valeurs "réel" des composants n différentes fois.
end

H_reel_max=max(abs(H_reel)); % Tolérance maximale
H_reel_min=min(abs(H_reel)); % Tolérance minimale

% PLOT
figure
semilogx(f,20*log10(abs(H_reel_max)),'b',f,20*log10(abs(H_reel_min)),'g')
hold on
semilogx(f,20*log10(abs(subs(H))))
legend('Maximum','Minimum','Théorique')
grid on