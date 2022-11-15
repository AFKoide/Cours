function [fref,Aref]=fft_eea(t,a)
%FUNCTION [f,A]=FFT_EEA(t,a)
%
%a=fct(t) gives in the frequency domain A=fct(f)
%Note that A is complex, so we should plot abs(A)=fct(f) to observe the
%amplitude



%We need a power of 2 for fft process
N=2^nextpow2(length(t));

%Create a time vector centered over 0
tref=linspace(0,max(t)-min(t),N);

%Corresponding frequency vector
fref=(-N/2:N/2-1)/(abs(tref(2)-tref(1))*N);

%Interpolate a over this new vector
aref=interp1(t-min(t),a,tref,'pchip');


%FFT
Aref=fftshift(ifft(aref));
%fftshift is used to allow the user to simply draw Aref as a function of
%fref, but with an abs function (or real)

