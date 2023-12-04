clc;clear;close all;
T1 = readtable('datarates0.csv');
T2 = readtable('datarates1.csv');
T3 = readtable('datarates2.csv');
T1.Var3(T1.Var2>100)=0;
T2.Var3(T2.Var2>120)=0;
T3.Var3(T3.Var2>140)=0;
T2.Var3(T2.Var2<10)=0;
T3.Var3(T3.Var2<40)=0;
%%
figure(1);
plot(T1.Var2,smooth(T1.Var3),LineWidth=2);
figure(2);
plot(T2.Var2,T2.Var3,LineWidth=2);
figure(3);
plot(T3.Var2,T3.Var3,LineWidth=2);

%%
Td1 = readtable("delaybounds.csv");
%%
Td1.Var4 = str2double(Td1.Var4);
Td1.Var5 = str2double(Td1.Var5);
Td1.Var6 = str2double(Td1.Var6);
Td1.Var7 = str2double(Td1.Var7);
Td1.Var8 = str2double(Td1.Var8);
Td1.Var9 = str2double(Td1.Var9);

%%
Td1.Var3(Td1.Var3<0) = inf;
Td1.Var6(Td1.Var6<0) = inf;
Td1.Var9(Td1.Var9<0) = inf;
plot(Td1.Var2(Td1.Var2>=10 & Td1.Var2<=10.2),Td1.Var3(Td1.Var2>=10 & Td1.Var2<=10.2));
hold on;
plot(Td1.Var5(Td1.Var5>=10 & Td1.Var5<=10.2),Td1.Var6(Td1.Var5>=10 & Td1.Var5<=10.2));
%%
figure;
plot(Td1.Var2(Td1.Var2>=40 & Td1.Var2<=42),Td1.Var3(Td1.Var2>=40 & Td1.Var2<=42),Color='b');
hold on;
plot(Td1.Var5(Td1.Var5>=40 & Td1.Var5<=42),Td1.Var6(Td1.Var5>=40 & Td1.Var5<=42),Color='r');
hold on;
plot(Td1.Var8(Td1.Var8>=40 & Td1.Var8<=42),Td1.Var9(Td1.Var8>=40 & Td1.Var8<=42),Color='g');
%%
figure;
plot(Td1.Var5(Td1.Var5>=100 & Td1.Var5<=102),Td1.Var6(Td1.Var5>=100 & Td1.Var5<=102),Color='r');
hold on;
plot(Td1.Var8(Td1.Var8>=100 & Td1.Var8<=102),Td1.Var9(Td1.Var8>=100 & Td1.Var8<=102),Color='k')
