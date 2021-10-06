close all
clc
clear
set(gcf,'Visible','on')

load("TP5\reponse_indicielle.mat");

T = reponse_indicielle(:,1);
Y = reponse_indicielle(:,2);
a=heaviside(T);

D = max(Y)-Y(end);
k = 1;

y = Y-1

% On 
for i=1:1:length(Y)-1
    if(y(i)*y(1+i)<0 && y(i)<0)
        y0(k) = T(i);
        k = k+1;
    end
end
Ty = y0(2)-y0(1)

plot(T,Y)
hold on
plot(T,a,'--r')
hold on
syms m w0

% d = exp(-(m*pi)/sqrt(1-m^2))
% M = solve(d-d,m);

m = sqrt(abs(log(D)^2/(pi^2+log(D)^2)))


% t = 2*pi/(w0*sqrt(1-m^2));
% W0 = solve(t,w0)

w0 = 2*pi/Ty*sqrt(1-m^2)

for j=1:1:length(Y)
    if or(Y(j)<Y(end)*0.95,Y(j)>Y(end)*1.05 )
        t5 = T(j+1)
        end
        end

syms p m t
H = 1./(1+2*m*(p/w0)+(p.^2/w0^2));

w0 = 2*pi*1e3

m2 = [0.1 0.3 0.6 1.01 1.5 2]

h = ilaplace(H/p,p,t);
    
t = linspace(0,6.5e-3,1000);

h = subs(h);
    
plot(t,h)
hold on;
