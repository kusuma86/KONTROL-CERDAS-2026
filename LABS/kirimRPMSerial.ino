// ============================
// Konfigurasi Pin & Variabel
// ============================
#define EncoderPinA 18
#define EncoderPinB 21
#define ForwardPin 27
#define BackwardPin 26
#define EnablePin 25

const int PPR = 330;

volatile long encoderValue = 0;
unsigned long lastMillis = 0;
long lastEncoder = 0;

float actual_rpm = 0;
int pwmValue = 0;

// ============================
// Encoder Interrupt
// ============================
void IRAM_ATTR updateEncoder() {
  if (digitalRead(EncoderPinA) == digitalRead(EncoderPinB)) {
    encoderValue++;
  } else {
    encoderValue--;
  }
}

// ============================
// Setup
// ============================
void setup() {
  Serial.begin(115200);

  pinMode(ForwardPin, OUTPUT);
  pinMode(BackwardPin, OUTPUT);
  pinMode(EnablePin, OUTPUT);

  pinMode(EncoderPinA, INPUT);
  pinMode(EncoderPinB, INPUT);

  attachInterrupt(digitalPinToInterrupt(EncoderPinA), updateEncoder, RISING);

  digitalWrite(ForwardPin, HIGH);
  digitalWrite(BackwardPin, LOW);
  analogWrite(EnablePin, 0);
}

// ============================
// Loop Utama
// ============================
void loop() {
  // Terima nilai PWM dari MATLAB
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.length() > 0) {
      pwmValue = input.toInt();
      pwmValue = constrain(pwmValue, 0, 255);

      digitalWrite(ForwardPin, HIGH);
      digitalWrite(BackwardPin, LOW);
      analogWrite(EnablePin, pwmValue);
    }
  }

  // Hitung dan kirim RPM setiap 100 ms
  if (millis() - lastMillis >= 100) {
    lastMillis = millis();

    long delta = encoderValue - lastEncoder;
    lastEncoder = encoderValue;

    actual_rpm = (delta / (float)PPR) * 60.0 * 10.0;

    // Kirim hanya RPM ke MATLAB
    Serial.println(actual_rpm);
  }
}
