clc
close all

u = symunit;

e = 1.60217663*10^-19;
h=6.62607015*10^-34;
H=h/(2*pi)
m = 9.1*10^-31;
e0 = 8.85*10^-12;
c = 3*10^8;

a0 = (4*pi*e0/m)*(H^2/e^2) % En Angstrom
E0=-((m*e^4)/(8*h^2*e0^2)) % En Joule

for n=1:4
    n
    a = a0*n^2 % En Angstrom
    E = -E0/(n^2) % En Joule
    EE = E/e % En eV
    if EE<13.4
        hh1=vpa((h*c)/((13.4-EE)*e))*u.m;
        hh1=unitConvert(hh1,u.nm)
    end
     if EE<3.4
         hh2=vpa((h*c)/((3.4-EE)*e))*u.m;
         hh2=unitConvert(hh2,u.nm)
     end
     if EE<1.51
         hh3=vpa((h*c)/((1.51-EE)*e))*u.m;
         hh3=unitConvert(hh3,u.nm)
     end
end

