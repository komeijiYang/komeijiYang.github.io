prompt = {'采样率 (Hz)', '持续时间 (秒)', '频率 (Hz) (用逗号分隔多个频率)', '幅度 (0~1)', '波形类型 (sine, square, sawtooth, triangle)', '相位偏移 (弧度)', '声音强度 (0~1)'};
dlg_title = '声音参数输入';
num_lines = 1;
default_vals = {'44100', '3', '90', '1', 'square', '0', '1'}; 
answer = inputdlg(prompt, dlg_title, num_lines, default_vals);
Fs = str2double(answer{1});        % 采样率
duration = str2double(answer{2});  % 持续时间
frequency_str = answer{3};         % 频率（字符串形式，逗号分隔）
amplitude = str2double(answer{4}); % 振幅
waveform_type = answer{5};         % 波形类型
phase = str2double(answer{6});     % 相位偏移
sound_intensity = str2double(answer{7}); % 声音强度
frequency = str2num(frequency_str); 
attack_time = 0.2;   % 攻击时间
sustain_level = 0.7; % 持续阶段的音量比例
release_time = 0.4;  % 释放时间
t = 0:1/Fs:duration;
envelope = ones(size(t)); 
attack_samples = round(attack_time * Fs);
release_samples = round(release_time * Fs);
sustain_samples = length(t) - (attack_samples + release_samples);
envelope(1:attack_samples) = linspace(0, 1, attack_samples);
envelope(attack_samples+1:attack_samples+sustain_samples) = sustain_level;
envelope(end-release_samples+1:end) = linspace(sustain_level, 0, release_samples);

% 初始化波形
y = zeros(size(t));
% 根据波形类型生成波形，叠加多个频率
for i = 1:length(frequency)
    switch waveform_type
        case 'sine'
            y = y + amplitude * sin(2 * pi * frequency(i) * t + phase);
        case 'square'
            y = y + amplitude * square(2 * pi * frequency(i) * t + phase);
        case 'sawtooth'
            y = y + amplitude * sawtooth(2 * pi * frequency(i) * t + phase);
        case 'triangle'
            y = y + amplitude * sawtooth(2 * pi * frequency(i) * t + phase, 0.5);
        otherwise
            error('未识别的波形类型');
    end
end
y = y * sound_intensity;
y = y .* envelope;
sound(y, Fs);
audiowrite('声音.wav', y, Fs);

% 计算声压级
P_ref = 20e-6; % 参考声压 20 µPa
SPL = 20 * log10(abs(y) / P_ref);

% 绘制波形、频谱和声压级图像
figure;
subplot(4, 1, 1);
plot(t, y);
title('声音波形');
xlabel('时间 (秒)');
ylabel('幅度');
grid on;

Y_fft = fft(y);
n = length(y);         
f = (0:n-1)*(Fs/n);    
power = abs(Y_fft).^2/n; 
subplot(4, 1, 2);
plot(f(1:floor(n/2)), power(1:floor(n/2)));
title('频率谱');
xlabel('频率 (Hz)');
ylabel('功率');
grid on;

subplot(4, 1, 3);
plot(t, envelope);
title('ADSR 包络（无衰减）');
xlabel('时间 (秒)');
ylabel('幅度');
grid on;

% 绘制声压级
subplot(4, 1, 4);
plot(t, SPL);
title('声压级 (dB)');
xlabel('时间 (秒)');
ylabel('声压级 (dB)');
grid on;
