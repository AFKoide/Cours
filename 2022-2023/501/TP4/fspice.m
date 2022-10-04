function [e,nom_inconnues,MNA,RHS,allnodes,netlist]=fspice(netlistfile)
disp('** fspice 2.2  ** (c) Frederic Martinez')
%% FredSpice 
% (c) Frederic Martinez
%% Historique révisions
% V2.2
% 19/07/2013 Correction bug noeud 10 20 30 etc ... 
% V2.1
% Detection de 2 espaces consécutifs dans netlist ==> interruption + message
%d'erreur.
%arg de sortie supp : cell netlist
% ligne commencée par ! = une ligne en commentaire
%
% V2
% possibilité d'utiliser des labels de noeuds alphanumeriques
% Modif et amélioration de la présentation des résultats
% 
%
%
%% Composants & analyses implantés
% R,C,L,I,V,G,E 
% X : AOP idéal  / Xlabel in+ in- out
% D : Soustracteur e+ e- S
% B : Bloc Laplace in out F(s)

% Analyse symbolique de circuits/systèmes linéaires

%% Definitions des variables symboliques

syms s


%% Analyse du fichier, creation cell netlist
if ischar(netlistfile)
fid = fopen(netlistfile, 'r');
netlist={};
nligne=1;rem=0;
c=fread(fid,1);if not(isempty(c));netlist{1}='';end;
while not(isempty(c));
     
    if (c==33)&((isempty(netlist{nligne}=='')))&(rem==0)
        rem=1;
    end
    
    if (c~=13)&(c~=10)&(rem==0)
        netlist{nligne}=[netlist{nligne} char(c)];
    end
    if (c==13)
        nligne=nligne+1;netlist{nligne}='';rem=0;
   end
   
    
    
    c=fread(fid,1);
end
fclose(fid);
elseif iscell(netlistfile)
    netlist=netlistfile
else
    disp('pb argument')
    return

end


%% Lecture de la netlist et formulation MNA
% 0 est le noeud de reférence
% nl nombre de lignes de la netlist
nl=length(netlist);
k=1;
for i=1:nl
    if not(isempty(netlist{i}));netlist_tmp{k}=netlist{i};k=k+1;end
end
netlist=netlist_tmp;nl=length(netlist);
incsup=0;is=0;
allnodes={};
for inl=1:nl
    ch=netlist{inl};
    device=ch(1); 
   
    switch device
        case 'R'
            spacepositions=findstr(' ',ch);
            if not(isempty(findstr('  ',ch)));
                disp('Erreur Syntaxe 2 espaces consécutifs dans netlist')
                e=[];nom_inconnues=[];MNA=[];RHS=[];allnodes=[];
                return;
            end;
            id=ch(2:spacepositions(1)-1);
            np=ch(spacepositions(1)+1:spacepositions(2)-1);allnodes{length(allnodes)+1}=np;
            nm=ch(spacepositions(2)+1:spacepositions(3)-1);allnodes{length(allnodes)+1}=nm;
            value=ch(spacepositions(3)+1:length(ch));
            if isnan(str2double(value));val=sym(value);else val=str2double(value);end
            elements{inl}={'R',id,np,nm,val};

        case 'C'
            spacepositions=findstr(' ',ch);
            if not(isempty(findstr('  ',ch)));
                disp('Erreur Syntaxe 2 espaces consécutifs dans netlist')
                e=[];nom_inconnues=[];MNA=[];RHS=[];allnodes=[];
                return;
            end;
            id=ch(2:spacepositions(1)-1);
            np=ch(spacepositions(1)+1:spacepositions(2)-1);allnodes{length(allnodes)+1}=np;
            nm=ch(spacepositions(2)+1:spacepositions(3)-1);allnodes{length(allnodes)+1}=nm;
            value=ch(spacepositions(3)+1:length(ch));
            if isnan(str2double(value));val=1/s/sym(value);else val=1/s/str2double(value);end
            elements{inl}={'R',id,np,nm,val};
            
             case 'L'
            spacepositions=findstr(' ',ch);
            if not(isempty(findstr('  ',ch)));
                disp('Erreur Syntaxe 2 espaces consécutifs dans netlist')
                e=[];nom_inconnues=[];MNA=[];RHS=[];allnodes=[];
                return;
            end;
            id=ch(2:spacepositions(1)-1);
            np=ch(spacepositions(1)+1:spacepositions(2)-1);allnodes{length(allnodes)+1}=np;
            nm=ch(spacepositions(2)+1:spacepositions(3)-1);allnodes{length(allnodes)+1}=nm;
            value=ch(spacepositions(3)+1:length(ch));
            if isnan(str2double(value));val=s*sym(value);else val=s*str2double(value);end
            elements{inl}={'R',id,np,nm,val};
    
    case 'V'
        incsup=incsup+1;    
        spacepositions=findstr(' ',ch);
            id=ch(2:spacepositions(1)-1);
            np=ch(spacepositions(1)+1:spacepositions(2)-1);allnodes{length(allnodes)+1}=np;
            nm=ch(spacepositions(2)+1:spacepositions(3)-1);allnodes{length(allnodes)+1}=nm;
            value=ch(spacepositions(3)+1:length(ch));is=is+1;
            if isnan(str2double(value));val=sym(value);else val=str2double(value);end
            elements{inl}={'V',id,np,nm,val,is};
    
     case 'I'
          
        spacepositions=findstr(' ',ch);
            id=ch(2:spacepositions(1)-1);
            np=ch(spacepositions(1)+1:spacepositions(2)-1);allnodes{length(allnodes)+1}=np;
            nm=ch(spacepositions(2)+1:spacepositions(3)-1);allnodes{length(allnodes)+1}=nm;
            value=ch(spacepositions(3)+1:length(ch));
            if isnan(str2double(value));val=sym(value);else val=str2double(value);end
    elements{inl}={'I',id,np,nm,val};
    
        case 'G'
            spacepositions=findstr(' ',ch);
            if not(isempty(findstr('  ',ch)));
                disp('Erreur Syntaxe 2 espaces consécutifs dans netlist')
                e=[];nom_inconnues=[];MNA=[];RHS=[];allnodes=[];
                return;
            end;
            id=ch(2:spacepositions(1)-1);
            np=ch(spacepositions(1)+1:spacepositions(2)-1);allnodes{length(allnodes)+1}=np;
            nm=ch(spacepositions(2)+1:spacepositions(3)-1);allnodes{length(allnodes)+1}=nm;
            ncp=ch(spacepositions(3)+1:spacepositions(4)-1);allnodes{length(allnodes)+1}=ncp;
            ncm=ch(spacepositions(4)+1:spacepositions(5)-1);allnodes{length(allnodes)+1}=ncm;
            value=ch(spacepositions(5)+1:length(ch));
            if isnan(str2double(value));val=sym(value);else val=str2double(value);end
            elements{inl}={'G',id,np,nm,ncp,ncm,val};
    
    case 'E'
        incsup=incsup+1;        
        spacepositions=findstr(' ',ch);
        if not(isempty(findstr('  ',ch)));
                disp('Erreur Syntaxe 2 espaces consécutifs dans netlist')
                e=[];nom_inconnues=[];MNA=[];RHS=[];allnodes=[];
                return;
            end;
            id=ch(2:spacepositions(1)-1);
            np=ch(spacepositions(1)+1:spacepositions(2)-1);allnodes{length(allnodes)+1}=np;
            nm=ch(spacepositions(2)+1:spacepositions(3)-1);allnodes{length(allnodes)+1}=nm;
            ncp=ch(spacepositions(3)+1:spacepositions(4)-1);allnodes{length(allnodes)+1}=ncp;
            ncm=ch(spacepositions(4)+1:spacepositions(5)-1);allnodes{length(allnodes)+1}=ncm;
            value=ch(spacepositions(5)+1:length(ch));is=is+1;
            if isnan(str2double(value));
                %syms(value);
                val=sym(value);
            else val=str2double(value);
            end
            elements{inl}={'E',id,np,nm,ncp,ncm,val,is};
     
                case 'D'
        incsup=incsup+1;        
        spacepositions=findstr(' ',ch);
        if not(isempty(findstr('  ',ch)));
                disp('Erreur Syntaxe 2 espaces consécutifs dans netlist')
                e=[];nom_inconnues=[];MNA=[];RHS=[];allnodes=[];
                return;
            end;
            id=ch(2:spacepositions(1)-1);
            np=ch(spacepositions(1)+1:spacepositions(2)-1);allnodes{length(allnodes)+1}=np;
            nm=ch(spacepositions(2)+1:spacepositions(3)-1);allnodes{length(allnodes)+1}=nm;
            ncp=ch(spacepositions(3)+1:length(ch));allnodes{length(allnodes)+1}=ncp;
            
           is=is+1;
           
            elements{inl}={'E',id,ncp,'0',np,nm,sym(1),is};
            
            
             case 'B'
        incsup=incsup+1;        
        spacepositions=findstr(' ',ch);
        if not(isempty(findstr('  ',ch)));
                disp('Erreur Syntaxe 2 espaces consécutifs dans netlist')
                e=[];nom_inconnues=[];MNA=[];RHS=[];allnodes=[];
                return;
            end;
            id=ch(2:spacepositions(1)-1);
            nin=ch(spacepositions(1)+1:spacepositions(2)-1);allnodes{length(allnodes)+1}=nin;
            nout=ch(spacepositions(2)+1:spacepositions(3)-1);allnodes{length(allnodes)+1}=nout;
            
            value=ch(spacepositions(3)+1:length(ch));is=is+1;
            if isnan(str2double(value));
                %syms(value);
                val=sym(value);
            else val=str2double(value);
            end
            elements{inl}={'E',id,nout,'0',nin,'0',val,is};
    
        case 'X'
        incsup=incsup+1;        
        spacepositions=findstr(' ',ch);
        if not(isempty(findstr('  ',ch)));
                disp('Erreur Syntaxe 2 espaces consécutifs dans netlist')
                e=[];nom_inconnues=[];MNA=[];RHS=[];allnodes=[];
                return;
            end;
            id=ch(2:spacepositions(1)-1);
            np=ch(spacepositions(1)+1:spacepositions(2)-1);allnodes{length(allnodes)+1}=np;
            nm=ch(spacepositions(2)+1:spacepositions(3)-1);allnodes{length(allnodes)+1}=nm;
            ns=ch(spacepositions(3)+1:length(ch));allnodes{length(allnodes)+1}=ns;
            
            is=is+1;
%             if isnan(str2double(value));syms(value);val=sym(value);else val=str2double(value);end
            elements{inl}={'X',id,np,nm,ns,is};
    
    end
end
% allnodes contient la liste des labels de noeuds rencontrés lors de
% l'analyse de netlist; contient forcement des doublons !!


nodenumber=zeros(1,length(allnodes));
k=1;
%% Construction de nodenumber
% nodenumber contient les numéros (numérique) des noeuds associés au labels
% (doublons également)
% 
% for i=1:length(allnodes)
% 
%     if max(allnodes{i}~='0') ;
%      allnodes_num=str2double(allnodes);
%      id=find(allnodes_num==allnodes_num(i));
%      if min(id)>=i;nodenumber(id)=k;k=k+1;end
%     end
% end

% Réduction de allnodes, élimination des doublons 
allnodes=union(allnodes,allnodes);
if allnodes{1}~='0';disp('Pas de noeud 0 défini !!');pause;
else
% enleve le noeud zero
    allnodes=allnodes(:,2:length(allnodes));
end
for inl=1:length(elements)
    if (elements{inl}{1}=='R')|(elements{inl}{1}=='V')...
            |(elements{inl}{1}=='C')|(elements{inl}{1}=='L')|...
            (elements{inl}{1}=='I')
    elements{inl}(2,:)=elements{inl};
    for i=1:length(allnodes)
        node=allnodes{i};
        items=strfind(elements{inl}(1,1:4),node);
        for k=1:length(items);
            if isempty(items{k})==0;elements{inl}{2,k}=i;end
        end
    end
    node='0';
        items=strfind(elements{inl}(1,1:4),node);
        for k=1:length(items);
            if (isempty(items{k})==0)&&(length(elements{inl}{1,k})==1);
                elements{inl}{2,k}=0;
            end
        end
    end
    
    if (elements{inl}{1}=='G')|(elements{inl}{1}=='E')
    elements{inl}(2,:)=elements{inl};
    for i=1:length(allnodes)
        node=allnodes{i};
        items=strfind(elements{inl}(1,1:6),node);
        for k=1:length(items);
            if isempty(items{k})==0;elements{inl}{2,k}=i;end
        end
    end
    node='0';
        items=strfind(elements{inl}(1,1:6),node);
        for k=1:length(items);
            if (isempty(items{k})==0)&&(length(elements{inl}{1,k})==1);elements{inl}{2,k}=0;end
        end
    end
    
  if (elements{inl}{1}=='X')
    elements{inl}(2,:)=elements{inl};
    for i=1:length(allnodes)
        node=allnodes{i};
        items=strfind(elements{inl}(1,1:5),node);
        for k=1:length(items);
            if isempty(items{k})==0;elements{inl}{2,k}=i;end
        end
    end
    node='0';
        items=strfind(elements{inl}(1,1:5),node);
        for k=1:length(items);
            if (isempty(items{k})==0)&&(length(elements{inl}{1,k})==1);elements{inl}{2,k}=0;end
        end
    end  
end
    

%% Remplissage MNA
N=length(allnodes)+incsup; 
MNA=sym(zeros(N,N));RHS=sym(zeros(N,1));
inc_courant_name={};
for inl=1:length(elements)
    MNAt=sym(zeros(N,N));RHSt=sym(zeros(N,1));
    if elements{inl}{1}=='R'
        np=(elements{inl}{2,3});nm=(elements{inl}{2,4});
        if (np~=0)*(nm~=0)
        MNAt(np,np)=1/elements{inl}{1,5};
        MNAt(nm,nm)=1/elements{inl}{1,5};
        MNAt(nm,np)=-1/elements{inl}{1,5};
        MNAt(np,nm)=-1/elements{inl}{1,5};
        end
        if (np==0)*(nm~=0)
            MNAt(nm,nm)=1/elements{inl}{1,5};
        end
        if (nm==0)*(np~=0)
            MNAt(np,np)=1/elements{inl}{1,5};
        end
            
            
    end
    
    if elements{inl}{1}=='C'
 np=(elements{inl}{2,3});nm=(elements{inl}{2,4});
        if (np~=0)*(nm~=0)
        MNAt(np,np)=1/elements{inl}{1,5};
        MNAt(nm,nm)=1/elements{inl}{1,5};
        MNAt(nm,np)=-1/elements{inl}{1,5};
        MNAt(np,nm)=-1/elements{inl}{1,5};
        end
        if (np==0)*(nm~=0)
            MNAt(nm,nm)=1/elements{inl}{1,5};
        end
        if (nm==0)*(np~=0)
            MNAt(np,np)=1/elements{inl}{1,5};
        end
            
            
    end

     if elements{inl}{1}=='L'
         np=(elements{inl}{2,3});nm=(elements{inl}{2,4});
        if (np~=0)*(nm~=0)
        MNAt(np,np)=1/elements{inl}{1,5};
        MNAt(nm,nm)=1/elements{inl}{1,5};
        MNAt(nm,np)=-1/elements{inl}{1,5};
        MNAt(np,nm)=-1/elements{inl}{1,5};
        end
        if (np==0)*(nm~=0)
            MNAt(nm,nm)=1/elements{inl}{1,5};
        end
        if (nm==0)*(np~=0)
            MNAt(np,np)=1/elements{inl}{1,5};
        end
            
            
    end
    %% 
    if elements{inl}{1}=='G'
        np=(elements{inl}{2,3});
        nm=(elements{inl}{2,4});
        ncp=(elements{inl}{2,5});
        ncm=(elements{inl}{2,6});
        
        if (np~=0)*(ncp~=0)
            MNAt(np,ncp)=MNAt(np,ncp)+elements{inl}{1,7};
        end

        if (np~=0)*(ncm~=0)
        MNAt(np,ncm)=MNAt(np,ncm)-elements{inl}{1,7};
        end
        
        if (nm~=0)*(ncp~=0)
        MNAt(nm,ncp)=MNAt(nm,ncp)-elements{inl}{1,7};
        end

        if (nm~=0)*(ncm~=0)
        MNAt(nm,ncm)=MNAt(nm,ncm)+elements{inl}{1,7};
        end
        
            
    end
%%
    if elements{inl}{1}=='E'

        np=(elements{inl}{2,3});
        nm=(elements{inl}{2,4});
        ncp=(elements{inl}{2,5});
        ncm=(elements{inl}{2,6});
        is=length(allnodes)+elements{inl}{1,8};
        inc_courant_name{length(inc_courant_name)+1}=['E' elements{inl}{1,2}];
        if (np~=0)
            MNAt(np,is)=MNAt(np,is)+1;
            MNAt(is,np)=MNAt(is,np)+1;
            
        end
        if (nm~=0)
            MNAt(nm,is)=MNAt(nm,is)-1;
            MNAt(is,nm)=MNAt(is,nm)-1;
        end

        if (ncp~=0)
        MNAt(is,ncp)=MNAt(is,ncp)-elements{inl}{1,7};
        end
        if (ncm~=0)
        MNAt(is,ncm)=MNAt(is,ncm)+elements{inl}{1,7};
        end
        
      
            
    end
    
    if elements{inl}{1}=='X'

        np=(elements{inl}{2,3});
        nm=(elements{inl}{2,4});
        ns=(elements{inl}{2,5});
        
        is=length(allnodes)+elements{inl}{1,6};
        inc_courant_name{length(inc_courant_name)+1}=['X' elements{inl}{1,2}];
        if (np~=0)
          
            MNAt(is,np)=MNAt(is,np)+1;
            
        end
        if (nm~=0)
           
            MNAt(is,nm)=MNAt(is,nm)-1;
        end

        if (ns~=0)
        MNAt(ns,is)=MNAt(ns,is)+1;
        end
        
        
      
            
    end
    
    if elements{inl}{1}=='V'
        np=(elements{inl}{2,3});nm=(elements{inl}{2,4});
        is=length(allnodes)+elements{inl}{1,6};
        inc_courant_name{length(inc_courant_name)+1}=['V' elements{inl}{1,2}]; 
        if (np~=0)*(nm~=0)
        MNAt(is,np)=1;
        MNAt(is,nm)=-1;
        MNAt(np,is)=1;
        MNAt(nm,is)=-1;
        end
        if (np==0)*(nm~=0)
        MNAt(is,nm)=-1;
        MNAt(nm,is)=-1;
        end
        if (nm==0)*(np~=0)
        MNAt(is,np)=1;
        MNAt(np,is)=1;
        end
        RHSt(is,1)=elements{inl}{1,5};    
            
    end
    if elements{inl}{1}=='I'
   np=(elements{inl}{2,3});nm=(elements{inl}{2,4});
        if (np~=0)*(nm~=0)
        RHSt(np,1)=-elements{inl}{1,5};
        RHSt(nm,1)=elements{inl}{1,5};
        end
        if (np==0)*(nm~=0)
         RHSt(nm,1)=elements{inl}{1,5};
        end
        if (nm==0)*(np~=0)
        RHSt(np,1)=-elements{inl}{1,5};
        end
       
            
    end
MNA=MNA+MNAt;
RHS=RHS+RHSt;
end
e=MNA\RHS;

%% classement des noms des noeuds


for i=1:length(allnodes)
nom_inconnues{i}=['V(' allnodes{i} ')'];
end

if incsup~=0
for i=length(allnodes)+1:N;
    nom_inconnues{i}=['I(' inc_courant_name{i-length(allnodes)} ')'];
end
end

        