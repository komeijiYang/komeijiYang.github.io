% 问题1：分别用图形法和二分法求方程 sin(x) + cos(x) = 0 在闭区间 [-1, 4] 内的解
 
% 图形法
f = @(x) sin(x) + cos(x);
x_graph = linspace(-1, 4, 1000);
y_graph = f(x_graph);
 
figure;
plot(x_graph, y_graph);
hold on;
plot(x_graph, -y_graph, '--');
xlabel('x');
ylabel('f(x)');
title('sin(x) + cos(x) = 0');
grid on;
 
% 视觉估计图形法求得的根（实际使用时需手动从图中读取）
% 示例估计值（需根据图像调整）
root_graph = -1.316957; % 注意：此值为视觉估计，实际运行时需根据图像确定
 
% 二分法
tol = 1e-6;
a = -1;
b = 4;
 
while (b - a) / 2 > tol
    c = (a + b) / 2;
    if f(c) == 0
        break;
    elseif f(a) * f(c) < 0
        b = c;
    else
        a = c;
    end
end
 
root_bisection = (a + b) / 2;
fprintf('图形法求得根: %.6f\n', root_graph);
fprintf('二分法求得根: %.6f\n', root_bisection);
 
% 问题2：用Newton迭代法求方程 x^2 + sqrt(x) - 3 = 0 的近似解
 
f = @(x) x^2 + sqrt(x) - 3;
df = @(x) 2*x + 1./(2*sqrt(x));
 
x0 = 1; % 初始猜测值
tol = 1e-6;
max_iter = 100;
 
for iter = 1:max_iter
    x1 = x0 - f(x0) / df(x0);
    if abs(x1 - x0) < tol
        break;
    end
    x0 = x1;
end
 
if iter == max_iter
    warning('未能收敛到指定误差范围内');
end
 
root_newton = x1;
fprintf('Newton迭代法求得根: %.6f\n', root_newton);