clear all
close all
set(gcf,'Visible','on')
syms m p w0

f = logspace(1,5,1000);
w0 = 2*pi*1e3;
m = [0.05:0.05:1/sqrt(2)]';

p = 1i*2*pi.*f;

H = 1./(1+2*m*(p/w0)+(p.^2/w0^2))

QdB = 0*m  % Q en décibel
fr = 0*m  % Fréquence de résonnance
for k=1:1:length(m)
   H = subs(H);
   [M,I] = max(20 .* log10(abs(H(k,:))));
   QdB(k) = M;
   fr(k) = f(I); % On a la fréquence fr à I de la fréquence
end
z = ((2*pi*fr)/w0).^2
figure
plot(m.^2,z,'.');
% fr = (w0/2pi)sqrt(-2m²)
p = polyfit(m.^2,z,1)
plot(p);

Q = 10.^(QdB/20)
plot(1./(1-m.^2),(2*Q.*m).^2,'.r');
hold on
q = polyfit(1./(1-m.^2),(2*Q.*m).^2,1);
plot(q);
% 2mQ² = 1*(1/1-m²)+0;
% 
% Passage en Db : 20logQ -> QdB
