%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Generate white noise
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function a = WhiteNoise(N,A)

%----------------------------------------------------------
% -- Build noise
k=0.01;
t = (0:1/N:1-1/N)';
y1 = wgn(N,1,A);
y2 = wgn(N,1,k*A) + k*A;

%----------------------------------------------------------
% -- Save file
T = [t,y1,y2];

%save ('wnoise.txt','T','-ascii','-double','-tabs')

fid = fopen('wnoise.txt', 'wt');
fprintf(fid, '%s\n', '#header <t><y1(t)><y2(t)>');
fprintf(fid, '%G\t%G\t%G\n', T');
fclose(fid);


%----------------------------------------------------------
% -- Plot section
figure(1);
grid on;
hold on;
plot(t,y1,t,y2);
legend('y1','y2');
title('White noise');
xlabel('');
ylabel('');
hold off;
end