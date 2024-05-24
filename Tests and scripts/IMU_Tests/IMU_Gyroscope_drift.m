clear;
line_colors = [0 0 0; 0.6 0.6 0.6]; % Black and grey (RGB)
set(gca, 'ColorOrder', line_colors);
IMUData = readtable("IMU_Gyroscope_drift_data.log");
timeMillis = table2array(IMUData(:,1));
timeSec = timeMillis/1000;

compDataArray = table2array(IMUData(:,2));
gyroDataArray = table2array(IMUData(:,3));
accDataArray = table2array(IMUData(:,4));


mdl = fitlm(timeSec,gyroDataArray);


hold on

plot(timeSec,compDataArray,"--")
%plot(timeSec,accDataArray,".")
plot(timeSec,gyroDataArray,":")
h = plot(mdl)
% Remove gyroscope measurements
delete(h(1))
% Remove confidence bounds
delete(h(3));

legend compData gyrodata
title("Gyro data & Complementary filter data",FontName="Palatino Linotype",FontSize=12);
ylabel("Measured Angle (Radians)",FontName="Palatino Linotype",FontSize=11);
xlabel("Time (seconds)",FontName="Palatino Linotype",FontSize=11);


legendRVal = "R^2 = " +string(round(mdl.Rsquared.Ordinary,3));
legendYOffset = string(round(mdl.Coefficients.Estimate(1),4));
legendX = string(round(mdl.Coefficients.Estimate(2),4));
pointName = "gyroData";
lineName ="y="+ legendX +"x" + "+" + legendYOffset+ newline+ legendRVal ;
legend("compData", pointName,lineName) ;
xlim([20,120])
hold off
f = gcf;
exportgraphics(f,'IMU_gyrodrift.pdf','Resolution',1200)% change output path

