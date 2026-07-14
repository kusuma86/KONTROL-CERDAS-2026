#include <Fuzzy.h>

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
long lastEncoder = 0;

unsigned long lastControlMillis = 0;
unsigned long lastStepMillis = 0;

const unsigned long controlInterval = 100;    // sampling 100 ms
const unsigned long stepInterval    = 10000;  // tiap setpoint bertahan 10 detik

float setpoint = 20;
float actual_rpm = 0;

float error = 0;
float last_error = 0;
float delta_error = 0;

float pwmValue = 0;
float delta_pwm = 0;

// =====================================================
// SETPOINT SEQUENCE UNTUK DATASET
// =====================================================
// Kombinasi dibuat naik, turun, dan loncat agar dataset lebih kaya
float setpointSequence[] = {
  20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150,
  140, 130, 120, 110, 100, 90, 80, 70, 60, 50, 40, 30, 20,
  50, 100, 70, 130, 40, 120, 90, 150, 60, 110, 30, 140, 80, 20
};

int totalSteps = sizeof(setpointSequence) / sizeof(setpointSequence[0]);
int stepIndex = 0;

// Batas tampilan jika ingin dipakai Serial Plotter
float set_atas = 160.0;
float set_bawah = 0.0;

// =====================================================
// FUZZY LOGIC SETUP
// =====================================================
Fuzzy *fuzzy = new Fuzzy();

// Input: error [-100, 100]
FuzzySet *error_N = new FuzzySet(-100, -100, -100, 0);
FuzzySet *error_Z = new FuzzySet(-100, 0, 0, 100);
FuzzySet *error_P = new FuzzySet(0, 100, 100, 100);

// Input: delta_error [-50, 50]
FuzzySet *derr_N = new FuzzySet(-50, -50, -50, 0);
FuzzySet *derr_Z = new FuzzySet(-50, 0, 0, 50);
FuzzySet *derr_P = new FuzzySet(0, 50, 50, 50);

// Output: delta_pwm [-25, 25]
FuzzySet *dpwm_N = new FuzzySet(-25, -25, -25, 0);
FuzzySet *dpwm_Z = new FuzzySet(-10, 0, 0, 10);
FuzzySet *dpwm_P = new FuzzySet(0, 25, 25, 25);

void setupFuzzy() {
  // Input 1: error
  FuzzyInput *in_error = new FuzzyInput(1);
  in_error->addFuzzySet(error_N);
  in_error->addFuzzySet(error_Z);
  in_error->addFuzzySet(error_P);
  fuzzy->addFuzzyInput(in_error);

  // Input 2: delta_error
  FuzzyInput *in_dErr = new FuzzyInput(2);
  in_dErr->addFuzzySet(derr_N);
  in_dErr->addFuzzySet(derr_Z);
  in_dErr->addFuzzySet(derr_P);
  fuzzy->addFuzzyInput(in_dErr);

  // Output: delta_pwm
  FuzzyOutput *out_pwm = new FuzzyOutput(1);
  out_pwm->addFuzzySet(dpwm_N);
  out_pwm->addFuzzySet(dpwm_Z);
  out_pwm->addFuzzySet(dpwm_P);
  fuzzy->addFuzzyOutput(out_pwm);

  // Rules
  int ruleNum = 1;

  auto makeRule = [&](FuzzySet* e, FuzzySet* de, FuzzySet* dp) {
    FuzzyRuleAntecedent *r = new FuzzyRuleAntecedent();
    r->joinWithAND(e, de);

    FuzzyRuleConsequent *c = new FuzzyRuleConsequent();
    c->addOutput(dp);

    fuzzy->addFuzzyRule(new FuzzyRule(ruleNum++, r, c));
  };

  makeRule(error_N, derr_N, dpwm_N);
  makeRule(error_N, derr_Z, dpwm_N);
  makeRule(error_N, derr_P, dpwm_Z);

  makeRule(error_Z, derr_N, dpwm_N);
  makeRule(error_Z, derr_Z, dpwm_Z);
  makeRule(error_Z, derr_P, dpwm_P);

  makeRule(error_P, derr_N, dpwm_Z);
  makeRule(error_P, derr_Z, dpwm_P);
  makeRule(error_P, derr_P, dpwm_P);
}

// =====================================================
// INTERRUPT ENCODER
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

  setupFuzzy();

  digitalWrite(ForwardPin, HIGH);
  digitalWrite(BackwardPin, LOW);
  analogWrite(EnablePin, 0);

  setpoint = setpointSequence[0];

  lastControlMillis = millis();
  lastStepMillis = millis();

  // Header CSV untuk dataset
  Serial.println("time_ms,setpoint,rpm,error,delta_error,delta_pwm,pwm,delta_encoder,encoder_total");
}

// =====================================================
// LOOP UTAMA
// =====================================================
void loop() {
  unsigned long now = millis();

  // =====================================================
  // 1. Ganti setpoint otomatis setiap stepInterval
  // =====================================================
  if (now - lastStepMillis >= stepInterval) {
    lastStepMillis = now;

    stepIndex++;

    if (stepIndex >= totalSteps) {
      stepIndex = 0;
    }

    setpoint = setpointSequence[stepIndex];

    // Saat setpoint berubah, last_error disesuaikan agar delta_error tidak terlalu ekstrem
    last_error = error;
  }

  // =====================================================
  // 2. Hitung RPM dan jalankan fuzzy setiap 100 ms
  // =====================================================
  if (now - lastControlMillis >= controlInterval) {
    float dt = (now - lastControlMillis) / 1000.0;
    lastControlMillis = now;

    long currentEncoder;

    noInterrupts();
    currentEncoder = encoderValue;
    interrupts();

    long deltaEncoder = currentEncoder - lastEncoder;
    lastEncoder = currentEncoder;

    // RPM
    // Jika ingin RPM selalu positif, gunakan abs(deltaEncoder)
    actual_rpm = (abs(deltaEncoder) / (float)PPR) * (60.0 / dt);

    // Error dan delta error
    error = setpoint - actual_rpm;
    delta_error = error - last_error;

    // Batasi input fuzzy agar tetap dalam universe of discourse
    float error_fuzzy = constrain(error, -100, 100);
    float derror_fuzzy = constrain(delta_error, -50, 50);

    // Fuzzy inference
    fuzzy->setInput(1, error_fuzzy);
    fuzzy->setInput(2, derror_fuzzy);
    fuzzy->fuzzify();

    delta_pwm = fuzzy->defuzzify(1);

    // Update PWM incremental
    pwmValue += delta_pwm;
    pwmValue = constrain(pwmValue, 0, 255);

    // Kendali motor
    digitalWrite(ForwardPin, HIGH);
    digitalWrite(BackwardPin, LOW);
    analogWrite(EnablePin, (int)pwmValue);

    // =====================================================
    // 3. Cetak dataset CSV
    // =====================================================
    Serial.print(now);
    Serial.print(",");
    Serial.print(setpoint);
    Serial.print(",");
    Serial.print(actual_rpm);
    Serial.print(",");
    Serial.print(error);
    Serial.print(",");
    Serial.print(delta_error);
    Serial.print(",");
    Serial.print(delta_pwm);
    Serial.print(",");
    Serial.print(pwmValue);
    Serial.print(",");
    Serial.print(deltaEncoder);
    Serial.print(",");
    Serial.println(currentEncoder);

    // Simpan error sebelumnya
    last_error = error;
  }
}
