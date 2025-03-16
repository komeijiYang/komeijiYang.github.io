% 初始化变量
x1 = 1/2; % 初始值
n_max = 1000; % 最大迭代次数
xn_values = zeros(1, n_max); % 存储xn的值
xn_values(1) = x1; % 设置初始值
 
% 计算数列{xn}
for n = 1:n_max-1
    xn_values(n+1) = xn_values(n)^2 + xn_values(n);
end
 
% 假设yn = xn（或根据具体需求调整）
yn_values = xn_values;
 
% 绘制图形
figure;
subplot(2,1,1);
plot(1:n_max, xn_values, '-o');
xlabel('n');
ylabel('xn');
title('数列{xn}的变化趋势');
grid on;
 
subplot(2,1,2);
stairs(1:n_max, yn_values, 'LineWidth', 1.5); % 使用stairs绘制阶梯图，更清晰地显示变化趋势
xlabel('n');
ylabel('yn');
title('数列{yn}的变化趋势');
grid on;
 
% 判断极限是否存在（简单方法：观察最后几个值是否趋于稳定）
% 注意：这种方法不是严格的数学证明，仅用于初步判断
limit_xn = xn_values(end-10:end); % 取最后10个值
if all(diff(limit_xn) < 1e-6) % 如果这些值的差异非常小（例如小于1e-6），则认为极限存在
    fprintf('根据初步判断，数列{xn}（及{yn}）的极限可能存在，约为：%.6f\n', limit_xn(end));
else
    fprintf('根据初步判断，数列{xn}（及{yn}）的极限可能不存在或需要进一步分析。\n');
end