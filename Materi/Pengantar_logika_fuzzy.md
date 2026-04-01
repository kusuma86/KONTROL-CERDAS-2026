# Pengantar Logika Fuzzy

**Mata Kuliah:** Kontrol Cerdas (MKB2436304405)  
**Dosen Pengampu:** Dr. I N. K. Wardana

---

## Pendahuluan

Dalam beberapa tahun terakhir, logika fuzzy digunakan semakin luas dalam berbagai bidang. Penerapannya tidak hanya ditemukan pada sistem yang kompleks, tetapi juga pada produk sehari-hari seperti kamera, handycam, mesin cuci, dan oven microwave. Di dunia industri, logika fuzzy dipakai dalam kontrol proses, peralatan medis, sistem pendukung keputusan, hingga pemilihan portofolio keuangan. Konsep ini diperkenalkan oleh **Lotfi A. Zadeh**.

![Lotfi A. Zadeh](images/zadeh.jpeg)

Secara sempit, logika fuzzy dapat dipahami sebagai sistem logika yang merupakan pengembangan dari logika multivalued, yaitu logika yang tidak hanya mengenal dua nilai kebenaran. Secara lebih luas, logika fuzzy sangat dekat dengan teori himpunan fuzzy, yaitu teori mengenai kelompok objek yang batas keanggotaannya tidak selalu tegas. Dalam teori ini, suatu objek tidak harus sepenuhnya menjadi anggota atau bukan anggota suatu himpunan. Sebaliknya, objek dapat menjadi anggota secara sebagian.

Salah satu konsep utama dalam logika fuzzy adalah **variabel linguistik**. Variabel ini dinyatakan dalam bentuk kata-kata, bukan semata-mata angka. Misalnya, kita dapat mengatakan “air panas” atau “cuaca dingin”, bukan hanya menyebut suhu 35°C atau 15°C. Dengan demikian, logika fuzzy memungkinkan kita melakukan perhitungan menggunakan istilah yang lebih dekat dengan cara berpikir manusia. Walaupun kata-kata bersifat kurang presisi dibanding angka, justru di situlah keunggulan logika fuzzy: ia mampu menerima ketidakpastian dan menyederhanakan penyelesaian masalah.

Konsep penting lainnya adalah aturan fuzzy berbentuk **jika–maka** (*if–then*). Melalui aturan ini, sistem dapat memproses kondisi yang samar pada bagian antecedent maupun consequent. Dalam praktiknya, solusi fuzzy sering kali merupakan terjemahan dari cara manusia menyelesaikan masalah ke dalam bentuk aturan formal.

Logika fuzzy juga sering dipadukan dengan jaringan saraf tiruan (*neurocomputing*) dan algoritma genetika. Kombinasi ini dikenal sebagai **soft computing**. Berbeda dari *hard computing* yang cenderung menuntut presisi tinggi dan kepastian, *soft computing* lebih toleran terhadap ketidakpastian, ketidakjelasan, dan kebenaran parsial. Karena itu, pendekatan ini sering menghasilkan sistem yang lebih sederhana, tangguh, dan murah. Salah satu contoh yang populer adalah **ANFIS** (*Adaptive Neuro-Fuzzy Inference System*).

Secara umum, logika fuzzy meniru cara berpikir manusia. Dalam banyak situasi, manusia tidak selalu membutuhkan angka yang sangat presisi untuk mengambil keputusan. Misalnya, ketika memperingatkan seseorang tentang benda yang jatuh, kita tidak perlu menyebut massa dan kecepatannya secara tepat. Cukup dengan mengatakan, “Awas, ada benda jatuh!”, pesan sudah dapat dipahami.

![Presisi-Makna](images/presisi.png)

Dengan sudut pandang tersebut, logika fuzzy dapat dilihat sebagai cara praktis untuk memetakan ruang input ke ruang output. Kotak hitam pemetaan ini dapat berupa sistem fuzzy, jaringan saraf tiruan, sistem pakar, persamaan diferensial, dan sebagainya.

![Pemetaan Input-Output](images/blackbox.jpg)

Beberapa contoh pemetaan input–output pada sistem fuzzy antara lain sebagai berikut.

- Berdasarkan kualitas pelayanan di restoran, sistem dapat menentukan besarnya tip yang layak diberikan.
- Berdasarkan tingkat panas air yang diinginkan, sistem dapat mengatur posisi keran secara tepat.
- Berdasarkan jarak objek foto, sistem dapat mengatur fokus lensa kamera secara otomatis.
- Berdasarkan kecepatan mobil dan beban kerja mesin, sistem dapat menentukan waktu perpindahan gigi yang sesuai.

### Mengapa Menggunakan Logika Fuzzy?

Ada beberapa alasan mengapa logika fuzzy banyak digunakan.

1. **Mudah dipahami.** Konsepnya sederhana, intuitif, dan dekat dengan penalaran manusia.
2. **Fleksibel.** Sistem fuzzy dapat dikembangkan tanpa harus dirancang ulang sepenuhnya.
3. **Toleran terhadap data yang tidak presisi.** Dunia nyata sering kali mengandung ketidakpastian, dan logika fuzzy memang dirancang untuk menghadapi kondisi tersebut.
4. **Mampu memodelkan fungsi nonlinier yang rumit.** Sistem fuzzy dapat menyesuaikan berbagai pola hubungan input–output, terlebih jika dipadukan dengan metode adaptif seperti ANFIS.
5. **Dapat memanfaatkan pengalaman ahli.** Berbeda dari model yang benar-benar bersifat *black box*, sistem fuzzy dapat dibangun dari pengetahuan para ahli.
6. **Dapat digabungkan dengan kontrol konvensional.** Logika fuzzy tidak harus menggantikan metode kontrol klasik, tetapi dapat melengkapinya.
7. **Berbasis bahasa alami.** Hal ini menjadikan logika fuzzy lebih komunikatif dan mudah diterapkan.

### Kapan Logika Fuzzy Tidak Perlu Digunakan?

Logika fuzzy pada dasarnya hanyalah salah satu cara praktis untuk memetakan ruang input ke ruang output. Jika pendekatan ini ternyata tidak praktis, maka sebaiknya digunakan metode lain. Jika sudah ada solusi yang lebih sederhana dan efektif, maka solusi itulah yang sebaiknya dipilih. Dengan kata lain, logika fuzzy tetap perlu diterapkan dengan akal sehat. Walaupun banyak pengendali dapat bekerja baik tanpa logika fuzzy, mempelajari logika fuzzy tetap penting karena pendekatan ini sangat berguna untuk menangani ketidakpastian dan nonlinieritas secara cepat dan efisien.

---

## Teori Himpunan

### Definisi Himpunan

Himpunan (*set*) adalah kumpulan objek-objek yang **berbeda**. Objek di dalam himpunan disebut **elemen**, **unsur**, atau **anggota**. Ada beberapa cara untuk menyajikan himpunan, antara lain melalui enumerasi, simbol-simbol baku, notasi pembentuk himpunan, dan diagram Venn.

### Cara Menyajikan Himpunan

#### 1. Enumerasi

Pada cara enumerasi, setiap anggota himpunan didaftarkan secara rinci.

Contoh:

- Empat bilangan asli pertama:  
  `A = {1, 2, 3, 4}`
- Lima bilangan genap positif pertama:  
  `B = {2, 4, 6, 8, 10}`
- Himpunan campuran:  
  `C = {kucing, a, Amir, 10, paku}`
- Contoh himpunan yang memuat himpunan lain sebagai anggota:  
  `R = {a, b, {a, b, c}, {a, c}}`
- Contoh lain:  
  `C = {a, {a}, {{a}}}`
- Himpunan yang hanya berisi himpunan kosong:  
  `K = {{}}`

Perlu diperhatikan bahwa himpunan ditentukan oleh **siapa saja anggotanya**, bukan oleh **urutan** penulisannya. Selain itu, anggota himpunan **tidak boleh ditulis berulang**, kecuali jika memang dibahas sebagai *multiset*.

Misalnya:

- `R = {a, b, {a, b, c}, {a, c}}` memiliki **empat** anggota, yaitu `a`, `b`, `{a, b, c}`, dan `{a, c}`.
- `C = {a, {a}, {{a}}}` memiliki **tiga** anggota, yaitu `a`, `{a}`, dan `{{a}}`.
- `K = {{}}` memiliki **satu** anggota, yaitu `{}`. Himpunan kosong ini sering dilambangkan dengan `∅`.

#### 2. Keanggotaan Himpunan

Keanggotaan suatu objek dalam himpunan dinyatakan dengan simbol:

- `x ∈ A`, artinya `x` adalah anggota himpunan `A`
- `x ∉ A`, artinya `x` bukan anggota himpunan `A`

Contoh:

- Jika `A = {1, 2, 3, 4}`, maka `3 ∈ A`
- Jika `R = {a, b, {a, b, c}, {a, c}}`, maka `{a, b, c} ∈ R`, tetapi `c ∉ R`
- Jika `K = {{}}`, maka `{ } ∈ K`

Contoh latihan:

Jika `A = {1, 2, 3, 4}`, `R = {a, b, {a, b, c}, {a, c}}`, dan `K = {{}}`, maka:

- `a ∈ R` → **Benar**
- `{1, 2} ∈ A` → **Salah**
- `4 ∉ A` → **Salah**
- `{a} ∉ R` → **Benar**
- `{ } ∈ K` → **Benar**
- `5 ∉ A` → **Benar**

Perhatikan pula contoh berikut:

- `P1 = {a, b}`
- `P2 = {{a, b}}`
- `P3 = {{{a, b}}}`

Maka:

- `a ∈ P1`
- `a ∉ P2`
- `P1 ∈ P2`
- `P1 ∉ P3`
- `P2 ∈ P3`

#### 3. Simbol-Simbol Baku

Beberapa himpunan bilangan biasa dituliskan dengan simbol baku sebagai berikut.

- `P` : himpunan bilangan bulat positif = `{0, 1, 2, 3, ...}`
- `N` : himpunan bilangan alami = `{1, 2, 3, ...}`
- `Z` : himpunan bilangan bulat = `{..., -2, -1, 0, 1, 2, ...}`
- `Q` : himpunan bilangan rasional
- `R` : himpunan bilangan riil
- `C` : himpunan bilangan kompleks

Himpunan universal atau semesta biasanya disimbolkan dengan `U`.

Contoh:

Jika `U = {1, 2, 3, 4, 5}` dan `A = {1, 3, 5}`, maka `A` adalah himpunan bagian dari `U`.

#### 4. Notasi Pembentuk Himpunan

Notasi pembentuk himpunan ditulis sebagai berikut:

`{x | syarat yang harus dipenuhi oleh x}`

Contoh:

- Himpunan bilangan bulat positif yang kurang dari 5 dapat ditulis sebagai:  
  `A = {x | x adalah bilangan bulat positif kurang dari 5}`
- Atau secara lebih singkat:  
  `A = {x | x ∈ P, x < 5}`

Notasi tersebut ekuivalen dengan:

`A = {1, 2, 3, 4}`

Contoh lain:

`M = {x | x adalah mahasiswa yang mengambil kuliah Kontrol Cerdas}`

#### 5. Diagram Venn

Diagram Venn digunakan untuk menggambarkan hubungan antarhimpunan secara visual. Dalam diagram ini, `U` menyatakan semesta pembicaraan.

![Venn Diagram](images/venn.png)

Contoh:

Jika `U = {1, 2, ..., 8}`, `A = {1, 2, 3, 5}`, dan `B = {2, 5, 6, 8}`, maka hubungan antara `A` dan `B` dapat divisualisasikan melalui diagram Venn.

### Operasi Himpunan

#### Gabungan (*Union*)

Gabungan dua himpunan `A` dan `B` ditulis:

`A ∪ B = {x | x ∈ A atau x ∈ B}`

![Gabungan Diagram](images/gabungan.png)

Contoh:

Jika `A = {2, 5, 8}` dan `B = {7, 5, 22}`, maka:

`A ∪ B = {2, 5, 7, 8, 22}`

Selain itu, `A ∪ ∅ = A`.

Contoh lain:

- `A = {2, 3, 5, 7, 8, 10, 15}`
- `B = {0, 1, 2, 4, 5, 6}`
- `C = {1, 5, 10, 11, 14, 15}`
- `D = {Kucing, 14, M}`

Maka:

- `A ∪ B = {0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 15}`
- `A ∪ C = {1, 2, 3, 5, 7, 8, 10, 11, 14, 15}`
- `B ∪ C = {0, 1, 2, 4, 5, 6, 10, 11, 14, 15}`
- `B ∪ D = {0, 1, 2, 4, 5, 6, 14, Kucing, M}`
- `A ∪ D = {2, 3, 5, 7, 8, 10, 14, 15, Kucing, M}`
- `C ∪ D = {1, 5, 10, 11, 14, 15, Kucing, M}`

#### Irisan (*Intersection*)

Irisan dua himpunan `A` dan `B` ditulis:

`A ∩ B = {x | x ∈ A dan x ∈ B}`

![Irisan Diagram](images/irisan.png)

Contoh:

Jika `A = {2, 4, 6, 8, 10}` dan `B = {4, 10, 14, 18}`, maka:

`A ∩ B = {4, 10}`

Jika `A = {3, 5, 9}` dan `B = {-2, 6}`, maka:

`A ∩ B = ∅`

Contoh lain dengan himpunan sebelumnya:

- `A ∩ B = {2, 5}`
- `A ∩ C = {5, 10, 15}`
- `B ∩ C = {1, 5}`
- `B ∩ D = ∅`
- `A ∩ D = ∅`
- `C ∩ D = {14}`

---

## Himpunan Fuzzy

### Gambaran Umum

Inti dari logika fuzzy adalah memetakan ruang input ke ruang output. Mekanisme utamanya berupa kumpulan aturan jika–maka (*if–then rules*). Semua aturan tersebut dievaluasi secara paralel, sehingga urutan aturan tidak terlalu penting. Aturan-aturan itu bekerja dengan mengacu pada variabel dan kata sifat yang mendeskripsikan variabel tersebut.

Sebelum membangun sistem fuzzy, semua istilah yang digunakan harus didefinisikan terlebih dahulu. Misalnya, jika ingin menyatakan “air panas”, maka kita harus menentukan rentang suhu yang dimaksud dan bagaimana kata “panas” diterjemahkan ke dalam sistem. Secara umum, inferensi fuzzy adalah metode untuk menafsirkan nilai pada vektor input dan, berdasarkan sejumlah aturan, menghasilkan nilai pada vektor output.

### Pengertian Himpunan Fuzzy

Logika fuzzy dimulai dari konsep **himpunan fuzzy**. Himpunan fuzzy adalah himpunan yang tidak memiliki batas keanggotaan yang sepenuhnya tegas. Untuk memahami hal ini, bandingkan dengan himpunan klasik. Dalam himpunan klasik, suatu elemen hanya memiliki dua kemungkinan: menjadi anggota atau bukan anggota.

![Himpunan Hari](images/himpunanHari.png)

Sebagai contoh, himpunan hari dalam seminggu secara jelas mencakup Senin, Kamis, dan Sabtu, tetapi jelas tidak mencakup sepeda, buku, mentega, atau sepatu. Namun, ketika kita membahas himpunan “hari yang termasuk akhir pekan”, situasinya tidak lagi setegas itu. Sabtu dan Minggu tentu dianggap akhir pekan. Akan tetapi, bagaimana dengan Jumat? Bagi sebagian orang, Jumat sudah terasa seperti bagian dari akhir pekan, meskipun secara formal masih termasuk hari kerja.

![Himpunan Akhir Pekan](images/himpunanAkhirPekan.png)

Contoh ini menunjukkan bahwa persepsi individu dan latar belakang budaya dapat memengaruhi definisi suatu konsep. Bahkan kamus pun tidak selalu memberikan batas yang mutlak tegas. Dalam kondisi semacam ini, logika “ya atau tidak” menjadi kurang memadai. Penalaran fuzzy menjadi relevan karena ia mampu mewakili cara manusia benar-benar memahami suatu konsep.

Dalam logika fuzzy, kebenaran suatu pernyataan tidak hanya dinyatakan sebagai benar atau salah, tetapi sebagai **derajat kebenaran** tertentu. Dengan kata lain, suatu elemen dapat menjadi anggota suatu himpunan secara sebagian.

Secara teknis, hal ini berkaitan dengan logika multivalued. Jika ditanyakan, “Apakah X anggota dari himpunan A?”, maka jawabannya tidak harus hanya 1 atau 0. Jawaban dapat berada di antara keduanya. Jadi, X dapat memiliki **keanggotaan parsial** dalam A.

Contoh yang sangat umum adalah konsep *weekend-ness* atau “tingkat keakhirpekanan”. Sepanjang hari Jumat, dan bahkan sebagian kecil hari Kamis, dapat dikatakan memiliki sifat sebagai bagian dari akhir pekan. Tingkat sifat tersebut dapat digambarkan oleh sebuah kurva yang memetakan input berupa waktu dalam seminggu ke output berupa derajat “akhir pekan”. Kurva inilah yang disebut **fungsi keanggotaan** (*membership function*).

![Konsep Biner dan Multi Value](images/himpunanMultiValue.png)

Contoh lain dari kekaburan konsep dapat dilihat pada pergantian musim. Batas antara satu musim dan musim lainnya tidak selalu berubah secara mendadak, melainkan sering berlangsung secara bertahap.

![Konsep Pergantian Musim](images/pergantianMusim.png)

### Atribut Himpunan Fuzzy

Himpunan fuzzy memiliki dua atribut utama.

1. **Linguistik**, yaitu penamaan dengan bahasa alami, misalnya: dingin, sejuk, hangat, dan panas.
2. **Numeris**, yaitu nilai yang menyatakan ukuran suatu variabel, misalnya: 0, 15, 20, 30, dan 40.

### Istilah-Istilah dalam Sistem Fuzzy

Beberapa istilah penting dalam sistem fuzzy adalah sebagai berikut.

- **Variabel fuzzy**: variabel yang sedang dibahas, misalnya temperatur, tinggi badan, atau umur.
- **Himpunan fuzzy**: bagian dari variabel fuzzy yang mewakili suatu kondisi tertentu. Sebagai contoh, variabel umur dapat dibagi menjadi himpunan fuzzy *muda*, *paruh baya*, dan *tua*.
- **Semesta pembicaraan** (*universe of discourse*): seluruh rentang nilai yang mungkin dari suatu variabel fuzzy. Misalnya, untuk umur dapat digunakan interval `[0, 100]`.
- **Domain**: rentang nilai yang digunakan oleh suatu himpunan fuzzy tertentu. Misalnya, *muda = [0, 45]*, *paruh baya = [35, 55]*, dan *tua = [45, 100]*.

---

## Fungsi Keanggotaan

Fungsi keanggotaan (*membership function* atau MF) adalah kurva yang menunjukkan bagaimana setiap titik pada ruang input dipetakan ke nilai keanggotaan antara 0 dan 1. Ruang input ini biasa disebut sebagai **semesta pembicaraan**.

Sebagai contoh, jika kita ingin mendefinisikan konsep “orang tinggi”, maka tidak cukup hanya menentukan batas tunggal. Seseorang dengan tinggi tertentu dapat dianggap “cukup tinggi”, “sangat tinggi”, atau “tidak terlalu tinggi”, tergantung pada nilai derajat keanggotaannya.

Pendekatan biner akan memaksa kita memilih hanya dua kemungkinan: tinggi atau tidak tinggi. Sebaliknya, pendekatan fuzzy memungkinkan penilaian yang lebih halus. Inilah yang membuat logika fuzzy lebih dekat dengan cara manusia menilai realitas.

![Konsep Tinggu Badan Fuzzy](images/tinggiBadan.png)

Contoh perhitungan:

Untuk himpunan fuzzy **paruh baya**, menggunakan fungsi keanggotaan segitiga, seperti gambar di bawah ini. Misalkan usia 40 tahun memiliki fungsi keanggotaan linear pada interval 35 sampai 45, maka:

`μ_paruhbaya(40) = (40 - 35) / (45 - 35) = 5 / 10 = 0.5`

Artinya, usia 40 tahun memiliki derajat keanggotaan 0,5 dalam himpunan *paruh baya*.

![Konsep Tinggi Badan berdasarkan Fuzzy](images/paruhBaya.png)

---

## Operasi Logika Fuzzy

Dalam sistem fuzzy, operasi logika digunakan untuk menggabungkan atau memodifikasi derajat keanggotaan. Operasi ini menjadi dasar ketika sistem menafsirkan aturan-aturan fuzzy. Operasi **OR** akan mencari nilai **maksimum** sedangkan operasi **AND** akan mencari nilai **minimum** dari setiap keanggotaan yang terlibat dalam operasi tersebut. Amati ilustrasi dari operasi tersebut di bawah ini.

![Operasi Fuzzy](images/operasiFuzzy.png)
---

## Aturan If–Then

Aturan **if–then** merupakan inti dari pembentukan sistem fuzzy. Bentuk umumnya adalah:

`IF x is A THEN y is B`

Contoh:

`JIKA udara panas MAKA kecepatan kipas besar`

Contoh lain:

`JIKA pelayanan istimewa ATAU makanan enak MAKA tip besar`

![IF-THEN](images/ifthen.png)

Aturan seperti ini memungkinkan sistem mengambil keputusan berdasarkan istilah yang dekat dengan bahasa manusia.

### Tahapan Penafsiran Aturan If–Then

Penafsiran aturan fuzzy secara umum dilakukan melalui tiga tahap.

#### 1. Fuzzifikasi Input

Semua pernyataan fuzzy pada bagian antecedent, yaitu bagian “jika”, diubah menjadi derajat keanggotaan antara 0 dan 1. Jika antecedent hanya terdiri dari satu bagian, maka nilai tersebut langsung menjadi tingkat dukungan terhadap aturan.

#### 2. Operator Fuzzy untuk Antecedent Majemuk

Jika antecedent terdiri dari beberapa bagian, maka digunakan operator fuzzy seperti **AND** atau **OR** untuk menggabungkannya. Hasilnya adalah satu nilai antara 0 dan 1 yang menyatakan kekuatan aktivasi aturan.

#### 3. Metode Implikasi

Pada bagian consequent, yaitu bagian “maka”, sistem menetapkan suatu himpunan fuzzy pada output. Jika antecedent hanya benar sebagian, maka himpunan fuzzy output akan dipotong atau disesuaikan menurut metode implikasi yang digunakan.

---

## Fuzzy Inference System (FIS)

**Fuzzy inference** adalah proses formulasi untuk memetakan input ke output menggunakan logika fuzzy. Dengan kata lain, FIS adalah metode yang menafsirkan nilai-nilai dalam vektor input dan, berdasarkan sejumlah aturan, memberikan keluaran pada vektor output.

Secara umum, FIS mencakup tiga komponen utama:

- fungsi keanggotaan,
- operasi logika,
- aturan if–then.

![FIS](images/fis.png)

---

## Fuzzy dan Sistem Kontrol

Dalam sistem kontrol, logika fuzzy dapat ditempatkan sebagai bagian dari pengendali. Sistem menerima **set point** dan **pengukuran**, lalu menghasilkan **aksi kontrol** untuk memengaruhi plant. Selisih antara nilai yang diinginkan dan nilai aktual menjadi **error**, yang kemudian diproses oleh pengendali fuzzy untuk menghasilkan respons kontrol yang sesuai.

Dengan demikian, logika fuzzy dapat dipandang sebagai salah satu pendekatan pengendalian yang memanfaatkan pengetahuan berbasis aturan dan toleransi terhadap ketidakpastian.

---

## Referensi

1. MathWorks, “What Is Fuzzy Logic? - MATLAB & Simulink.” Accessed: Sep. 03, 2025.
2. MathWorks, “Foundations of Fuzzy Logic - MATLAB & Simulink.” Accessed: Sep. 03, 2025.
3. MathWorks, “Fuzzy vs. Nonfuzzy Logic - MATLAB & Simulink.” Accessed: Sep. 03, 2025.
4. MathWorks, “Fuzzy Inference Process - MATLAB & Simulink.” Accessed: Sep. 03, 2025.

---

