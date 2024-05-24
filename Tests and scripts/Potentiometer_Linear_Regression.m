%create linear regression
Measured_values = [45 ,35 ,25  ,15  ,5   ,0   ,-5  ,-15  ,-25,-35 ,-45;
                    3 ,125,245,316,386,460,509,670,745,863,1001]
x =Measured_values(2,:);
y = Measured_values(1,:);
mdl = fitlm(x,y);

line_colors = [0 0 0; 0.6 0.6 0.6]; % Black and grey (RGB)
set(gca, 'ColorOrder', line_colors);
hold on 
h = plot(mdl);
title("Voltage vs. Angle",FontName="Palatino Linotype",FontSize=12);
ylabel("Measured voltage (Volts)",FontName="Palatino Linotype",FontSize=11);
xlabel("Measured angle (Degrees)",FontName="Palatino Linotype",FontSize=11);
grid on;
% Remove confidence bounds
h(1).Marker = 's' ;
delete(h(3));
hold off
%add Legend (very tedious)
legendRVal = "R^2 = " +string(round(mdl.Rsquared.Ordinary,3));
legendYOffset = string(round(mdl.Coefficients.Estimate(1),4));
legendX = string(round(mdl.Coefficients.Estimate(2),4));
pointName = "Measured points";
lineName ="y="+ legendX +"x" + "+" + legendYOffset+ newline+ legendRVal ;
legend(pointName,lineName);
f = gcf;
exportgraphics(f,'Potentiometer_linear_regression.pdf','Resolution',1200)
