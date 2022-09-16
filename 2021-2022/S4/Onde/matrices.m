C=[
phi_r(i0-3),       -phi_p(i0+3),      -phi_r(i0+3),      0;    
0,                  phi_p(ia-3),       phi_r(ia-3),       -phi_p(ia+3);    
deriv_r(i0-3),     -deriv_p(i0+3),    -deriv_r(i0+3),    0;   
0,                  deriv_p(ia-3),     deriv_r(ia-3),     -deriv_p(ia+3)
];

D=[ 
-phi_p(i0-3);
0;
-deriv_p(i0-3);
0
];