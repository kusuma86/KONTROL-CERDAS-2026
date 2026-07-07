#include <Arduino.h>
#include <math.h>
#include <esp_system.h>

// =====================================================
// ARSITEKTUR JARINGAN
// 2 input -> 3 hidden layer 1 -> 2 hidden layer 2 -> 1 output
// =====================================================

#define N_INPUT 2
#define N_H1 3
#define N_H2 2
#define N_OUTPUT 1

// =====================================================
// PARAMETER TRAINING
// =====================================================

float learningRate = 0.8;
int maxEpoch = 1200;
float targetMSE = 0.001;

// =====================================================
// DATASET OR
// Input akan diproses menjadi -1 dan 1 seperti mapminmax MATLAB
// =====================================================

float X[4][2] = {
  {0, 0},
  {0, 1},
  {1, 0},
  {1, 1}
};

float T[4] = {
  0,
  1,
  1,
  1
};

// =====================================================
// BOBOT DAN BIAS
// =====================================================

// Input -> Hidden Layer 1
float W1[N_H1][N_INPUT];
float b1[N_H1];

// Hidden Layer 1 -> Hidden Layer 2
float W2[N_H2][N_H1];
float b2[N_H2];

// Hidden Layer 2 -> Output
float W3[N_OUTPUT][N_H2];
float b3[N_OUTPUT];

// Aktivasi tiap layer
float a1[N_H1];
float a2[N_H2];
float y;

// Delta untuk backpropagation
float delta3[N_OUTPUT];
float delta2[N_H2];
float delta1[N_H1];

// Random seed
uint32_t trainingSeed = 0;

// =====================================================
// FUNGSI AKTIVASI
// =====================================================

float logsig(float n) {
  return 1.0 / (1.0 + exp(-n));
}

float dlogsig_from_output(float a) {
  return a * (1.0 - a);
}

// =====================================================
// RANDOM SEED BERBEDA UNTUK SETIAP BOARD / RUN
// =====================================================

uint32_t generateRandomSeed() {
  uint32_t seed = esp_random();
  seed ^= micros();
  seed ^= (uint32_t)(ESP.getEfuseMac() & 0xFFFFFFFF);

  return seed;
}

// =====================================================
// RANDOM KECIL
// =====================================================

float randomWeight() {
  return random(-1000, 1000) / 1000.0;
}

// =====================================================
// INISIALISASI BOBOT
// =====================================================

void initializeWeights() {
  trainingSeed = generateRandomSeed();
  randomSeed(trainingSeed);

  Serial.print("Random seed yang digunakan: ");
  Serial.println(trainingSeed);
  Serial.println();

  for (int i = 0; i < N_H1; i++) {
    for (int j = 0; j < N_INPUT; j++) {
      W1[i][j] = randomWeight();
    }
    b1[i] = randomWeight();
  }

  for (int i = 0; i < N_H2; i++) {
    for (int j = 0; j < N_H1; j++) {
      W2[i][j] = randomWeight();
    }
    b2[i] = randomWeight();
  }

  for (int i = 0; i < N_OUTPUT; i++) {
    for (int j = 0; j < N_H2; j++) {
      W3[i][j] = randomWeight();
    }
    b3[i] = randomWeight();
  }
}

// =====================================================
// PREPROCESSING INPUT
// 0 -> -1
// 1 ->  1
// =====================================================

float preprocessInput(float x) {
  return 2.0 * x - 1.0;
}

// =====================================================
// FEEDFORWARD
// =====================================================

float feedforward(float x1, float x2) {
  float xp[N_INPUT];

  xp[0] = preprocessInput(x1);
  xp[1] = preprocessInput(x2);

  // Hidden Layer 1
  for (int i = 0; i < N_H1; i++) {
    float n = b1[i];

    for (int j = 0; j < N_INPUT; j++) {
      n += W1[i][j] * xp[j];
    }

    a1[i] = logsig(n);
  }

  // Hidden Layer 2
  for (int i = 0; i < N_H2; i++) {
    float n = b2[i];

    for (int j = 0; j < N_H1; j++) {
      n += W2[i][j] * a1[j];
    }

    a2[i] = logsig(n);
  }

  // Output Layer
  float n = b3[0];

  for (int j = 0; j < N_H2; j++) {
    n += W3[0][j] * a2[j];
  }

  y = logsig(n);

  return y;
}

// =====================================================
// BACKPROPAGATION UNTUK 1 DATA
// =====================================================

void trainOneSample(float x1, float x2, float target) {
  float xp[N_INPUT];

  xp[0] = preprocessInput(x1);
  xp[1] = preprocessInput(x2);

  // Feedforward dulu
  float output = feedforward(x1, x2);

  // Error output
  float error = target - output;

  // Delta output layer
  delta3[0] = error * dlogsig_from_output(output);

  // Delta hidden layer 2
  for (int i = 0; i < N_H2; i++) {
    float sum = 0.0;

    for (int k = 0; k < N_OUTPUT; k++) {
      sum += delta3[k] * W3[k][i];
    }

    delta2[i] = dlogsig_from_output(a2[i]) * sum;
  }

  // Delta hidden layer 1
  for (int i = 0; i < N_H1; i++) {
    float sum = 0.0;

    for (int k = 0; k < N_H2; k++) {
      sum += delta2[k] * W2[k][i];
    }

    delta1[i] = dlogsig_from_output(a1[i]) * sum;
  }

  // Update bobot Hidden 2 -> Output
  for (int i = 0; i < N_OUTPUT; i++) {
    for (int j = 0; j < N_H2; j++) {
      W3[i][j] += learningRate * delta3[i] * a2[j];
    }

    b3[i] += learningRate * delta3[i];
  }

  // Update bobot Hidden 1 -> Hidden 2
  for (int i = 0; i < N_H2; i++) {
    for (int j = 0; j < N_H1; j++) {
      W2[i][j] += learningRate * delta2[i] * a1[j];
    }

    b2[i] += learningRate * delta2[i];
  }

  // Update bobot Input -> Hidden 1
  for (int i = 0; i < N_H1; i++) {
    for (int j = 0; j < N_INPUT; j++) {
      W1[i][j] += learningRate * delta1[i] * xp[j];
    }

    b1[i] += learningRate * delta1[i];
  }
}

// =====================================================
// HITUNG MSE
// =====================================================

float calculateMSE() {
  float mse = 0.0;

  for (int i = 0; i < 4; i++) {
    float output = feedforward(X[i][0], X[i][1]);
    float error = T[i] - output;
    mse += error * error;
  }

  mse = mse / 4.0;

  return mse;
}

// =====================================================
// TRAINING
// =====================================================

void trainNetwork() {
  Serial.println("Mulai training on-device...");
  Serial.println();

  bool targetReached = false;

  for (int epoch = 1; epoch <= maxEpoch; epoch++) {
    for (int i = 0; i < 4; i++) {
      trainOneSample(X[i][0], X[i][1], T[i]);
    }

    float mse = calculateMSE();

    if (epoch % 100 == 0) {
      Serial.print("Epoch: ");
      Serial.print(epoch);
      Serial.print(" | MSE: ");
      Serial.println(mse, 8);
    }

    if (mse < targetMSE) {
      Serial.print("Training berhenti pada epoch: ");
      Serial.println(epoch);
      Serial.print("MSE akhir: ");
      Serial.println(mse, 8);
      Serial.println();

      targetReached = true;
      break;
    }
  }

  if (!targetReached) {
    float mse = calculateMSE();

    Serial.println();
    Serial.println("Training mencapai maxEpoch.");
    Serial.print("MSE akhir: ");
    Serial.println(mse, 8);
    Serial.println();
  }
}

// =====================================================
// CETAK HASIL PREDIKSI
// =====================================================

void testNetwork() {
  Serial.println("Hasil pengujian fungsi OR:");
  Serial.println("x1\tx2\tTarget\tOutput\t\tClass");

  for (int i = 0; i < 4; i++) {
    float output = feedforward(X[i][0], X[i][1]);

    int kelas;

    if (output > 0.5) {
      kelas = 1;
    } else {
      kelas = 0;
    }

    Serial.print((int)X[i][0]);
    Serial.print("\t");
    Serial.print((int)X[i][1]);
    Serial.print("\t");
    Serial.print((int)T[i]);
    Serial.print("\t");
    Serial.print(output, 6);
    Serial.print("\t");
    Serial.println(kelas);
  }

  Serial.println();
}

// =====================================================
// CETAK BOBOT HASIL TRAINING
// =====================================================

void printWeights() {
  Serial.println("Bobot hasil training:");
  Serial.println();

  Serial.println("W1 = ");
  for (int i = 0; i < N_H1; i++) {
    for (int j = 0; j < N_INPUT; j++) {
      Serial.print(W1[i][j], 6);
      Serial.print("\t");
    }
    Serial.println();
  }

  Serial.println();
  Serial.println("b1 = ");
  for (int i = 0; i < N_H1; i++) {
    Serial.println(b1[i], 6);
  }

  Serial.println();
  Serial.println("W2 = ");
  for (int i = 0; i < N_H2; i++) {
    for (int j = 0; j < N_H1; j++) {
      Serial.print(W2[i][j], 6);
      Serial.print("\t");
    }
    Serial.println();
  }

  Serial.println();
  Serial.println("b2 = ");
  for (int i = 0; i < N_H2; i++) {
    Serial.println(b2[i], 6);
  }

  Serial.println();
  Serial.println("W3 = ");
  for (int i = 0; i < N_OUTPUT; i++) {
    for (int j = 0; j < N_H2; j++) {
      Serial.print(W3[i][j], 6);
      Serial.print("\t");
    }
    Serial.println();
  }

  Serial.println();
  Serial.println("b3 = ");
  for (int i = 0; i < N_OUTPUT; i++) {
    Serial.println(b3[i], 6);
  }

  Serial.println();
}

// =====================================================
// SETUP
// =====================================================

void setup() {
  Serial.begin(115200);
  delay(1500);

  Serial.println();
  Serial.println("========================================");
  Serial.println("ON-DEVICE TRAINING Neural Network OR");
  Serial.println("Arsitektur: 2 - 3 - 2 - 1");
  Serial.println("Metode: Backpropagation Gradient Descent");
  Serial.println("========================================");
  Serial.println();

  initializeWeights();

  Serial.println("Sebelum training:");
  testNetwork();

  trainNetwork();

  Serial.println("Setelah training:");
  testNetwork();

  printWeights();
}

// =====================================================
// LOOP
// =====================================================

void loop() {
  // Kosong.
  // Training dilakukan sekali di setup().
}
