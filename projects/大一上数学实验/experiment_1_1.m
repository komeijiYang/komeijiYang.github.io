% 初始化变量
n_values = 1:1000; % n的取值范围，从1到1000
limit_values = (1 + 1./n_values).^n_values; % 计算极限值
 
% 绘制图形
figure;
plot(n_values, limit_values, '-o');
xlabel('n');
ylabel('lim(1 + 1/n)^n');
title('观察极限lim(1 + 1/n)^n的变化趋势');
grid on;