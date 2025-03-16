% 定义x的范围
x = linspace(-2*pi, 2*pi, 1000);
 
% 定义cos(x)
y_cos = cos(x);
 
% 绘制cos(x)的图形
figure;
plot(x, y_cos, 'b', 'LineWidth', 2);
hold on;
title('Cosine Function and Its Taylor Polynomials at x0=0');
xlabel('x');
ylabel('y');
legend('cos(x)');
grid on;
 
% 定义Taylor多项式的阶数
N = 1:10; % 从1阶到10阶
 
% 在x0=0处计算Taylor多项式的系数
% cos(x)的Taylor级数展开式为：sum((-1)^j * x^(2j) / (2j)!)
% 在x0=0时，所有项都基于x的幂次展开
 
% 初始化Taylor多项式矩阵
y_taylor = zeros(length(N), length(x));
 
% 计算并绘制Taylor多项式的图形
for k = 1:length(N)
    n = N(k);
    % Taylor多项式展开式
    P = 0;
    for j = 0:n
        P = P + ((-1).^j * x.^(2*j)) / factorial(2*j);
    end
    y_taylor(k, :) = P;
    
    % 绘制Taylor多项式图形
    plot(x, y_taylor(k, :), 'DisplayName', ['n = ', num2str(n)]);
end
 
% 更新图例
legend show;
hold off;