%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Generate data for memory dump example
%
% The DSP of a servo drive has stored in its RAM an array 
% of words (16bit) encoded as signed integers containing 
% the time history of some internal variables of the 
% control algorithm, sampled with different frequency. The 
% first 4000 words of this array are filled with the 
% measure of quadrature current Iq sampled every 62.5us 
% and scaled respect 5A; the following 250 contains the 
% motor speed sampled every 1ms and scaled respect 
% 3500rpm, while the remaining 125 are the samples of 
% measured DC bus voltage taken every 2ms and scaled 
% respect 1000V; voltage measures have a known offset of 
% 23.5V. The first chunk of each segment is used as 
% initial buffer to collect a certain amount of samples 
% before trigger event. The size of this chunk is 
% equivalent to 20ms and so is equal to 320 samples for 
% the first segment, 20 for the second and 10 for the 
% third. The trigger fired at sample 6 of the third 
% segment (the shortest one), corresponding to sample 12 
% of the second, 192 of the first. 
% After acquisition, data are converted to strings and 
% sent character by character through a RS232 serial line 
% to a PC that saves on hard disk a text file of 4375 
% lines containing a sequence of base 10 integer numbers 
% literals reproducing the array of 16bit signed integers
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%----------------------------------------------------------
% -- Build data

N = 4000;
t1 = (0:1/N:1-1/N)';
Iu1 = round( 30000 * sin( (10*pi) * t1) );
Iu = [Iu1(129:320);Iu1(1:128);Iu1(321:N)];

N = 250;
t2 = (0:1/N:1-1/N)';
w1 = round( 20000 * t2 + 5000);
w = [w1(9:20);w1(1:8);w1(21:N)];

N = 125;
t3 = (0:1/N:1-1/N)';
Vdc1 = round( 32767 * (1 - exp(t3)) + 32767 );
Vdc = [Vdc1(5:10);Vdc1(1:4);Vdc1(11:N)];

%----------------------------------------------------------
% -- Save file
T = [Iu;w;Vdc];
%save ('memdump.txt','T','-ascii')
fid = fopen('memdump.txt', 'wt');
fprintf(fid, '%d\n', T');
fclose(fid);

%----------------------------------------------------------
% -- Plot section
figure(1);
grid on;
hold on;
plot(t1,Iu,t2,w,t3,Vdc);
legend('Iu','w','Vdc');
title('Memory dump example');
xlabel('t');
ylabel('');
hold off;
