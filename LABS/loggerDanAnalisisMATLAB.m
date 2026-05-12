clc;
clear;
close all;

%% ============================
%  Konfigurasi Serial
% ============================
port = "COM14";        % Ganti sesuai port Arduino/ESP32
baudrate = 115200;

s = serialport(port, baudrate);
configureTerminator(s, "LF");

pause(2);
flush(s);

disp("MATLAB Fuzzy Controller siap.");
disp("Pastikan Arduino/ESP32 mengirim RPM dan menerima nilai PWM.");

%% ============================
%  Pastikan Motor Berhenti Sebelum Mulai
% ============================
disp("Menyiapkan sistem. Motor dihentikan terlebih dahulu...");

writeline(s, "0");     % kirim PWM = 0
pause(2);              % beri waktu motor berhenti
flush(s);              % bersihkan buffer serial

%% ============================
%  Parameter Pengujian
% ============================
setpoint = input("Masukkan target RPM: ");

duration_s = 20;       % durasi logging dan kontrol
Ts = 0.1;              % sampling time kontrol 100 ms

pwmValue = 0;
last_error = 0;

%% ============================
%  Membuat FIS
% ============================
fis = createMotorFIS();

%% ============================
%  Variabel Logger
% ============================
time_log = [];
setpoint_log = [];
rpm_log = [];
error_log = [];
delta_error_log = [];
delta_pwm_log = [];
pwm_log = [];

%% ============================
%  Plot Real-Time
% ============================
figure("Name", "Fuzzy Motor DC Controller - MATLAB");

h1 = animatedline( ...
    'LineWidth', 1.5, ...
    'Color', 'red', ...
    'MaximumNumPoints', 300);

grid on;
xlabel("Waktu (s)");
ylabel("RPM");
title("Kontrol Kecepatan Motor DC dengan Fuzzy Logic MATLAB");
legend("Actual RPM", "Location", "best");

xlim([0 duration_s]);

yMin = 0;
yMax = 150;
ylim([yMin yMax]);

%% ============================
%  Loop Kontrol dan Logging 20 Detik
% ============================
disp("Kontrol dimulai selama 20 detik...");

tStart = tic;
lastDisplayTime = 0;
k = 0;

while toc(tStart) <= duration_s

    % ============================
    % Sampling time tetap
    % ============================
    targetTime = k * Ts;

    while toc(tStart) < targetTime
        pause(0.001);
    end

    % ============================
    % Ambil RPM terbaru dari serial
    % ============================
    latest_rpm = NaN;

    % Baca semua data yang tersedia, gunakan data terakhir
    while s.NumBytesAvailable > 0
        data = readline(s);
        val = str2double(strtrim(data));

        if ~isnan(val)
            latest_rpm = val;
        end
    end

    % Jika buffer kosong, tunggu satu data baru
    if isnan(latest_rpm)
        data = readline(s);
        latest_rpm = str2double(strtrim(data));
    end

    % Abaikan jika tetap tidak valid
    if isnan(latest_rpm)
        continue;
    end

    actual_rpm = latest_rpm;
    t = toc(tStart);

    % ============================
    % Hitung error dan delta error
    % ============================
    error = setpoint - actual_rpm;
    delta_error = error - last_error;

    % Batasi input agar sesuai range FIS
    error_fis = max(min(error, 100), -100);
    delta_error_fis = max(min(delta_error, 50), -50);

    % ============================
    % Inferensi fuzzy di MATLAB
    % ============================
    delta_pwm = evalfis(fis, [error_fis delta_error_fis]);

    % ============================
    % Update PWM
    % ============================
    pwmValue = pwmValue + delta_pwm;
    pwmValue = max(min(pwmValue, 255), 0);

    % Kirim PWM ke Arduino/ESP32
    writeline(s, num2str(round(pwmValue)));

    % ============================
    % Simpan data logger
    % ============================
    time_log(end+1,1) = t;
    setpoint_log(end+1,1) = setpoint;
    rpm_log(end+1,1) = actual_rpm;
    error_log(end+1,1) = error;
    delta_error_log(end+1,1) = delta_error;
    delta_pwm_log(end+1,1) = delta_pwm;
    pwm_log(end+1,1) = pwmValue;

    % ============================
    % Plot real-time
    % ============================
    addpoints(h1, t, actual_rpm);
    xlim([0 duration_s]);
    ylim([yMin yMax]);
    drawnow limitrate;

    % Judul ringkas
    if t - lastDisplayTime >= 0.5
        title(sprintf("Setpoint: %.1f RPM | Actual: %.1f RPM | PWM: %.0f", ...
            setpoint, actual_rpm, pwmValue));
        lastDisplayTime = t;
    end

    % Simpan error sebelumnya
    last_error = error;

    % Naik ke sampling berikutnya
    k = k + 1;
end

%% ============================
%  Stop Motor
% ============================
writeline(s, "0");
pause(0.5);
flush(s);
clear s;

disp("Kontrol selesai. Motor dihentikan.");

%% ============================
%  Cek Data
% ============================
if isempty(time_log)
    error("Data kosong. Periksa koneksi serial atau format data dari Arduino/ESP32.");
end

%% ============================
%  Simpan Data Logger ke CSV
% ============================
data = table( ...
    time_log, ...
    setpoint_log, ...
    rpm_log, ...
    error_log, ...
    delta_error_log, ...
    delta_pwm_log, ...
    pwm_log, ...
    'VariableNames', { ...
    'time_s', ...
    'setpoint', ...
    'rpm', ...
    'error', ...
    'delta_error', ...
    'delta_pwm', ...
    'pwm'});

filename = "log_fuzzy_matlab_controller_" + ...
    string(datetime("now", "Format", "yyyyMMdd_HHmmss")) + ".csv";

writetable(data, filename);

fprintf("Data logger disimpan sebagai: %s\n", filename);

%% ============================
%  Analisis Performa
% ============================
time_s = time_log;
rpm = rpm_log;
error_data = error_log;
pwm = pwm_log;
sp = setpoint;

% RPM maksimum
rpm_max = max(rpm);

% Overshoot
overshoot_rpm = max(0, rpm_max - sp);
overshoot_percent = (overshoot_rpm / sp) * 100;

% Rise time: dari 10% ke 90% setpoint
idx_10 = find(rpm >= 0.1 * sp, 1, "first");
idx_90 = find(rpm >= 0.9 * sp, 1, "first");

if ~isempty(idx_10) && ~isempty(idx_90)
    rise_time = time_s(idx_90) - time_s(idx_10);
else
    rise_time = NaN;
end

% Settling time: masuk dan bertahan dalam ±5% setpoint
tolerance = 0.05 * sp;
lower_bound = sp - tolerance;
upper_bound = sp + tolerance;

settling_time = NaN;

for i = 1:length(time_s)
    rpm_remaining = rpm(i:end);

    if all(rpm_remaining >= lower_bound & rpm_remaining <= upper_bound)
        settling_time = time_s(i);
        break;
    end
end

% Steady-state: 5 detik terakhir
idx_ss = time_s >= (max(time_s) - 5);

rpm_ss_mean = mean(rpm(idx_ss));
rpm_ss_std = std(rpm(idx_ss));
steady_state_error = sp - rpm_ss_mean;

% Integral Absolute Error
IAE = trapz(time_s, abs(error_data));

%% ============================
%  Plot Hasil Akhir
% ============================

% Plot RPM
figure;
plot(time_s, rpm, "LineWidth", 1.5);
hold on;
plot(time_s, setpoint_log, "--", "LineWidth", 1.2);
grid on;
xlabel("Waktu (s)");
ylabel("RPM");
title("Respons Kecepatan Motor DC - Fuzzy Controller di MATLAB");
legend("Actual RPM", "Setpoint", "Location", "best");
ylim([yMin yMax]);

% Plot Error
figure;
plot(time_s, error_data, "LineWidth", 1.5);
grid on;
xlabel("Waktu (s)");
ylabel("Error RPM");
title("Error Kecepatan Motor DC");

% Plot PWM
figure;
plot(time_s, pwm, "LineWidth", 1.5);
grid on;
xlabel("Waktu (s)");
ylabel("PWM");
title("Sinyal PWM dari Fuzzy Controller MATLAB");
ylim([0 255]);

%% ============================
%  Tampilkan Hasil Analisis
% ============================
fprintf("\n===== HASIL ANALISIS KONTROL FUZZY MATLAB =====\n");
fprintf("Setpoint                  : %.2f RPM\n", sp);
fprintf("Durasi pengujian          : %.2f s\n", duration_s);
fprintf("Sampling time             : %.2f s\n", Ts);
fprintf("RPM maksimum              : %.2f RPM\n", rpm_max);
fprintf("Overshoot                 : %.2f RPM\n", overshoot_rpm);
fprintf("Overshoot                 : %.2f %%\n", overshoot_percent);
fprintf("Rise time                 : %.2f s\n", rise_time);
fprintf("Settling time ±5%%         : %.2f s\n", settling_time);
fprintf("Rata-rata RPM steady-state: %.2f RPM\n", rpm_ss_mean);
fprintf("Std RPM steady-state      : %.2f RPM\n", rpm_ss_std);
fprintf("Steady-state error        : %.2f RPM\n", steady_state_error);
fprintf("IAE                       : %.2f\n", IAE);

%% ============================
%  Simpan Ringkasan Analisis
% ============================
summary = table( ...
    sp, duration_s, Ts, rpm_max, overshoot_rpm, overshoot_percent, ...
    rise_time, settling_time, rpm_ss_mean, rpm_ss_std, ...
    steady_state_error, IAE, ...
    'VariableNames', { ...
    'Setpoint_RPM', ...
    'Duration_s', ...
    'Sampling_Time_s', ...
    'RPM_Max', ...
    'Overshoot_RPM', ...
    'Overshoot_Percent', ...
    'Rise_Time_s', ...
    'Settling_Time_s', ...
    'RPM_SS_Mean', ...
    'RPM_SS_Std', ...
    'Steady_State_Error', ...
    'IAE'});

summary_filename = "summary_fuzzy_matlab_controller_" + ...
    string(datetime("now", "Format", "yyyyMMdd_HHmmss")) + ".csv";

writetable(summary, summary_filename);

fprintf("\nRingkasan analisis disimpan sebagai: %s\n", summary_filename);

%% ============================
%  Tampilkan 10 Data Pertama
% ============================
disp("10 data pertama:");
disp(data(1:min(10,height(data)),:));

%% ============================
%  Fungsi Membuat FIS
% ============================
function fis = createMotorFIS()

    fis = mamfis(Name = "FuzzyMotorDC");

    %% Input 1: Error [-100, 100]
    fis = addInput(fis, [-100 100], "Name", "error");
    fis = addMF(fis, "error", "trapmf", [-100 -100 -100 0], Name = "N");
    fis = addMF(fis, "error", "trimf", [-100 0 100], Name = "Z");
    fis = addMF(fis, "error", "trapmf", [0 100 100 100], Name = "P");

    %% Input 2: Delta Error [-50, 50]
    fis = addInput(fis, [-50 50], "Name", "delta_error");
    fis = addMF(fis, "delta_error", "trapmf", [-50 -50 -50 0], Name = "N");
    fis = addMF(fis, "delta_error", "trimf", [-50 0 50], Name = "Z");
    fis = addMF(fis, "delta_error", "trapmf", [0 50 50 50], Name = "P");

    %% Output: Delta PWM [-25, 25]
    fis = addOutput(fis, [-25 25], "Name", "delta_pwm");
    fis = addMF(fis, "delta_pwm", "trapmf", [-25 -25 -25 0], Name = "N");
    fis = addMF(fis, "delta_pwm", "trimf", [-10 0 10], Name = "Z");
    fis = addMF(fis, "delta_pwm", "trapmf", [0 25 25 25], Name = "P");

    %% Rule Base
    % Format:
    % [error delta_error delta_pwm weight AND/OR]
    % 1 = N, 2 = Z, 3 = P
    % AND = 1

    rules = [
        1 1 1 1 1;
        1 2 1 1 1;
        1 3 2 1 1;

        2 1 1 1 1;
        2 2 2 1 1;
        2 3 3 1 1;

        3 1 2 1 1;
        3 2 3 1 1;
        3 3 3 1 1;
    ];

    fis = addRule(fis, rules);

end
