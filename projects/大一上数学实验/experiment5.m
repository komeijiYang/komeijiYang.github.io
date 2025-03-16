% 计算定积分 ∫sin(x^2)dx 的 Riemann 和
function riemann_sum = calculate_riemann_sum(a, b, n)
    % a: 积分下限
    % b: 积分上限
    % n: 分区数量
    h = (b - a) / n;  % 分区宽度
    riemann_sum = 0;
    for i = 1:n
        x_i = a + (i - 0.5) * h;  % 取每个小区间的中点
        riemann_sum = riemann_sum + sin(x_i^2) * h;
    end
end
 
% 使用梯形积分法计算定积分 ∫sin(x^2)dx 的近似值
function trapezoidal_approx = trapezoidal_integral(a, b, tol)
    % a: 积分下限
    % b: 积分上限
    % tol: 精度要求
    n = 1;
    h = (b - a) / n;
    integral_approx = 0.5 * (sin(a^2) + sin(b^2)) * h;
    while true
        n = n * 2;
        h = (b - a) / n;
        new_integral_approx = 0.5 * (sin(a^2) + sin(b^2)) * h;
        for i = 1:n-1
            x_i = a + i * h;
            new_integral_approx = new_integral_approx + sin(x_i^2) * h;
        end
        if abs(new_integral_approx - integral_approx) < tol
            trapezoidal_approx = new_integral_approx;
            break;
        end
        integral_approx = new_integral_approx;
    end
end
 
% 使用Simpson法计算定积分 ∫sin(x^2)dx 的近似值
function simpson_approx = simpson_integral(a, b, tol)
    % a: 积分下限
    % b: 积分上限
    % tol: 精度要求
    n = 2;  % Simpson法要求n为偶数
    h = (b - a) / n;
    integral_approx = sin(a^2) + sin(b^2);
    for i = 1:2:n-1
        x_i = a + i * h;
        integral_approx = integral_approx + 4 * sin(x_i^2);
    end
    integral_approx = integral_approx * h / 3;
    
    while true
        n = n * 2;
        h = (b - a) / n;
        new_integral_approx = sin(a^2) + sin(b^2);
        for i = 1:2:n-1
            x_i = a + i * h;
            new_integral_approx = new_integral_approx + 4 * sin(x_i^2);
        end
        new_integral_approx = new_integral_approx * h / 3;
        
        if abs(new_integral_approx - integral_approx) < tol
            simpson_approx = new_integral_approx;
            break;
        end
        integral_approx = new_integral_approx;
    end
end
 
% 主程序
a = 0;  % 积分下限
b = pi; % 积分上限
tol = 0.0001; % 精度要求
 
% 计算Riemann和
n = 1000; % 分区数量，可以根据需要调整
riemann_result = calculate_riemann_sum(a, b, n);
fprintf('Riemann和: %.5f\n', riemann_result);
 
% 使用梯形积分法
trapezoidal_result = trapezoidal_integral(a, b, tol);
fprintf('梯形积分法近似值: %.5f\n', trapezoidal_result);
 
% 使用Simpson法
simpson_result = simpson_integral(a, b, tol);
fprintf('Simpson法近似值: %.5f\n', simpson_result);