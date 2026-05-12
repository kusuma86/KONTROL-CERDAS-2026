#include <Fuzzy.h>


// Konfigurasi Pin & Variabel

#define EncoderPinA 18
#define EncoderPinB 21
#define ForwardPin 27
#define BackwardPin 26
#define EnablePin 25
const int PPR = 330;  // Pulses per revolution

volatile long encoderValue = 0;
unsigned long lastMillis = 0;
long lastEncoder = 0;

float setpoint = 0;
float actual_rpm = 0;
float error = 0;
float last_error = 0;
float delta_error = 0;
float pwmValue = 0;
float set_atas = 150.0;
float set_bawah = 0.0;


// Fuzzy Logic Setup

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
  // Input 1
  FuzzyInput *in_error = new FuzzyInput(1);
  in_error->addFuzzySet(error_N);
  in_error->addFuzzySet(error_Z);
  in_error->addFuzzySet(error_P);
  fuzzy->addFuzzyInput(in_error);

  // Input 2
  FuzzyInput *in_dErr = new FuzzyInput(2);
  in_dErr->addFuzzySet(derr_N);
  in_dErr->addFuzzySet(derr_Z);
  in_dErr->addFuzzySet(derr_P);
  fuzzy->addFuzzyInput(in_dErr);

  // Output
  FuzzyOutput *out_pwm = new FuzzyOutput(1);
  out_pwm->addFuzzySet(dpwm_N);
  out_pwm->addFuzzySet(dpwm_Z);
  out_pwm->addFuzzySet(dpwm_P);
  fuzzy->addFuzzyOutput(out_pwm);

  // Rules (dari FIS MATLAB)
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


// Encoder Interrupt

void IRAM_ATTR updateEncoder() {
  if (digitalRead(EncoderPinA) == digitalRead(EncoderPinB)) encoderValue++;
  else encoderValue--;
}


// Setup

void setup() {
  Serial.begin(115200);

  pinMode(ForwardPin, OUTPUT);
  pinMode(BackwardPin, OUTPUT);
  pinMode(EnablePin, OUTPUT);
  pinMode(EncoderPinA, INPUT);
  pinMode(EncoderPinB, INPUT);

  attachInterrupt(digitalPinToInterrupt(EncoderPinA), updateEncoder, RISING);

  setupFuzzy();

  Serial.println("Masukkan Setpoint RPM (contoh: 50) lalu tekan Enter:");
}


// Loop utama

void loop() {
  // --- 1. Baca input setpoint dari Serial ---
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input.length() > 0) {
      setpoint = input.toFloat();
      Serial.print("Setpoint diubah ke: ");
      Serial.println(setpoint);
    }
  }

  // --- 2. Hitung RPM aktual setiap 100ms ---
  if (millis() - lastMillis >= 100) {
    lastMillis = millis();

    long delta = encoderValue - lastEncoder;
    lastEncoder = encoderValue;
    actual_rpm = (delta / (float)PPR) * 60.0 * 10;  // RPM

    // --- 3. Hitung error & delta_error ---
    error = setpoint - actual_rpm;
    delta_error = error - last_error;

    // --- 4. Fuzzy Logic Inferensi ---
    fuzzy->setInput(1, error);
    fuzzy->setInput(2, delta_error);
    fuzzy->fuzzify();
    float delta_pwm = fuzzy->defuzzify(1);

    // --- 5. Update PWM & kendali motor ---
    pwmValue += delta_pwm;
    pwmValue = constrain(pwmValue, 0, 255);

    digitalWrite(ForwardPin, HIGH);
    digitalWrite(BackwardPin, LOW);
    analogWrite(EnablePin, pwmValue);

    // --- 6. Plot hasil ke Serial Plotter ---
    Serial.print(actual_rpm);
    Serial.print("\t");
    Serial.print(set_bawah);
    Serial.print("\t");
    Serial.println(set_atas);
    // Serial.print(",");
    // Serial.print(error);
    // Serial.print(",");
    //Serial.println(pwmValue);

    // Simpan error untuk iterasi berikut
    last_error = error;
  }
}
