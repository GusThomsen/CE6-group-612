clear
% Test is made by measuring 1000 data points from an MPU-6050 gyroscope where the IMU laid still
% on an MCU and then converting the data points to a CSV file which is read into this
% matlab script. The data points are then discrete integrated and outputtet
% on the scope 'Gyro drift'.
GyroVal = readmatrix("XdriftOnGyro.csv");
L = length(GyroVal);
simuSteps = L-1; %Lenght of the simulation, 1000 steps.

dt = 0.05; % delta time for the integration. Arbitrary number.

sim("GyroDriftSim.slx")
open_system("GyroDriftSim/Gyro drift")
% As it can be seen on the scope, the angle (X-axis) increases over time
% (Y-axis). This is the drift from integrating over time in the discrete
% time domain.