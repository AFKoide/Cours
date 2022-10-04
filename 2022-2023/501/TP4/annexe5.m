R2=1e3;
C1=47e-9;
C2=3.3e-9;
R4=1e3;
R3=0;
 
w0_cdc=2*pi*1e4;
m_cdc=.25;
R1_adjusted=1/(C1*C2*R2*w0_cdc^2)

 
m_with_only_R1_adjusted=(C2*R1_adjusted*R4 - C1*R1_adjusted*R3 + C2*R2*R4)/(2*R4*(C1*C2*R1_adjusted*R2)^(1/2))
 
R3_adjusted=(C2*R2*R4 - 2*R4*m_cdc*(C1*C2*R2*R1_adjusted)^(1/2) + C2*R4*R1_adjusted)/(C1*R1_adjusted)
