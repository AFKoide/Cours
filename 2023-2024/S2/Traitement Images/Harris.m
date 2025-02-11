function guicorner
clear all;
close all;
figure(  'Name','translation',...
            'NumberTitle','off',...
            'color',[0.3137 0.3137 0.5098]);
       
a(1)=axes('units','normalized',...
    'position',[0.05 0.2 0.5 0.5]);
a(2)=axes('units','normalized',...
    'position',[0.5 0.2 0.5 0.5]);


uicontrol(  'style','pushbutton',...
            'string','load',...
            'Position', [10 10 50 20],...
            'callback',@loadimage);
       
uicontrol(  'style','pushbutton',...
            'string','executer',...
            'Position', [290 10 100 20],...
            'callback',@executer);
 

%parametre initial
setappdata(gcf,'x',1);
setappdata(gcf,'type',0);

     
function loadimage(~,~)
    % appeler quand appui check box
    [filename, pathname] = uigetfile({'*.jpg;*.tif;*.png;*.gif;*.bmp','All Image Files';...
          '*.*','All Files' },'mytitle',...
          'C:\Work\myfile.jpg')

    x = imread(filename);
    if (length(size(x))>2)
        x=x(:,:,1);% on prend une seul plan image noir et blanc chaque plan sont egaux
    end
    k = whos('x');
    if k.class == 'uint8'
        k=8;
    end
    setappdata(1,'k',k);
    setappdata(1,'x',x);
    axes(a(1))
    imshow(x)
    axes(a(2))
    imshow(x)
   
end
function executer(hObj,~,Name)

     type = getappdata(1,'type');
     img = getappdata(1,'x');
     
     [ligne colonne]=size(img);
     img=double(img);
     img2=zeros(ligne,colonne);
     
     % derivate
     for i = 1 : colonne-1
         dv(:,i) = img(:,i+1)-img(:,i);
     end
     dv=dv(1:end-1,:);
     for i = 1 : ligne-1
         dh(i,:) = img(i+1,:)-img(i,:);
     end
     dh=dh(:,1:end-1);
     %
     A = dv.^2;
     B = dh.^2;
     C = dv.* dh;
     %smooth filter
     Fgauss =  fspecial('gaussian', 3, 0.5);
     Ap = imfilter(A, Fgauss);
     Bp = imfilter(B, Fgauss);
     Cp = imfilter(C, Fgauss);
         
%      M = [Ap Cp;Cp Bp];
%      lambda1 = 1/2*( Ap + Bp + sqrt(Ap.^2 - 2.*Ap.*Bp + Bp.^2 + 4*Cp.^2));
%      lambda2 = 1/2*( Ap + Bp - sqrt(Ap.^2 - 2.*Ap.*Bp + Bp.^2 + 4*Cp.^2));
     %CRF
     alpha = 0.05 ;
     Q= ( Ap.*Bp - Cp.^2 ) - alpha*((Ap+Bp).^2);
     %remove small values
     %th = max(max(Q))*0.9;
     th=25000;
     Q(Q<th)=0;
     
     %local maxima
     p=1;
     Corner=[];
     for i = 5 : ligne -5 -1
         for j = 5 : colonne -5-1
             maxq= max(max(Q(i-4:i+4,j-4:j+4)));
             if Q(i,j)~=0 && (Q(i,j)==maxq)
                 Corner(p,1:3)=[i,j,Q(i,j)];
                 p=p+1;
             end
         end
     end
     
     %sort you can keep just the highest values
     %tri vous pouvais garder les plus hautes valeurs.
     [value tri] = sort(Corner(:,3),1,'descend');
     for i = 1 : p-1
         Cornersort(i,:)=Corner(tri(i),:);
     end
     
%      sizecorner =length(Cornersort);
%      for i = 1 : sizecorner
%          if Cornersort(i,1)~= 0
%              Cornerinter = Cornersort(i,:);
%              for j=i+1 : sizecorner
%                  dist = ((Cornerinter(1)-Cornersort(j,1))^2 + (Cornerinter(2)-Cornersort(j,2))^2)^0.5;
%                  if dist < 10
%                      Cornersort(j,:) = [0 0 0];
%                  end
%              end
%          end
%      end
     
%      [value tri] = sort(Cornersort(:,3),1,'descend');
%      for i = 1 : p-1
%          if Cornersort(tri(i),:) ~= 0
%              Cornersortdist(i,:)=Corner(tri(i),:);
%          end
%      end

     img2=uint8(img)./3;
      for i = 1 : length(Cornersort)
         img2(Cornersort(i,1),Cornersort(i,2)) = 255;
     end
     axes(a(2))
     imshow(uint8(img2))
     figure
     imshow(uint8(img2))
     
end
end