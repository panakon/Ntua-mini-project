clc;clear;

Input = readtable('pendulum_input.csv');

Se      = zeros(2,1);
S       = zeros(2,1);
Sprev   = zeros(2,1);
Pe      = zeros(2,2);
Pprev   = zeros(2,2);
P       = zeros(2,2);
Z       = 0;
F       = zeros(2,2);

P0 = 1E-2*ones(2,2);
S0 = [pi/6 ; 0];

B = 0.8; %kg/s
M = 0.9; %kg
G = 9.81; %m/s^2
L = 0.5; %m
R = 0.8; %rad^2/s^2
Q =1E-6*ones(2,2);

H = [0 1];

f = @(x,t) [x(1)+x(2)*t ; x(2)*(1-B*t/M)-G*sin(x(1))*t/L];
h = @(x)  x(2);

i=0;
tprev=0;
Pprev = P0;
Sprev = S0;


result=array2table(zeros(0,5));
result.Properties.VariableNames= {'time','theta','dtheta','theta_cov','dtheta_cov'};


while i<size(Input,1)
    i=i+1;
    %READ
        Dt = Input.timestamp(i) - tprev;
        tprev = Input.timestamp(i);
        Z = Input.speed_noisy(i);
        F = [ 1 , Dt ; -G/L*cos(Sprev(1))*Dt , 1-B/M*Dt];
    %PREDICT
        Se = f(Sprev,Dt);
        Pe = F*Pprev*F' + Q;
    %GAIN
        K = (Pe*H')/(H*Pe*H'+R);
        %K = [1;1];
        
    %UPDATE
        S = Se + K*(Z - h(Se));
        P = (eye(2)-K*H)*Pe;
    %PREV
        Sprev = S;
        Pprev = P;
    %STORE
        new={tprev,S(1),S(2),P(1,1),P(2,2)};
        result=[result;new];
end
writetable(result,'pendulum_ekf_result.csv','WriteRowNames',true);
figure(1)
plot(Input.timestamp' , Input.displacement_truth','r')
hold on
plot(result.time',result.theta','b')
hold on
title('Estimation of angle over time')
xlabel('Time (s)')
ylabel('angle (rad)')
legend({'true value','estimation'},'Location','southeast')

figure(2)
plot(Input.timestamp', Input.speed_noisy','g')
hold on
plot(Input.timestamp' , Input.speed_truth','r')
hold on
plot(result.time',result.dtheta','b')
hold on
title('Estimation of angular velocity over time')
xlabel('Time (s)')
ylabel('angular velocity (rad/s)')
legend({'sensor input','true value','estimation'},'Location','southeast')

 


