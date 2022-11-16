%% Calcul de C1 et C2 en fonction de m, w0 et R
syms m w0 R C1 C2
S=solve(m==sqrt(C2/C1),w0==1/R/sqrt(C1*C2),C1,C2);
fprintf('C1=%s\n',char(S.C1))
fprintf('C2=%s\n',char(S.C2))
