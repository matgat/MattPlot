%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% MatPlot's auto-generated m-file #date#
% Load in workspace all tracks contained in #FrameName#
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%------------------------------------------------------------------------
% -- Data section
disp('Loading MatPlot #FrameName# datas...')

% This is the independent variable of track
tr_x = [0.1;0.2;0.3;0.4;0.5;0.65;0.78;0.83;0.9];
% This is the dependent variable of track
tr_y = [0.4;0.7;1.0;1.3;1.6;1.9;2.2;2.5;3];

% The independent variable of track is tr_x
% This is the dependent variable of track
tr2_y = [2.4;1.7;1.0;1.3;0.6;0.9;0.2;0.1;-0.2];

disp('...Done')

%------------------------------------------------------------------------
% -- Plot section
if ( ~exist('plotalso','var') )
    plotalso = 0; % ensure this global variable
end
if ( plotalso )
    disp('Preparing plots...')
    figure();
    grid on;
    hold on;
    plot(tr_x,tr_y,tr_x,tr2_y);
    legend('tr []','tr2 []');
    %set(get(gca,'XLabel'),'String','time [s]')
    set(get(gca,'Title'),'Interpreter','none')
    set(get(gca,'Title'),'String','MatPlot || #FrameTitle#')
    hold off;
    disp('...Done')
end
