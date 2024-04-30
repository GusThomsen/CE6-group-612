clc;

f = @(theta) sin(theta);
g = @(theta) theta;
lim = deg2rad(10);
theta = -1:0.01:1;
plot(theta, f(theta), "Color","blue");
title("Linerisation of sin(theta)")
xlabel("Radians");
ylabel("Amplitude")
hold on
plot(theta, g(theta), 'Color','green');
L = legend('sin(theta)', 'theta', Location='southeast');
L.AutoUpdate = "off";
xline([-lim, lim], '-r', 'LineWidth',1.25)

hold off
