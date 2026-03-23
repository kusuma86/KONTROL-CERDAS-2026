# Dasar-dasar Pemrograman MATLAB

## Daftar Isi

1. [Variabel](#1-variabel)
2. [Operator Aritmatika](#2-operator-aritmatika)
3. [Operator Relasional](#3-operator-relasional)
4. [Array](#4-array)
5. [Akses Elemen Array](#5-akses-elemen-array)
6. [Operasi Dasar Array](#6-operasi-dasar-array)

---

## 1. Variabel

### Penjelasan

Variabel adalah tempat penyimpanan data di memori. Di MATLAB, variabel dibuat dengan memberikan nilai menggunakan tanda sama dengan (=).

### Contoh Kode

**Contoh 1: Membuat variabel numerik**
```matlab
a = 5
```
**Output:**
```
a =
     5
```

**Contoh 2: Membuat variabel dengan nama berbeda**
```matlab
nilai = 100
```
**Output:**
```
nilai =
   100
```

**Contoh 3: Variabel string**
```matlab
nama = 'MATLAB'
```
**Output:**
```
nama =
    'MATLAB'
```

### Soal Latihan

**Latihan 1: Buat variabel suhu dengan nilai 25**
```matlab
suhu = 25
```
**Output:**
```
suhu =
    25
```

**Latihan 2: Buat variabel kota dengan nilai 'Jakarta'**
```matlab
kota = 'Jakarta'
```
**Output:**
```
kota =
    'Jakarta'
```

**Latihan 3: Buat variabel harga dengan nilai 50000**
```matlab
harga = 50000
```
**Output:**
```
harga =
       50000
```

---

## 2. Operator Aritmatika

### Penjelasan

Operator aritmatika digunakan untuk melakukan operasi matematika dasar. MATLAB mendukung penjumlahan (+), pengurangan (-), perkalian (*), pembagian (/), dan perpangkatan (^).

### Contoh Kode

**Contoh 1: Penjumlahan**
```matlab
hasil = 10 + 5
```
**Output:**
```
hasil =
    15
```

**Contoh 2: Pengurangan**
```matlab
selisih = 20 - 8
```
**Output:**
```
selisih =
    12
```

**Contoh 3: Perkalian**
```matlab
kali = 6 * 7
```
**Output:**
```
kali =
    42
```

**Contoh 4: Pembagian**
```matlab
bagi = 50 / 10
```
**Output:**
```
bagi =
     5
```

**Contoh 5: Perpangkatan**
```matlab
pangkat = 2 ^ 3
```
**Output:**
```
pangkat =
     8
```

### Soal Latihan

**Latihan 1: Hitung 15 + 25**
```matlab
jumlah = 15 + 25
```
**Output:**
```
jumlah =
    40
```

**Latihan 2: Hitung 100 - 37**
```matlab
kurang = 100 - 37
```
**Output:**
```
kurang =
    63
```

**Latihan 3: Hitung 8 * 9**
```matlab
hasil_kali = 8 * 9
```
**Output:**
```
hasil_kali =
    72
```

---

## 3. Operator Relasional

### Penjelasan

Operator relasional membandingkan dua nilai dan menghasilkan nilai logika (1 untuk benar, 0 untuk salah). Operator yang tersedia: == (sama dengan), ~= (tidak sama dengan), > (lebih besar), < (lebih kecil), >= (lebih besar sama dengan), <= (lebih kecil sama dengan).

### Contoh Kode

**Contoh 1: Sama dengan**
```matlab
hasil = 5 == 5
```
**Output:**
```
hasil =
  logical
   1
```

**Contoh 2: Tidak sama dengan**
```matlab
cek = 10 ~= 5
```
**Output:**
```
cek =
  logical
   1
```

**Contoh 3: Lebih besar**
```matlab
banding = 8 > 3
```
**Output:**
```
banding =
  logical
   1
```

**Contoh 4: Lebih kecil**
```matlab
kecil = 2 < 7
```
**Output:**
```
kecil =
  logical
   1
```

**Contoh 5: Lebih besar sama dengan**
```matlab
tes = 5 >= 5
```
**Output:**
```
tes =
  logical
   1
```

### Soal Latihan

**Latihan 1: Bandingkan apakah 15 sama dengan 20**
```matlab
sama = 15 == 20
```
**Output:**
```
sama =
  logical
   0
```

**Latihan 2: Cek apakah 30 lebih besar dari 25**
```matlab
lebih = 30 > 25
```
**Output:**
```
lebih =
  logical
   1
```

**Latihan 3: Cek apakah 10 lebih kecil sama dengan 10**
```matlab
kecil_sama = 10 <= 10
```
**Output:**
```
kecil_sama =
  logical
   1
```

---

## 4. Array

### Penjelasan

Array adalah kumpulan data yang disimpan dalam satu variabel. Array dibuat dengan menggunakan tanda kurung siku [ ].

### Contoh Kode

**Contoh 1: Array baris**
```matlab
vektor = [1 2 3 4 5]
```
**Output:**
```
vektor =
     1     2     3     4     5
```

**Contoh 2: Array kolom**
```matlab
kolom = [10; 20; 30]
```
**Output:**
```
kolom =
    10
    20
    30
```

**Contoh 3: Matriks 2x3**
```matlab
matriks = [1 2 3; 4 5 6]
```
**Output:**
```
matriks =
     1     2     3
     4     5     6
```

**Contoh 4: Array dengan jarak tertentu**
```matlab
urutan = 1:5
```
**Output:**
```
urutan =
     1     2     3     4     5
```

**Contoh 5: Array dengan increment**
```matlab
loncat = 0:2:10
```
**Output:**
```
loncat =
     0     2     4     6     8    10
```

### Soal Latihan

**Latihan 1: Buat array [5 10 15 20]**
```matlab
data = [5 10 15 20]
```
**Output:**
```
data =
     5    10    15    20
```

**Latihan 2: Buat matriks 2x2 dengan nilai [1 2; 3 4]**
```matlab
mat = [1 2; 3 4]
```
**Output:**
```
mat =
     1     2
     3     4
```

**Latihan 3: Buat array dari 10 sampai 20**
```matlab
rentang = 10:20
```
**Output:**
```
rentang =
    10    11    12    13    14    15    16    17    18    19    20
```

---

## 5. Akses Elemen Array

### Penjelasan

Elemen array dapat diakses menggunakan indeks dalam tanda kurung. Indeks di MATLAB dimulai dari 1.

### Contoh Kode

**Contoh 1: Akses elemen pertama**
```matlab
A = [10 20 30 40];
elemen1 = A(1)
```
**Output:**
```
elemen1 =
    10
```

**Contoh 2: Akses elemen ketiga**
```matlab
B = [5 15 25 35];
elemen3 = B(3)
```
**Output:**
```
elemen3 =
    25
```

**Contoh 3: Akses elemen matriks**
```matlab
M = [1 2 3; 4 5 6];
nilai = M(2,3)
```
**Output:**
```
nilai =
     6
```

**Contoh 4: Akses beberapa elemen**
```matlab
C = [100 200 300 400];
subset = C(1:3)
```
**Output:**
```
subset =
   100   200   300
```

**Contoh 5: Akses elemen terakhir**
```matlab
D = [7 14 21 28];
akhir = D(end)
```
**Output:**
```
akhir =
    28
```

### Soal Latihan

**Latihan 1: Akses elemen kedua dari [50 60 70 80]**
```matlab
data = [50 60 70 80];
elemen2 = data(2)
```
**Output:**
```
elemen2 =
    60
```

**Latihan 2: Akses elemen baris 1 kolom 2 dari matriks [9 8 7; 6 5 4]**
```matlab
mat = [9 8 7; 6 5 4];
nilai = mat(1,2)
```
**Output:**
```
nilai =
     8
```

**Latihan 3: Akses 3 elemen pertama dari [11 22 33 44 55]**
```matlab
arr = [11 22 33 44 55];
tiga = arr(1:3)
```
**Output:**
```
tiga =
    11    22    33
```

---

## 6. Operasi Dasar Array

### Penjelasan

MATLAB dapat melakukan operasi pada seluruh elemen array sekaligus. Operasi elemen-per-elemen menggunakan titik (.) sebelum operator.

### Contoh Kode

**Contoh 1: Penjumlahan array**
```matlab
A = [1 2 3];
B = [4 5 6];
C = A + B
```
**Output:**
```
C =
     5     7     9
```

**Contoh 2: Perkalian elemen-per-elemen**
```matlab
X = [2 4 6];
Y = [1 2 3];
Z = X .* Y
```
**Output:**
```
Z =
     2     8    18
```

**Contoh 3: Pembagian elemen-per-elemen**
```matlab
P = [10 20 30];
Q = [2 4 5];
R = P ./ Q
```
**Output:**
```
R =
     5     5     6
```

**Contoh 4: Perpangkatan elemen**
```matlab
V = [2 3 4];
W = V .^ 2
```
**Output:**
```
W =
     4     9    16
```

**Contoh 5: Operasi dengan skalar**
```matlab
arr = [5 10 15];
hasil = arr * 2
```
**Output:**
```
hasil =
    10    20    30
```

### Soal Latihan

**Latihan 1: Jumlahkan [3 6 9] dengan [1 2 3]**
```matlab
A = [3 6 9];
B = [1 2 3];
jumlah = A + B
```
**Output:**
```
jumlah =
     4     8    12
```

**Latihan 2: Kalikan elemen [5 10 15] dengan [2 2 2]**
```matlab
X = [5 10 15];
Y = [2 2 2];
kali = X .* Y
```
**Output:**
```
kali =
    10    20    30
```

**Latihan 3: Pangkatkan [1 2 3 4] dengan 3**
```matlab
data = [1 2 3 4];
pangkat = data .^ 3
```
**Output:**
```
pangkat =
     1     8    27    64
```

---

## Kesimpulan

Materi ini mencakup dasar-dasar pemrograman MATLAB meliputi:
- **Variabel**: Penyimpanan data dengan nama tertentu
- **Operator Aritmatika**: Operasi matematika dasar (+, -, *, /, ^)
- **Operator Relasional**: Perbandingan nilai (==, ~=, >, <, >=, <=)
- **Array**: Kumpulan data dalam satu variabel
- **Akses Elemen Array**: Mengambil nilai tertentu dari array menggunakan indeks
- **Operasi Dasar Array**: Operasi matematika pada seluruh elemen array

Praktikkan setiap contoh dan latihan untuk memahami konsep dengan lebih baik!
