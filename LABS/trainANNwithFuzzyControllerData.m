clc;
clear;
close all;

%% =====================================================
% 1. MEMBACA DATASET
% ======================================================

data = readtable('dataset_fuzzy_motor_20_150.csv');

% Tampilkan nama kolom untuk memastikan
disp(data.Properties.VariableNames);

%% =====================================================
% 2. MENGAMBIL FITUR INPUT DAN TARGET
% ======================================================

setpoint     = data.setpoint;
rpm          = data.rpm;
error        = data.error;
delta_error  = data.delta_error;
delta_pwm    = data.delta_pwm;
pwm          = data.pwm;

% Input NN:
% error dan delta_error
X = [error'; delta_error'];

% Target NN:
% delta_pwm dari fuzzy controller
T = delta_pwm';

%% =====================================================
% 3. MEMBERSIHKAN DATA
% ======================================================

% Hilangkan NaN atau Inf jika ada
valid_idx = all(isfinite(X), 1) & isfinite(T);

X = X(:, valid_idx);
T = T(:, valid_idx);

fprintf('Jumlah data valid: %d sampel\n', size(X, 2));

%% =====================================================
% 4. NORMALISASI DATA
% ======================================================

% Normalisasi input ke range -1 sampai 1
[Xn, psX] = mapminmax(X, -1, 1);

% Normalisasi target ke range -1 sampai 1
[Tn, psT] = mapminmax(T, -1, 1);

%% =====================================================
% 5. MEMBUAT MODEL NEURAL NETWORK
% ======================================================

hidden_neurons = 8;

net = feedforwardnet(hidden_neurons);

% Fungsi aktivasi
net.layers{1}.transferFcn = 'tansig';
net.layers{2}.transferFcn = 'purelin';

% Pembagian data
net.divideParam.trainRatio = 70/100;
net.divideParam.valRatio   = 15/100;
net.divideParam.testRatio  = 15/100;

% Parameter training
net.trainParam.epochs = 1000;
net.trainParam.goal   = 1e-6;
net.trainParam.min_grad = 1e-7;

% Agar tampilan training muncul
net.trainParam.showWindow = true;

%% =====================================================
% 6. TRAINING
% ======================================================

[net, tr] = train(net, Xn, Tn);

%% =====================================================
% 7. PREDIKSI DAN DENORMALISASI
% ======================================================

Yn = net(Xn);

% Kembalikan ke skala asli delta_pwm
Y = mapminmax('reverse', Yn, psT);

%% =====================================================
% 8. EVALUASI HASIL TRAINING
% ======================================================

mse_value = mean((T - Y).^2);
rmse_value = sqrt(mse_value);
mae_value = mean(abs(T - Y));

fprintf('\nEvaluasi Training Neural Controller:\n');
fprintf('MSE  = %.6f\n', mse_value);
fprintf('RMSE = %.6f\n', rmse_value);
fprintf('MAE  = %.6f\n', mae_value);

%% =====================================================
% 9. PLOT TARGET VS PREDIKSI
% ======================================================

figure;
plot(T, 'b', 'LineWidth', 1.2);
hold on;
plot(Y, 'r--', 'LineWidth', 1.2);
grid on;
legend('Target delta\_pwm Fuzzy', 'Prediksi delta\_pwm NN');
xlabel('Sample');
ylabel('delta\_pwm');
title('Perbandingan Target Fuzzy dan Prediksi Neural Network');

%% =====================================================
% 10. PLOT ERROR PREDIKSI
% ======================================================

prediction_error = T - Y;

figure;
plot(prediction_error, 'k', 'LineWidth', 1.1);
grid on;
xlabel('Sample');
ylabel('Error Prediksi');
title('Error Prediksi delta\_pwm');

%% =====================================================
% 11. REGRESSION PLOT
% ======================================================

figure;
plotregression(T, Y);
title('Regression Plot: Target Fuzzy vs Output NN');

%% =====================================================
% 12. PLOT PERFORMANCE
% ======================================================

figure;
plotperform(tr);

%% =====================================================
% 13. SIMPAN MODEL
% ======================================================

save('trained_nn_fuzzy_controller.mat', 'net', 'psX', 'psT');

disp('Model berhasil disimpan sebagai trained_nn_fuzzy_controller.mat');

%% =====================================================
% 14. EKSPOR BOBOT DAN BIAS
% ======================================================

IW = net.IW{1,1};   % bobot input ke hidden
b1 = net.b{1};      % bias hidden
LW = net.LW{2,1};   % bobot hidden ke output
b2 = net.b{2};      % bias output

disp('=====================================================');
disp('BOBOT DAN BIAS UNTUK ESP32');
disp('=====================================================');

disp('IW = ');
disp(IW);

disp('b1 = ');
disp(b1);

disp('LW = ');
disp(LW);

disp('b2 = ');
disp(b2);

disp('=====================================================');
disp('PARAMETER NORMALISASI INPUT');
disp('=====================================================');

disp('psX.xmin = ');
disp(psX.xmin);

disp('psX.xmax = ');
disp(psX.xmax);

disp('psX.ymin = ');
disp(psX.ymin);

disp('psX.ymax = ');
disp(psX.ymax);

disp('=====================================================');
disp('PARAMETER NORMALISASI TARGET');
disp('=====================================================');

disp('psT.xmin = ');
disp(psT.xmin);

disp('psT.xmax = ');
disp(psT.xmax);

disp('psT.ymin = ');
disp(psT.ymin);

disp('psT.ymax = ');
disp(psT.ymax);
