close all;
clc;
%Note:
%In this script the data from the T3 current draw test there needs to uploaded with the names: Start, scope, and MaxCur.



jump = table2array(start);
sco = table2array(scope);
Maxc = table2array(MaxCur);




f1 = figure;  
f1.Position = [30 30 700 400];
hold on;
plot(jump(:, 1)+5, jump(:, 2), 'Color', 'k');


x1 = xline([4.4, 10],'-',{'Servo Movement','Jump and balance'}, ...
    'Color', [0.6, 0.6, 0.6], 'LabelHorizontalAlignment', 'left');
hold off;

title("Current draw of jump",FontName="Palatino Linotype",FontSize=12);
ylabel("Current (Ampere)",FontName="Palatino Linotype",FontSize=11);
xlabel("Time (seconds)",FontName="Palatino Linotype",FontSize=11);


f3 = figure;  
f3.Position = [30 30 700 400];
hold on;
plot(Maxc(:, 1)+5, Maxc(:, 2), 'Color', 'k')
x1 = xline([3.3, 5, 10],'-',{'Jump','Small push', 'Lowering frame'}, ...
    'Color', [0.6, 0.6, 0.6], 'LabelHorizontalAlignment', 'left');

hold off;
title("Current draw of Limit",FontName="Palatino Linotype",FontSize=12);
ylabel("Current (Ampere)",FontName="Palatino Linotype",FontSize=11);
xlabel("Time (seconds)",FontName="Palatino Linotype",FontSize=11);