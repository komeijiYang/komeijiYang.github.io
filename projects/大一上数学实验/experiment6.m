% 定义微分方程
function dydt = myode(x, y)
    dydt = zeros(2,1);
    dydt(1) = y(2);
    dydt(2) = (1 - x^2 * y(1) * sin(x)) / x;
end
 
% 定义边界条件
function res = adjusted_bc(ya, yb)
    res = [ya(1) - 1; yb(1) - 0]; % 第一个条件是初始条件，第二个条件是假设的右端点条件
end
 
% 网格划分
x = linspace(2, 5, 10); % 使用较少的点开始，以便更快地调试
 
% 创建初始猜测解的结构体
% 假设 y' 的初始猜测为 0，y 的初始值为 1
yinit = @(x) [1; 0]; % 这是一个函数句柄，返回一个两元素的列向量
solinit = bvpinit(x, yinit);
 
% 求解微分方程
sol = bvp4c(@myode, @adjusted_bc, solinit);
 
% 提取数值解
xsol = linspace(2, 5, 100);
ysol = deval(sol, xsol);
 
% 绘制数值解的图形
figure;
plot(xsol, ysol(1,:), 'b-');
xlabel('x');
ylabel('y');
title('初值问题数值解的图形');
grid on;
legend('y(x)');
% 提取数值解的 x 和 y 值
xsol = linspace(2, 5, 9); % 用于评估解的 x 值范围
[ysol, ~] = deval(sol, xsol); % ysol 是 y 的数值解，~ 表示我们不关心 y' 的数值解
 
% 输出 x 和 y 对应的数值解
for i = 1:length(xsol)
    fprintf('x = %.4f, y = %.4f\n', xsol(i), ysol(1,i));
end