% 清除工作区和命令窗口
clear;
clc;
 
% 定义符号变量
syms x;
 
% 定义函数表达式
f = log(cos(x^2) + sin(x));
 
% 计算函数在x0=0处的Taylor多项式（例如，计算到5阶）
n = 5;
taylor_expr = taylor(f, x, 0, 'Order', n+1); % Order 参数指定展开到的阶数，包含x^n项
 
% 将符号表达式转换为函数句柄以便数值计算
taylor_func = matlabFunction(taylor_expr);
 
% 定义x的范围用于绘图
x_vals = linspace(-pi, pi, 1000);
 
% 计算原函数和Taylor多项式的值
y_orig = log(cos(x_vals.^2) + sin(x_vals));
y_taylor = taylor_func(x_vals);
 
% 绘制图形
figure;
plot(x_vals, y_orig, 'b-', 'LineWidth', 2); % 原函数，蓝色实线
hold on;
plot(x_vals, y_taylor, 'r--', 'LineWidth', 2); % Taylor多项式，红色虚线
title('函数 y = ln(cos(x^2) + sin(x)) 及其Taylor多项式图形');
xlabel('x');
ylabel('y');
legend('原函数', 'Taylor多项式');
grid on;
hold off;