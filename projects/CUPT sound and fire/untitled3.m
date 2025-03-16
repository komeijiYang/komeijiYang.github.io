
x = linspace(-1, 1, 200);
y = linspace(0, 3, 300);
[X, Y] = meshgrid(x, y);
% 声波参数
rho0 = 1.22; % 空气密度（kg/m^3）
c = 343; % 声速（m/s）
f = 2000; % 声波频率（Hz）
omega = 2 * pi * f; % 角频率
k = omega / c; % 波数
A_sound = 1; % 声压振幅
% 时间参数
dt = 0.01;
t_end = 5;
t = 0:dt:t_end;
% 设置视频保存
video_name = 'candle_flame_simulation_with_sound.avi'; % 定义视频文件名
v = VideoWriter(video_name); 
v.FrameRate = 30; 
open(v); 
figure;
colormap hot;
axis tight manual;
axis off;
for i = 1:length(t)
    A = 1 + 0.02 * randn;
    B = 0.3 + 0.005 * randn;
    C = 1.5 + 0.005 * randn;
    D = 0.6;
    p = A_sound * sin(omega * t(i) - k * Y);
    I = A * exp(-((X / B).^2 + (Y - C).^2) / D);
    I = I .* exp(-abs(p));
    I(I < 0) = 0;
    imagesc(x, y, I);
    set(gca, 'YDir', 'normal');
    caxis([0, 1]);
    axis([min(x), max(x), min(y), max(y)]);
    axis off; 
    drawnow;
    % 获取当前帧并写入视频
    frame = getframe(gcf); % 获取当前图像帧
    writeVideo(v, frame); % 将帧写入视频
end
% 关闭视频文件
close(v);
disp(['视频已保存为: ', video_name]);
