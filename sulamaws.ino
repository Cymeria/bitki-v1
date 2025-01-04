#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <time.h>
#include <TZ.h>
#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "config.h"

ESP8266WebServer server(WEB_SERVER_PORT);

// Sistem Değişkenleri
int nemEsikDegeri = 30;  // Varsayılan nem eşik değeri
int sulamaSuresi = 5;    // Varsayılan sulama süresi (saniye)
bool otomatikSulamaAktif = true;
bool ledAktif = false;
unsigned long sonNemOlcumZamani = 0;
int guncelNemDegeri = 0;

// LED Zamanlayıcı
int ledBaslangicSaat = 6;
int ledBitisSaat = 18;
bool ledZamanlayiciAktif = true;

// WiFi yapılandırması
struct WiFiConfig {
  char ssid[32];
  char password[64];
} wifiConfig;

// Global değişkenler
struct Settings {
  int nemEsik;
  int sulamaSuresi;
  int ledBaslangic;
  int ledBitis;
} settings;

// RGB LED için renk yapısı
struct RGBColor {
  int r;
  int g;
  int b;
};

void loadWiFiConfig() {
  if (LittleFS.exists(wifi_config_file)) {
    File configFile = LittleFS.open(wifi_config_file, "r");
    if (configFile) {
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, configFile);
      if (!error) {
        strlcpy(wifiConfig.ssid, doc["ssid"] | wifi_default_ssid, sizeof(wifiConfig.ssid));
        strlcpy(wifiConfig.password, doc["password"] | wifi_default_pass, sizeof(wifiConfig.password));
      }
      configFile.close();
    }
  } else {
    // Varsayılan değerleri kullan
    strlcpy(wifiConfig.ssid, wifi_default_ssid, sizeof(wifiConfig.ssid));
    strlcpy(wifiConfig.password, wifi_default_pass, sizeof(wifiConfig.password));
    saveWiFiConfig();
  }
}

void saveWiFiConfig() {
  File configFile = LittleFS.open(wifi_config_file, "w");
  if (configFile) {
    StaticJsonDocument<200> doc;
    doc["ssid"] = wifiConfig.ssid;
    doc["password"] = wifiConfig.password;
    serializeJson(doc, configFile);
    configFile.close();
  }
}

bool connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiConfig.ssid, wifiConfig.password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nBağlandı!");
    Serial.print("IP Adresi: ");
    Serial.println(WiFi.localIP());
    return true;
  }
  
  return false;
}

void startAPMode() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(wifi_ap_ssid, wifi_ap_pass);
  Serial.println("AP Modu başlatıldı");
  Serial.print("AP IP Adresi: ");
  Serial.println(WiFi.softAPIP());
}

void setupRGBLed() {
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);
  analogWrite(RGB_RED_PIN, 0);
  analogWrite(RGB_GREEN_PIN, 0);
  analogWrite(RGB_BLUE_PIN, 0);
}

RGBColor getNemColor(int nemDegeri) {
  RGBColor color = {0, 0, 0};
  
  if (nemDegeri <= 30) {
    // Kırmızı (0%) -> Sarı (30%)
    float ratio = nemDegeri / 30.0;
    color.r = 255;
    color.g = 255 * ratio;
    color.b = 0;
  } else if (nemDegeri <= 50) {
    // Sarı (30%) -> Yeşil (50%)
    float ratio = (nemDegeri - 30) / 20.0;
    color.r = 255 * (1 - ratio);
    color.g = 255;
    color.b = 0;
  } else if (nemDegeri <= 80) {
    // Yeşil (50%) -> Turkuaz (80%)
    float ratio = (nemDegeri - 50) / 30.0;
    color.r = 0;
    color.g = 255;
    color.b = 255 * ratio;
  } else {
    // Turkuaz (80%) -> Mavi (100%)
    float ratio = (nemDegeri - 80) / 20.0;
    color.r = 0;
    color.g = 255 * (1 - ratio);
    color.b = 255;
  }
  
  return color;
}

void updateRGBLed(int nemDegeri) {
  RGBColor color = getNemColor(nemDegeri);
  analogWrite(RGB_RED_PIN, color.r);
  analogWrite(RGB_GREEN_PIN, color.g);
  analogWrite(RGB_BLUE_PIN, color.b);
}

bool loadSettings() {
  if (!LittleFS.begin()) {
    Serial.println("LittleFS başlatılamadı!");
    return false;
  }

  if (!LittleFS.exists(SETTINGS_FILE)) {
    // Varsayılan ayarları kullan
    settings.nemEsik = 30;
    settings.sulamaSuresi = 5;
    settings.ledBaslangic = 6;
    settings.ledBitis = 18;
    return saveSettings(); // Varsayılan ayarları kaydet
  }

  File file = LittleFS.open(SETTINGS_FILE, "r");
  if (!file) {
    Serial.println("Ayar dosyası açılamadı!");
    return false;
  }

  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, file);
  file.close();

  if (error) {
    Serial.println("Ayarlar parse edilemedi!");
    return false;
  }

  settings.nemEsik = doc["nemEsik"] | 30;
  settings.sulamaSuresi = doc["sulamaSuresi"] | 5;
  settings.ledBaslangic = doc["ledBaslangic"] | 6;
  settings.ledBitis = doc["ledBitis"] | 18;

  return true;
}

bool saveSettings() {
  if (!LittleFS.begin()) {
    Serial.println("LittleFS başlatılamadı!");
    return false;
  }

  File file = LittleFS.open(SETTINGS_FILE, "w");
  if (!file) {
    Serial.println("Ayar dosyası oluşturulamadı!");
    return false;
  }

  StaticJsonDocument<256> doc;
  doc["nemEsik"] = settings.nemEsik;
  doc["sulamaSuresi"] = settings.sulamaSuresi;
  doc["ledBaslangic"] = settings.ledBaslangic;
  doc["ledBitis"] = settings.ledBitis;

  if (serializeJson(doc, file) == 0) {
    Serial.println("Ayarlar yazılamadı!");
    file.close();
    return false;
  }

  file.close();
  return true;
}

void setupLittleFS() {
  // LittleFS başlatma öncesi bilgilendirme
  Serial.println("\n--- LittleFS Başlatma Tanılama ---");
  
  // Dosya sistemi bilgilerini al
  FSInfo fs_info;
  
  // LittleFS başlatma
  if (!LittleFS.begin()) {
    Serial.println("HATA: LittleFS başlatılamadı!");
    
    // Detaylı hata bilgisi
    Serial.println("Olası nedenler:");
    Serial.println("1. Dosya sistemi bozuk");
    Serial.println("2. Yetersiz bellek");
    Serial.println("3. Donanım hatası");
    
    // Formatlamayı dene
    Serial.println("\nDosya sistemi formatlanmaya çalışılıyor...");
    if (LittleFS.format()) {
      Serial.println("Dosya sistemi başarıyla formatlandı.");
      
      // Yeniden başlatmayı dene
      if (LittleFS.begin()) {
        Serial.println("Dosya sistemi yeniden başlatıldı.");
      } else {
        Serial.println("HATA: Dosya sistemi yeniden başlatılamadı!");
        return;
      }
    } else {
      Serial.println("HATA: Dosya sistemi formatlanamadı!");
      return;
    }
  }
  
  // Dosya sistemi bilgilerini yazdır
  LittleFS.info(fs_info);
  Serial.println("\n--- Dosya Sistemi Bilgileri ---");
  Serial.printf("Toplam boyut: %d bayt\n", fs_info.totalBytes);
  Serial.printf("Kullanılan boyut: %d bayt\n", fs_info.usedBytes);
  Serial.printf("Blok boyutu: %d bayt\n", fs_info.blockSize);
  Serial.printf("Sayfa boyutu: %d bayt\n", fs_info.pageSize);
  Serial.printf("Maksimum açık dosya sayısı: %d\n", fs_info.maxOpenFiles);
  
  // Gerekli dizinleri oluştur
  const char* requiredDirs[] = {"/logs", "/config", "/data"};
  for (const char* dir : requiredDirs) {
    if (!LittleFS.exists(dir)) {
      if (LittleFS.mkdir(dir)) {
        Serial.printf("'%s' dizini oluşturuldu.\n", dir);
      } else {
        Serial.printf("HATA: '%s' dizini oluşturulamadı!\n", dir);
      }
    }
  }
  
  // Örnek bir test dosyası oluştur
  File testFile = LittleFS.open("/logs/test.txt", "w");
  if (testFile) {
    testFile.println("LittleFS test dosyası");
    testFile.close();
    Serial.println("\nTest dosyası başarıyla oluşturuldu.");
  } else {
    Serial.println("\nHATA: Test dosyası oluşturulamadı!");
  }
  
  Serial.println("\n--- LittleFS Başlatma Tamamlandı ---\n");
}


// Bellek durumunu izleme fonksiyonu
void printMemoryStatus() {
  #ifdef ESP8266
    uint32_t free = ESP.getFreeHeap();
    uint32_t max = ESP.getMaxFreeBlockSize();
    uint32_t fragmentation = ESP.getHeapFragmentation();
    
    Serial.printf("Bellek Durumu:\n");
    Serial.printf("- Boş Bellek: %u bayt\n", free);
    Serial.printf("- Maks Boş Blok: %u bayt\n", max);
    Serial.printf("- Bellek Parçalanması: %u%%\n", fragmentation);
  #endif
}

// Gereksiz oturumları ve geçici verileri temizleme
void cleanupSessions() {
  // Eski oturumları temizle
  server.client().stop();
  
  // WiFi bağlantısını kontrol et ve gerekirse yeniden bağlan
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.reconnect();
  }
}

// Periyodik bakım fonksiyonu
void performMaintenanceTasks() {
  static unsigned long lastMaintenanceTime = 0;
  unsigned long currentTime = millis();
  
  // Her 1 saatte bir bakım
  if (currentTime - lastMaintenanceTime >= 3600000) {
    Serial.println("Periyodik Bakım Başladı");
    
    // Bellek durumunu yazdır
    printMemoryStatus();
    
    // Gereksiz oturumları temizle
    cleanupSessions();
    
    // LittleFS için bakım
    LittleFS.gc();
    
    lastMaintenanceTime = currentTime;
    Serial.println("Periyodik Bakım Tamamlandı");
  }
}

// JSON bellek kullanımını optimize etme
void optimizeJsonMemory(DynamicJsonDocument& doc) {
  // Kullanılmayan belleği serbest bırak
  doc.garbageCollect();
  
  // Bellek sınırını kontrol et
  if (doc.memoryUsage() > doc.capacity() * 0.8) {
    Serial.println("JSON bellek kullanımı yüksek, yeniden boyutlandırılıyor");
    doc.clear();
  }
}

// Log dosyasını boyutlandırma ve temizleme
void manageLogs() {
  const char* logPath = "/logs/system_log.txt";
  
  // Log dosyasının boyutunu kontrol et
  File logFile = LittleFS.open(logPath, "r");
  if (logFile) {
    size_t fileSize = logFile.size();
    logFile.close();
    
    // 100 KB'dan büyükse log dosyasını kes
    if (fileSize > 102400) {
      Serial.println("Log dosyası boyutu sınırı aşıldı, kırpılıyor");
      
      // Geçici dosyaya son 50 KB'ı yaz
      File tempFile = LittleFS.open("/logs/temp_log.txt", "w");
      logFile = LittleFS.open(logPath, "r");
      
      logFile.seek(fileSize - 51200);  // Son 50 KB
      
      while (logFile.available()) {
        tempFile.write(logFile.read());
      }
      
      logFile.close();
      tempFile.close();
      
      // Eski log dosyasını sil ve geçici dosyayı yeniden adlandır
      LittleFS.remove(logPath);
      LittleFS.rename("/logs/temp_log.txt", logPath);
    }
  }
}


void setup() {
  Serial.begin(115200);
  
  setupLittleFS();
  
  // Pin modlarını ayarla
  pinMode(SULAMA_ROLE_PIN, OUTPUT);
  pinMode(LED_ROLE_PIN, OUTPUT);
  digitalWrite(SULAMA_ROLE_PIN, HIGH);
  digitalWrite(LED_ROLE_PIN, HIGH);

  setupRGBLed();
  
  // WiFi yapılandırmasını yükle ve bağlan
  loadWiFiConfig();
  if (!connectWiFi()) {
    startAPMode();
  }

  // Zaman sunucusu ayarları
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  setenv("TZ", "EET-2EEST,M3.5.0/3,M10.5.0/4", 0);
  tzset();
  
  while (time(nullptr) < 1000000000l) {
    Serial.print(".");
    delay(100);
  }
  Serial.println("\nZaman senkronize edildi!");

  // MDNS başlat
  if (MDNS.begin(ota_hostname)) {
    Serial.println("MDNS başlatıldı");
  }

  // OTA ayarları
  setupOTA();

  // Web sunucu rotaları
  setupWebServer();

  server.begin();
  Serial.println("HTTP sunucu başlatıldı");
  
  if (!loadSettings()) {
    Serial.println("Ayarlar yüklenemedi, varsayılanlar kullanılıyor!");
  }
}

void setupOTA() {
  ArduinoOTA.setHostname(ota_hostname);
  ArduinoOTA.setPassword(ota_password);
  
  ArduinoOTA.onStart([]() {
    Serial.println("OTA Başlatılıyor...");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("OTA Tamamlandı!");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("İlerleme: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Hata[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Kimlik Doğrulama Hatası");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Başlangıç Hatası");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Bağlantı Hatası");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Veri Alma Hatası");
    else if (error == OTA_END_ERROR) Serial.println("Sonlandırma Hatası");
  });
  
  ArduinoOTA.begin();
}

void loop() {
  ArduinoOTA.handle();
  server.handleClient();
  MDNS.update();
  
  // Nem kontrolü
  if (millis() - sonNemOlcumZamani >= NEM_OLCUM_ARALIK) {
    nemKontrol();
    sonNemOlcumZamani = millis();
  }
  
  // LED zamanlayıcı kontrolü
  ledZamanlayiciKontrol();
  
  int nemDegeri = analogRead(NEM_SENSOR_PIN);
  nemDegeri = map(nemDegeri, 0, 1023, 0, 100);
  updateRGBLed(nemDegeri);

  // Periyodik bakım görevlerini çalıştır
  performMaintenanceTasks();
  
  // Log yönetimi
  manageLogs();
  
  // Sistemin çalışma durumunu güncelle
  //updateSystemStatus();
  
  // Bellek durumunu periyodik olarak kontrol et
  static unsigned long lastMemoryCheckTime = 0;
  unsigned long currentTime = millis();
  
  if (currentTime - lastMemoryCheckTime >= 300000) {  // 5 dakikada bir
    printMemoryStatus();
    lastMemoryCheckTime = currentTime;
    
    // Aşırı bellek kullanımı varsa önlem al
    #ifdef ESP8266
      if (ESP.getFreeHeap() < 10000) {  // 10 KB'dan az boş bellek
        Serial.println("DİKKAT: Düşük bellek seviyesi!");
        ESP.restart();  // Cihazı yeniden başlat
      }
    #endif
  }
  
  // Gereksiz işlemleri sonlandır
  yield();

}
