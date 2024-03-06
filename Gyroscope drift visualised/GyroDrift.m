clear
GyroVal = readmatrix("XdriftOnGyro.csv");
L = length(GyroVal);
simuSteps = L-1;

dt = 0.05;

sim("GyroDriftSim.slx")
open_system("GyroDriftSim/Gyro drift")