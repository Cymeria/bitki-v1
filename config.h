#ifndef CONFIG_H
#define CONFIG_H

// WiFi Ayarları
const char* wifi_default_ssid = "wifi";
const char* wifi_default_pass = "pass";

// AP Modu Ayarları
const char* wifi_ap_ssid = "BitkiBakim_AP";
const char* wifi_ap_pass = "1881";

// Dosya sistemi
const char* wifi_config_file = "/wifi_config.json";

// OTA Ayarları
const char* ota_hostname = "bitkibakim";
const char* ota_password = "1881";

// Pin Tanımlamaları
#define SULAMA_ROLE_PIN D1  // GPIO5
#define LED_ROLE_PIN    D2  // GPIO4
#define NEM_SENSOR_PIN  A0

// RGB LED pinleri
#define RGB_RED_PIN 12   // D6
#define RGB_GREEN_PIN 13 // D7
#define RGB_BLUE_PIN 15  // D8

// Sistem Değişkenleri (saniye cinsinden)
#define NEM_OLCUM_ARALIK 60     // Nem ölçüm aralığı (saniye)
#define MIN_SULAMA_SURESI 1     // Minimum sulama süresi (saniye)
#define MAX_SULAMA_SURESI 300   // Maximum sulama süresi (saniye)
#define MIN_SULAMA_BEKLEME 3600 // İki sulama arası minimum süre (saniye)

// Ayarlar dosyası
#define SETTINGS_FILE "/settings.json"

// Web Sunucu Port
#define WEB_SERVER_PORT 80

#endif
