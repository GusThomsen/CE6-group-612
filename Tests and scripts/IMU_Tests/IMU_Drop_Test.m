clear;
line_colors = [0 0 0; 0.6 0.6 0.6]; % Black and grey (RGB)
set(gca, 'ColorOrder', line_colors);
IMUData = readtable("114840_IMUlog.log");
timeMillis = table2array(IMUData(:,1));
timeSec = timeMillis/1000;

compDataArray = table2array(IMUData(:,2));
gyroDataArray = table2array(IMUData(:,3));
accDataArray = table2array(IMUData(:,4));


mdl = fitlm(timeSec,gyroDataArray);


hold on

plot(timeSec,compDataArray,"-")
plot(timeSec,accDataArray,".")
plot(timeSec,gyroDataArray,":")

title("Gyro vs Accelerometer vs Complementaryfilter",FontName="Palatino Linotype",FontSize=12);
ylabel("Measured Angle (Radians)",FontName="Palatino Linotype",FontSize=11);
xlabel("Time (seconds)",FontName="Palatino Linotype",FontSize=11);
legend("compData","accData","gyroData",Location="southeast") ;
xlim([4.5,9.5])
ylim([-2 1])
hold off
f = gcf;
exportgraphics(f,'IMU_drop_test.pdf','Resolution',1200)% change output path

