C11=phi_r(i12-1);
C12=-phi_p(i12+1);
C13=-phi_r(i12+1);
C22=phi_p(i23-1);
C23=phi_r(i23-1);
C24=-phi_p(i23+1);
C31=deriv_r(i12-1);
C32=-deriv_p(i12+1);
C33=-deriv_r(i12+1);
C42=deriv_p(i23-1);
C43=deriv_r(i23-1);
C44=-deriv_p(i23+1);

C=[C11 C12 C13 0;
    0 C22 C23 C24;
    C31 C32 C33 0;
    0 C42 C43 C44];

D=[-phi_p(i12-1);
    0;
    -deriv_p(i12-1);
    0];

X=linsolve(C,D);


plot(x(1:i12)/1e-10,proba1(1:i12),x(i12:i23)/1e-10,proba2(i12:i23),x(i23:end)/1e-10,proba3(i23:end))   %plot(x1,y1,x2,y2,x3,y3...)

