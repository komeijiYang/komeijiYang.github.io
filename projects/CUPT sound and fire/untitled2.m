clear all; close all; clc;
c = 343;          
rho0 = 1.225;     
L = 1;            
T = 0.02;        
Nx = 200;         
dx = L / (Nx - 1);
dt = dx / c * 0.9;
Nt = floor(T / dt);
r = c * dt / dx;
x = linspace(0, L, Nx);
p = zeros(Nx, Nt+1);
p(:,1) = sin(pi * x / L); 
p(:,2) = p(:,1);
for n = 2:Nt
    for i = 2:Nx-1
        p(i,n+1) = 2*p(i,n) - p(i,n-1) + r^2 * (p(i+1,n) - 2*p(i,n) + p(i-1,n));
    end
    p(1,n+1) = 0;
    p(Nx,n+1) = 0;
end
[X_grid, T_grid] = meshgrid(x, linspace(0, T, Nt+1));
figure;
surf(X_grid, T_grid, p');
shading interp;
xlabel('x (m)');
ylabel('t (s)');
zlabel('p');
title('波随时间传播');
