
%% 东南大学九龙湖教一楼简化模型 V6 - 左右立面窗户对称修复版
clear; clc; close all;
figure('Name','东南大学九龙湖教一楼简化模型','Position',[100 100 1000 700]);
axis equal; grid on; hold on;
xlabel('长度方向 (m)'); ylabel('宽度方向 (m)'); zlabel('高度 (m)');
title('教一楼几何模型 - 对称窗户修复版');
view(35, 25); rotate3d on;

%% 参数设置
length_x = 60; width_y = 24; height_z = 18; floors = 5;
window_w = 1.8; window_h = 1.5;

%% 主体与地基
fill3([0 length_x length_x 0], [0 0 width_y width_y], [-0.2 -0.2 -0.2 -0.2], [0.7 0.7 0.7]);
drawCuboid(0, 0, 0, length_x, width_y, height_z, [0.8 0.8 0.9]);

%% 主入口
door_w = 4; door_h = 3; door_x = length_x/2 - door_w/2;
drawDoor(door_x, 0, 0, door_w, door_h);

%% 窗户系统
for f = 1:floors
    z_base = (f-1)*(height_z/floors) + 1.5;

    % 正面
    for i = 1:4
        x = 8 + (i-1)*12;
        drawWindow(x, -0.1, z_base, window_w, window_h, 'front');
    end

    % 左右立面（完全对称）
    for i = 1:10
        y_pos = 4 + (i-1)*5.5;
        drawWindow(0, y_pos, z_base, window_w, window_h, 'left');
        drawWindow(length_x, y_pos, z_base, window_w, window_h, 'right');
    end
end

%% 楼层分隔线
for f = 1:floors
    z = f * (height_z / floors);
    plot3([0 length_x], [0 0], [z z], 'k-', 'LineWidth', 1);
    plot3([0 length_x], [width_y width_y], [z z], 'k-', 'LineWidth', 1);
    plot3([0 0], [0 width_y], [z z], 'k-', 'LineWidth', 1);
    plot3([length_x length_x], [0 width_y], [z z], 'k-', 'LineWidth', 1);
end

%% 天台围栏
railing_h = 1.2;
for i = 1:4:length_x
    plot3([i i], [0 0], [height_z height_z + railing_h], 'k-', 'LineWidth', 1.5);
    plot3([i i], [width_y width_y], [height_z height_z + railing_h], 'k-', 'LineWidth', 1.5);
end
for j = 1:4:width_y
    plot3([0 0], [j j], [height_z height_z + railing_h], 'k-', 'LineWidth', 1.5);
    plot3([length_x length_x], [j j], [height_z height_z + railing_h], 'k-', 'LineWidth', 1.5);
end

%% 天台楼梯间
stair_w = 5; stair_l = 6; stair_h = 3;
sx = length_x/2 - stair_l/2; sy = width_y - stair_w - 1;
drawCuboid(sx, sy, height_z, stair_l, stair_w, stair_h, [0.7 0.7 0.7]);

%% 天台连接通道
conn_l = 4; conn_w = 6; conn_h = 2.8;
conn_x = length_x; conn_y = width_y/2 - conn_w/2;
drawCuboid(conn_x, conn_y, height_z + 0.01, conn_l, conn_w, conn_h, [0.8 0.8 0.85]);

%% 标注
text(length_x/2, width_y/2, height_z+5, '教一楼', 'FontSize', 14, ...
     'FontWeight','bold', 'HorizontalAlignment','center');
text(length_x/2, -2, 1.5, '主入口', 'FontSize', 12, 'HorizontalAlignment','center');

%% 渲染
light('Position', [length_x/2 -10 height_z+10], 'Style', 'infinite');
light('Position', [length_x/2 width_y+10 height_z+10], 'Style', 'infinite');
lighting gouraud;
material([0.4 0.6 0.4]);
set(gcf, 'Color', [1 1 1]);

annotation('textbox', [0.1 0.05 0.2 0.05], 'String', ...
    sprintf('建筑尺寸: %.0fm×%.0fm×%.0fm', length_x, width_y, height_z), ...
    'FitBoxToText', 'on', 'BackgroundColor', [1 1 1]);

%% 函数定义
function drawCuboid(x, y, z, lx, ly, lz, color)
    fill3([x x+lx x+lx x], [y y y+ly y+ly], [z z z z], color, 'FaceAlpha', 0.7);
    fill3([x x+lx x+lx x], [y y y+ly y+ly], [z+lz z+lz z+lz z+lz], color, 'FaceAlpha', 0.7);
    fill3([x x+lx x+lx x], [y y y y], [z z z+lz z+lz], color, 'FaceAlpha', 0.7);
    fill3([x x+lx x+lx x], [y+ly y+ly y+ly y+ly], [z z z+lz z+lz], color, 'FaceAlpha', 0.7);
    fill3([x x x x], [y y y+ly y+ly], [z z+lz z+lz z], color, 'FaceAlpha', 0.7);
    fill3([x+lx x+lx x+lx x+lx], [y y y+ly y+ly], [z z+lz z+lz z], color, 'FaceAlpha', 0.7);
end

function drawDoor(x, y, z, w, h)
    patch([x x+w x+w x], [y y y y], [z z z+h z+h], [0.4 0.3 0.2], 'FaceAlpha', 0.9);
    patch([x+0.1 x+w/2 x+w/2 x+0.1], [y-0.1 y-0.1 y-0.1 y-0.1], ...
          [z+0.1 z+0.1 z+h-0.1 z+h-0.1], [0.6 0.5 0.4]);
    patch([x+w/2+0.1 x+w-0.1 x+w-0.1 x+w/2+0.1], [y-0.1 y-0.1 y-0.1 y-0.1], ...
          [z+0.1 z+0.1 z+h-0.1 z+h-0.1], [0.6 0.5 0.4]);
end

function drawWindow(x, y, z, w, h, face)
    frame_color = [0.3 0.3 0.3];
    glass_color = [0.7 0.9 1];
    offset = 0.05;
    switch face
        case 'front'
            patch([x x+w x+w x], [y y y y], [z z z+h z+h], frame_color);
            patch([x+offset x+w-offset x+w-offset x+offset], ...
                  [y+0.01 y+0.01 y+0.01 y+0.01], ...
                  [z+offset z+offset z+h-offset z+h-offset], glass_color, 'FaceAlpha', 0.5);
        case 'left'
            patch([y y+w y+w y], [x x x x], [z z z+h z+h], frame_color);
            patch([y+offset y+w-offset y+w-offset y+offset], ...
                  [x+0.01 x+0.01 x+0.01 x+0.01], ...
                  [z+offset z+offset z+h-offset z+h-offset], glass_color, 'FaceAlpha', 0.5);
        case 'right'
            patch([x x x x], ...
                  [y y y+h y+h], ...
                  [z z+w z+w z], frame_color);
            patch([x-0.01 x-0.01 x-0.01 x-0.01], ...
                  [y+offset y+h-offset y+h-offset y+offset], ...
                  [z+offset z+offset z+w-offset z+w-offset], ...
                  glass_color, 'FaceAlpha', 0.5);
    end
end
