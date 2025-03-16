% 1. 制作函数 y = sin(c*x) 的图形动画，并观察参数 c 对函数图形的影响
 
% 定义 x 的范围
x = linspace(-2*pi, 2*pi, 1000);
 
% 创建图形窗口
figure;
h = plot(x, sin(x)); % 初始图形 y = sin(x)
xlabel('x');
ylabel('y');
title('y = sin(c*x) 的动画');
axis([-2*pi 2*pi -1.5 1.5]);
grid on;
 
% 动画参数设置
c_values = linspace(0.1, 10, 100); % c 的取值范围及步长
for i = 1:length(c_values)
    c = c_values(i);
    set(h, 'YData', sin(c*x)); % 更新 y 数据
    pause(0.1); % 控制动画速度
end
 
% 2. 已知函数 f(x) = x^2 + 2x + c (-5 ≤ x ≤ 4)，当 c 取 -1, 0, 1, 2, 3 时，分别绘制该函数的图形
 
% 定义 x 的范围
x = linspace(-5, 4, 1000);
 
% 创建图形窗口
figure;
hold on;
colors = lines(5); % 获取5种不同的颜色
 
% 绘制不同 c 值的函数图形
for i = 1:5
    c = i - 6; % c 取 -1, 0, 1, 2, 3
    y = x.^2 + 2*x + c;
    plot(x, y, 'Color', colors(i,:), 'DisplayName', sprintf('c = %d', c));
end
 
% 添加图例、标签和标题
legend show;
xlabel('x');
ylabel('f(x)');
title('f(x) = x^2 + 2x + c (-5 ≤ x ≤ 4) 的图形');
grid on;
 
% 观察极值点、驻点、单调性、凹凸性和渐近线
% 注意：这些特性可以通过观察图形得出，MATLAB 没有直接的函数来标注它们，
% 但可以通过求导和分析函数性质来确定。例如，极值点和驻点可以通过求导并令其为0来找到。