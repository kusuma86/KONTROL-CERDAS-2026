// =====================================================
// NEURAL CONTROLLER UNTUK KONTROL RPM MOTOR DC
// ESP32 + L298N + Encoder
// Model NN meniru fuzzy controller:
// [error, delta_error] -> delta_pwm
// =====================================================

// =====================================================
// KONFIGURASI PIN
// =====================================================
#define EncoderPinA 18
#define EncoderPinB 21
#define ForwardPin  27
#define BackwardPin 26
#define EnablePin   25

const int PPR = 330;  // Pulses per revolution

// =====================================================
// VARIABEL ENCODER DAN KONTROL
// =====================================================
volatile long encoderValue = 0;
unsigned long lastMillis = 0;
long lastEncoder = 0;

float setpoint = 0;
float actual_rpm = 0;

float error = 0;
float last_error = 0;
float delta_error = 0;

float pwmValue = 0;
float delta_pwm = 0;

float set_atas = 150.0;
float set_bawah = 0.0;

// =====================================================
// PARAMETER NORMALISASI DARI MATLAB
// mapminmax input: error dan delta_error
// =====================================================
const float X_MIN[2] = {
  -29.0900,
  -29.0900
};

const float X_MAX[2] = {
  20.0000,
  20.0000
};

// Target delta_pwm
const float T_MIN = -9.8800;
const float T_MAX =  7.3600;

// =====================================================
// BOBOT DAN BIAS HASIL TRAINING MATLAB
// Arsitektur: 2 input -> 8 hidden neuron -> 1 output
// Hidden activation: tansig
// Output activation: purelin
// =====================================================

const int N_INPUT  = 2;
const int N_HIDDEN = 8;

// Bobot input ke hidden layer
const float IW[N_HIDDEN][N_INPUT] = {
  {-2.3075, -2.9047},
  { 4.0815,  2.1308},
  {-5.3365, -4.4035},
  {-4.6038,  0.8418},
  {-3.8802, -1.6733},
  {-2.8520,  3.7704},
  {-1.9725, -3.5783},
  {-2.3901,  2.7970}
};

// Bias hidden layer
const float b1[N_HIDDEN] = {
   4.3064,
  -2.7635,
   1.7656,
   0.8163,
  -0.5640,
  -2.1746,
  -2.5230,
  -4.2992
};

// Bobot hidden ke output
const float LW[N_HIDDEN] = {
  -0.2549,
   0.2517,
  -0.2142,
   0.1974,
  -0.2448,
   0.1881,
  -0.2830,
   0.1367
};

// Bias output
const float b2 = 0.4116;

// =====================================================
// FUNGSI AKTIVASI DAN NORMALISASI
// =====================================================

float tansig(float x) {
  return 2.0 / (1.0 + exp(-2.0 * x)) - 1.0;
}

// Normalisasi seperti mapminmax MATLAB ke range [-1, 1]
float normalizeInput(float x, float xmin, float xmax) {
  if (xmax == xmin) return 0;
  return 2.0 * (x - xmin) / (xmax - xmin) - 1.0;
}

// Denormalisasi output dari range [-1, 1] ke skala delta_pwm asli
float denormalizeOutput(float y_norm, float xmin, float xmax) {
  return ((y_norm + 1.0) * (xmax - xmin) / 2.0) + xmin;
}

// =====================================================
// FUNGSI FEEDFORWARD NEURAL NETWORK
// Input : error dan delta_error
// Output: delta_pwm
// =====================================================

float neuralController(float errorInput, float deltaErrorInput) {
  float x[2];

  // Batasi input sesuai range training MATLAB
  errorInput      = constrain(errorInput, X_MIN[0], X_MAX[0]);
  deltaErrorInput = constrain(deltaErrorInput, X_MIN[1], X_MAX[1]);

  // Normalisasi input
  x[0] = normalizeInput(errorInput,      X_MIN[0], X_MAX[0]);
  x[1] = normalizeInput(deltaErrorInput, X_MIN[1], X_MAX[1]);

  // Hidden layer
  float hidden[N_HIDDEN];

  for (int i = 0; i < N_HIDDEN; i++) {
    float sum = 0;

    for (int j = 0; j < N_INPUT; j++) {
      sum += IW[i][j] * x[j];
    }

    sum += b1[i];
    hidden[i] = tansig(sum);
  }

  // Output layer
  float y_norm = 0;

  for (int i = 0; i < N_HIDDEN; i++) {
    y_norm += LW[i] * hidden[i];
  }

  y_norm += b2;

  // Batasi output normalisasi agar tidak terlalu liar
  y_norm = constrain(y_norm, -1.0, 1.0);

  // Denormalisasi ke delta_pwm asli
  float y = denormalizeOutput(y_norm, T_MIN, T_MAX);

  return y;
}

// =====================================================
// INTERRUPT ENCODER
// Sama seperti skrip fuzzy sebelumnya
// =====================================================

void IRAM_ATTR updateEncoder() {
  if (digitalRead(EncoderPinA) == digitalRead(EncoderPinB)) {
    encoderValue++;
  } else {
    encoderValue--;
  }
}

// =====================================================
// SETUP
// =====================================================

void setup() {
  Serial.begin(115200);

  pinMode(ForwardPin, OUTPUT);
  pinMode(BackwardPin, OUTPUT);
  pinMode(EnablePin, OUTPUT);

  pinMode(EncoderPinA, INPUT_PULLUP);
  pinMode(EncoderPinB, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(EncoderPinA), updateEncoder, RISING);

  digitalWrite(ForwardPin, HIGH);
  digitalWrite(BackwardPin, LOW);
  analogWrite(EnablePin, 0);

  Serial.println("Masukkan Setpoint RPM, contoh: 20, 50, 100, 150 lalu tekan Enter:");
}

// =====================================================
// LOOP UTAMA
// =====================================================

void loop() {
  // =====================================================
  // 1. Baca input setpoint dari Serial
  // =====================================================
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.length() > 0) {
      setpoint = input.toFloat();

      // Batasi setpoint sesuai target eksperimen
      setpoint = constrain(setpoint, 0, 150);

      Serial.print("Setpoint diubah ke: ");
      Serial.println(setpoint);

      // Optional: reset error agar transisi lebih bersih
      last_error = 0;
    }
  }

  // =====================================================
  // 2. Hitung RPM setiap 100 ms
  // =====================================================
  if (millis() - lastMillis >= 100) {
    unsigned long now = millis();
    float dt = (now - lastMillis) / 1000.0;
    lastMillis = now;

    long currentEncoder;

    noInterrupts();
    currentEncoder = encoderValue;
    interrupts();

    long deltaEncoder = currentEncoder - lastEncoder;
    lastEncoder = currentEncoder;

    // Hitung RPM
    // Gunakan abs agar RPM selalu positif
    actual_rpm = (abs(deltaEncoder) / (float)PPR) * (60.0 / dt);

    // =====================================================
    // 3. Hitung error dan delta_error
    // =====================================================
    error = setpoint - actual_rpm;
    delta_error = error - last_error;

    // =====================================================
    // 4. Neural Network Controller
    // =====================================================
    delta_pwm = neuralController(error, delta_error);

    // =====================================================
    // 5. Update PWM incremental
    // Sama seperti fuzzy:
    // pwmValue += delta_pwm;
    // =====================================================
    pwmValue += delta_pwm;
    pwmValue = constrain(pwmValue, 0, 255);

    digitalWrite(ForwardPin, HIGH);
    digitalWrite(BackwardPin, LOW);
    analogWrite(EnablePin, (int)pwmValue);

    // =====================================================
    // 6. Serial Plotter
    // Format dibuat mirip skrip fuzzy sebelumnya
    // =====================================================
    Serial.print(actual_rpm);
    Serial.print("\t");
    Serial.print(set_bawah);
    Serial.print("\t");
    Serial.println(set_atas);

    // Kalau ingin debug lengkap, aktifkan bagian ini
    /*
    Serial.print("SP:");
    Serial.print(setpoint);
    Serial.print("\tRPM:");
    Serial.print(actual_rpm);
    Serial.print("\tE:");
    Serial.print(error);
    Serial.print("\tdE:");
    Serial.print(delta_error);
    Serial.print("\tdPWM:");
    Serial.print(delta_pwm);
    Serial.print("\tPWM:");
    Serial.println(pwmValue);
    */

    // Simpan error sebelumnya
    last_error = error;
  }
}
