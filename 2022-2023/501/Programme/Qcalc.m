function [Z T g]=Qcalc(cir,V1p,V1m,V2p,V2m)
% Quadripole calculor (c)FM
%Calcul quadripole Z
[X,name,MNA,RHS,allnodes,netlist]=fspice(cir);
% [X,name,MNA,RHS,allnodes,netlist2]=fspice(netlist);

netlist0=netlist;


% Adding V1
netlist{end+1}=['V1$$ ' V1p ' ' V1m ' V1'];
[X,name,MNA,RHS,allnodes,netlist]=fspice(netlist);

X(end+1)=0;n0=length(X);
id1p=find(strcmp(allnodes,V1p));
id1m=find(strcmp(allnodes,V1m));
id2p=find(strcmp(allnodes,V2p));
id2m=find(strcmp(allnodes,V2m));
if strcmp(V1p,'0'),id1p=n0;end
if strcmp(V1m,'0'),id1m=n0;end
if strcmp(V2p,'0'),id2p=n0;end
if strcmp(V2m,'0'),id2m=n0;end
V1=X(id1p)-X(id1m);
V2=X(id2p)-X(id2m);
idi1=find(strcmp(name,'I(V1$$)'));
I1=-X(idi1);
Z11=simplify(V1/I1);
Z21=simplify(V2/I1);

% Adding V2
netlist=netlist0;
netlist{end+1}=['V2$$ ' V2p ' ' V2m ' V2'];
[X,name,MNA,RHS,allnodes,netlist]=fspice(netlist);

X(end+1)=0;n0=length(X);
id1p=find(strcmp(allnodes,V1p));
id1m=find(strcmp(allnodes,V1m));
id2p=find(strcmp(allnodes,V2p));
id2m=find(strcmp(allnodes,V2m));
if strcmp(V1p,'0'),id1p=n0;end
if strcmp(V1m,'0'),id1m=n0;end
if strcmp(V2p,'0'),id2p=n0;end
if strcmp(V2m,'0'),id2m=n0;end
V1=X(id1p)-X(id1m);
V2=X(id2p)-X(id2m);
idi2=find(strcmp(name,'I(V2$$)'));
I2=-X(idi2);
Z12=simplify(V1/I2);
Z22=simplify(V2/I2);

Z=[Z11 Z12;Z21 Z22];

T=[Z(1,1)/Z(2,1) det(Z)/Z(2,1);1/Z(2,1) Z(2,2)/Z(2,1)];

g=[1/Z(1,1) -Z(1,2)/Z(2,2);Z(2,1)/Z(1,1) det(Z)/Z(1,1)];




end


