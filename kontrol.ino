// Nem sensörü kalibrasyonu için değerler
const int NEM_KURU = 1023;  // Tamamen kuru toprakta değer
const int NEM_ISLAK = 300;  // Tamamen ıslak toprakta değer

void nemKontrol() {
  // Nem sensöründen değer oku (0-1023)
  int sensorDeger = analogRead(NEM_SENSOR_PIN);
  
  // Değeri yüzdelik değere çevir (0-100)
  // Kalibrasyon değerlerini kullanarak daha doğru ölçüm
  guncelNemDegeri = map(constrain(sensorDeger, NEM_ISLAK, NEM_KURU), NEM_KURU, NEM_ISLAK, 0, 100);
  
  static unsigned long sonSulamaZamani = 0;
  unsigned long simdikiZaman = millis() / 1000; // Saniye cinsinden zaman
  
  // Otomatik sulama aktifse ve nem eşik değerinin altındaysa
  if (otomatikSulamaAktif && guncelNemDegeri < nemEsikDegeri) {
    // Son sulamadan beri minimum bekleme süresi geçtiyse
    if (simdikiZaman - sonSulamaZamani > MIN_SULAMA_BEKLEME) {
      sulamaYap();
      sonSulamaZamani = simdikiZaman;
    }
  }
}

void sulamaYap() {
  // Sulama başlamadan önce kontrol
  if (digitalRead(SULAMA_ROLE_PIN) == LOW) {
    return; // Zaten sulama yapılıyorsa çık
  }
  
  Serial.println("Sulama başlatılıyor...");
  digitalWrite(SULAMA_ROLE_PIN, LOW);  // Sulamayı başlat
  
  // Belirlenen süre kadar bekle, ama bu sırada diğer işlemleri engelleme
  unsigned long sulamaBitisSuresi = millis() + (sulamaSuresi * 1000); // Milisaniyeye çevir
  while (millis() < sulamaBitisSuresi) {
    server.handleClient(); // Web sunucuyu çalışır durumda tut
    delay(100);
  }
  
  digitalWrite(SULAMA_ROLE_PIN, HIGH); // Sulamayı durdur
  Serial.println("Sulama tamamlandı.");
}

void ledZamanlayiciKontrol() {
  if (!ledZamanlayiciAktif) return;
  
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);
  int currentHour = timeinfo->tm_hour;
  
  static bool ledDurumu = false;
  bool yeniDurum = (currentHour >= ledBaslangicSaat && currentHour < ledBitisSaat);
  
  // LED durumu değişmesi gerekiyorsa
  if (ledDurumu != yeniDurum) {
    ledDurumu = yeniDurum;
    digitalWrite(LED_ROLE_PIN, !ledDurumu); // Röle aktif LOW
    Serial.print("LED ");
    Serial.println(ledDurumu ? "açıldı." : "kapandı.");
  }
}
