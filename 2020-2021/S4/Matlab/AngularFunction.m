function DPA=AngularFunction(Mtheta,Mphi,l,m)
%returns the angular probability of presence DPA as a function of angles
%(theta,phi) for given values of quantic indexes l and m. Theta and phi
%should be 2D matrices (issued from meshgrid)


%% Calcul du polynôme de Legendre

% Calcul du pas ern theta pour la dérivée
dtheta=Mtheta(1,2)-Mtheta(1,1);   

% Derivées
deriv=(cos(Mtheta).^2-1).^2;
for k=1:l+m
    deriv=gradient(deriv,dtheta,dtheta);
end

% Terme factoriel
factl=1;
for i=1:l
    factl=factl*i;
end

%Polynôme de Legendre
PL=((1-cos(Mtheta).^2).^(m/2))*(-1)^m.*deriv/(2^l*factl);


%% Fonction angulaire

%Terme factoriel numérateur
fact_num=1;
for i=1:l-m,
    fact_num=fact_num*i;
end

%Terme factoriel dénominateur
fact_den=1;
for i=1:l+m,
    fact_den=fact_den*i;
end

%Fonction angulaire
Y=sqrt((2*l+1)*fact_num/(4*pi*fact_den))*PL.*cos(m*Mphi);

%Probabilité
DPA=abs(Y).^2;
