netlist={
    'V1 v1 0 @(t) (1+0.6*sin(2*pi*1e3*t))*sin(2*pi*87e3*t)'
    'V2 v2 0 @(t) sin(2*pi*87e3*t)'
%     'R1 1 0 1000'
    'B1 vm 0 v1 v2 AA'
    'R2 vm 0 1'
%     'D1 3 4 1N4001'
%     'R3 4 0 1000'
%     'C4 4 0 15.9e-9'
%     'R1 4 5 1000'
%      'C1 5 0 1.5e-9'
% 
%     '.model 1N4001 D (IS1=14.11e-9 n1=1.984)'
    '.fun AA{@(vp,vm) fun_mult(vp,vm)}'
    '.tran tstop=1.5e-4 dtmax=.5e-6 initTRAN=CI'
    };
sim=uacs(netlist)