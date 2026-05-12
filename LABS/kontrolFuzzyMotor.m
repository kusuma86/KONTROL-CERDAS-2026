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
flush(s);

disp("MATLAB Fuzzy Controller siap.");
disp("Kontrol dimulai. Setpoint RPM dapat diubah dari kotak input pada grafik.");

%% ============================
%  Parameter Kontrol
% ============================
setpoint_awal = input("Masukkan setpoint RPM awal: ");

pwmValue = 0;
last_error = 0;

%% ============================
%  Membuat FIS
% ============================
fis = createMotorFIS();

%% ============================
%  Plot Real-Time + Input Setpoint
% ============================
fig = figure("Name", "Fuzzy Motor DC Controller");

h1 = animatedline( ...
    'LineWidth', 1.5, ...
    'Color', 'red', ...
    'MaximumNumPoints', 300);

grid on;
xlabel("Waktu (s)");
ylabel("RPM");
title("Kontrol Kecepatan Motor DC dengan Fuzzy Logic MATLAB");
legend("Actual RPM", "Location", "best");

% Batas tampilan sumbu-x
windowTime = 20;          % menampilkan maksimal 20 detik terakhir
xlim([0 windowTime]);

% Batas tampilan sumbu-y agar grafik tidak autoscale
yMin = 0;
yMax = 150;
ylim([yMin yMax]);

% Simpan setpoint di UserData figure
fig.UserData.setpoint = setpoint_awal;

% Text label
uicontrol("Style", "text", ...
    "String", "Setpoint RPM:", ...
    "Position", [20 20 90 25]);

% Kotak input setpoint
setpointBox = uicontrol("Style", "edit", ...
    "String", num2str(setpoint_awal), ...
    "Position", [115 22 80 25]);

% Tombol update setpoint
uicontrol("Style", "pushbutton", ...
    "String", "Update", ...
    "Position", [205 22 70 25], ...
    "Callback", @(src, event) updateSetpoint(fig, setpointBox));

tic;
lastDisplayTime = 0;

%% ============================
%  Loop Utama Kontrol
% ============================
while isvalid(fig)

    % Baca RPM dari Arduino/ESP32
    data = readline(s);
    actual_rpm = str2double(strtrim(data));

    % Abaikan data jika tidak valid
    if isnan(actual_rpm)
        continue;
    end

    % Ambil setpoint terbaru dari figure
    setpoint = fig.UserData.setpoint;

    % Hitung error dan delta error
    error = setpoint - actual_rpm;
    delta_error = error - last_error;

    % Batasi input agar sesuai range FIS
    error_fis = max(min(error, 100), -100);
    delta_error_fis = max(min(delta_error, 50), -50);

    % Inferensi fuzzy
    delta_pwm = evalfis(fis, [error_fis delta_error_fis]);

    % Update PWM
    pwmValue = pwmValue + delta_pwm;
    pwmValue = max(min(pwmValue, 255), 0);

    % Kirim PWM ke Arduino/ESP32
    writeline(s, num2str(round(pwmValue)));

    % Plot actual RPM
    t = toc;
    addpoints(h1, t, actual_rpm);

    % Tampilan sumbu-x dinamis, maksimal 20 detik terakhir
    if t <= windowTime
        xlim([0 windowTime]);
    else
        xlim([t - windowTime, t]);
    end

    % Kunci batas sumbu-y agar grafik tidak terlihat bergejolak
    ylim([yMin yMax]);

    drawnow limitrate;

    % Tampilkan informasi ringkas di judul grafik
    if t - lastDisplayTime >= 0.5
        title(sprintf("Setpoint: %.1f RPM | Actual: %.1f RPM | PWM: %.0f", ...
            setpoint, actual_rpm, pwmValue));
        lastDisplayTime = t;
    end

    % Simpan error sebelumnya
    last_error = error;
end

%% Jika figure ditutup, hentikan motor
writeline(s, "0");
clear s;

disp("Kontrol dihentikan.");

%% ============================
%  Fungsi Update Setpoint
% ============================
function updateSetpoint(fig, setpointBox)

    newSetpoint = str2double(setpointBox.String);

    if isnan(newSetpoint)
        disp("Setpoint tidak valid.");
        return;
    end

    fig.UserData.setpoint = newSetpoint;

end

%% ============================
%  Fungsi Membuat FIS
% ============================
function fis = createMotorFIS()

    fis = mamfis(Name = "FuzzyMotorDC");

    %% Input 1: Error [-100, 100]
    fis = addInput(fis, [-100 100], "Name", "error");

    fis = addMF(fis, "error", "trapmf", [-100 -100 -100 0], ...
        Name = "N");

    fis = addMF(fis, "error", "trimf", [-100 0 100], ...
        Name = "Z");

    fis = addMF(fis, "error", "trapmf", [0 100 100 100], ...
        Name = "P");

    %% Input 2: Delta Error [-50, 50]
    fis = addInput(fis, [-50 50], "Name", "delta_error");
    fis = addMF(fis, "delta_error", "trapmf", [-50 -50 -50 0],Name = "N");
    fis = addMF(fis, "delta_error", "trimf", [-50 0 50],Name = "Z");
    fis = addMF(fis, "delta_error", "trapmf", [0 50 50 50],Name = "P");

    %% Output: Delta PWM [-25, 25]
    fis = addOutput(fis, [-25 25], "Name", "delta_pwm");
    fis = addMF(fis, "delta_pwm", "trapmf", [-25 -25 -25 0],Name = "N");
    fis = addMF(fis, "delta_pwm", "trimf", [-10 0 10],Name = "Z");
    fis = addMF(fis, "delta_pwm", "trapmf", [0 25 25 25],Name = "P");

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
