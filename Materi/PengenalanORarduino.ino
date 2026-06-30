#include <Arduino.h>
#include <math.h>

// ==========================================
// Fungsi aktivasi logsig MATLAB
// logsig(n) = 1 / (1 + exp(-n))
// ==========================================
double logsig(double n) {
  return 1.0 / (1.0 + exp(-n));
}


// ==========================================
// Bobot input ke hidden layer 1
// net.IW{1,1}
// Ukuran: 3 neuron x 2 input
// ==========================================
double IW1[3][2] = {
  {  4.9161,   3.5268 },
  {  6.2889,  -0.0578 },
  { -4.4555,  -6.2473 }
};


// ==========================================
// Bias hidden layer 1
// net.b{1}
// Ukuran: 3 x 1
// ==========================================
double b1[3] = {
  -4.5069,
  -4.0611,
  -5.2813
};


// ==========================================
// Bobot hidden layer 1 ke hidden layer 2
// net.LW{2,1}
// Ukuran: 2 neuron x 3 neuron
// ==========================================
double LW21[2][3] = {
  { -8.8759,  -2.5435,   2.5400 },
  {  2.2380,  -5.4801,  11.2255 }
};


// ==========================================
// Bias hidden layer 2
// net.b{2}
// Ukuran: 2 x 1
// ==========================================
double b2[2] = {
   1.3484,
  -5.8303
};


// ==========================================
// Bobot hidden layer 2 ke output
// net.LW{3,2}
// Ukuran: 1 neuron x 2 neuron
// ==========================================
double LW32[2] = {
  -3.2070,
 -27.7481
};


// ==========================================
// Bias output layer
// net.b{3}
// Ukuran: 1 x 1
// ==========================================
double b3 = 10.0464;


// ==========================================
// Fungsi feedforward neural network
// Input asli: x1 dan x2 bernilai 0 atau 1
// Output: nilai Y seperti hasil MATLAB net(X)
// ==========================================
double neuralNetworkOR(double x1, double x2) {

  // ------------------------------------------
  // 1. Preprocessing input seperti MATLAB mapminmax
  // Karena data input hanya 0 dan 1:
  // 0 menjadi -1
  // 1 menjadi  1
  // ------------------------------------------
  double xp[2];

  xp[0] = 2.0 * x1 - 1.0;
  xp[1] = 2.0 * x2 - 1.0;


  // ------------------------------------------
  // 2. Hidden layer 1
  // a1 = logsig(IW1 * xp + b1)
  // ------------------------------------------
  double a1[3];

  for (int i = 0; i < 3; i++) {
    double n = b1[i];

    for (int j = 0; j < 2; j++) {
      n += IW1[i][j] * xp[j];
    }

    a1[i] = logsig(n);
  }


  // ------------------------------------------
  // 3. Hidden layer 2
  // a2 = logsig(LW21 * a1 + b2)
  // ------------------------------------------
  double a2[2];

  for (int i = 0; i < 2; i++) {
    double n = b2[i];

    for (int j = 0; j < 3; j++) {
      n += LW21[i][j] * a1[j];
    }

    a2[i] = logsig(n);
  }


  // ------------------------------------------
  // 4. Output layer
  // a3 = logsig(LW32 * a2 + b3)
  // ------------------------------------------
  double n3 = b3;

  for (int j = 0; j < 2; j++) {
    n3 += LW32[j] * a2[j];
  }

  double a3 = logsig(n3);


  // ------------------------------------------
  // 5. Postprocessing output seperti MATLAB mapminmax reverse
  // MATLAB mengembalikan output ke skala target 0 sampai 1
  // y = (a3 + 1) / 2
  // ------------------------------------------
  double y = (a3 + 1.0) / 2.0;

  return y;
}


// ==========================================
// Setup Arduino
// ==========================================
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("Neural Network OR - Feedforward Only");
  Serial.println("Arsitektur: 2 input - 3 hidden - 2 hidden - 1 output");
  Serial.println();

  double X[4][2] = {
    {0, 0},
    {0, 1},
    {1, 0},
    {1, 1}
  };

  Serial.println("x1\tx2\tY\t\tClass");

  for (int i = 0; i < 4; i++) {
    double x1 = X[i][0];
    double x2 = X[i][1];

    double Y = neuralNetworkOR(x1, x2);

    int Y_class;

    if (Y > 0.5) {
      Y_class = 1;
    } else {
      Y_class = 0;
    }

    Serial.print((int)x1);
    Serial.print("\t");
    Serial.print((int)x2);
    Serial.print("\t");
    Serial.print(Y, 6);
    Serial.print("\t");
    Serial.println(Y_class);
  }
}


void loop() {
  // Kosong, karena pengujian hanya dilakukan sekali di setup()
}
