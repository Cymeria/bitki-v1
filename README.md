# 🌱 Akıllı Bitki Bakım Sistemi

Otomatik sulama ve aydınlatma özelliklerine sahip, WiFi kontrollü akıllı bitki bakım sistemi.

## 📋 Özellikler

- ✨ Modern ve mobil uyumlu web arayüzü
- 🌡️ Gerçek zamanlı nem takibi ve görsel gösterge
- 💧 Otomatik ve manuel sulama kontrolü
- 💡 Programlanabilir LED aydınlatma
- 📱 WiFi üzerinden uzaktan kontrol
- 💾 Ayarların kalıcı olarak saklanması
- 🎨 RGB LED ile nem durumu gösterimi

## 🛠️ Donanım Gereksinimleri

- NodeMCU ESP8266 (ESP-12E)
- Toprak Nem Sensörü
- Su Pompası için 5V Röle Modülü
- LED Aydınlatma için 5V Röle Modülü
- RGB LED (Ortak Anot)
- 12V Su Pompası
- 12V LED Şerit
- 12V Güç Adaptörü
- Jumper Kablolar
- Breadboard veya PCB

## 📊 Bağlantı Şeması

```
NodeMCU ESP8266 Pin Bağlantıları:
┌────────────────────────────────────────────────────────┐
│ Bileşen           │ ESP8266 Pin │ Açıklama            │
├──────────────────────────────────────────────────────┤
│ Toprak Nem Sensörü│    A0      │ Analog giriş        │
│ Sulama Röle      │     D1      │ Dijital çıkış      │
│ LED Röle         │     D2      │ Dijital çıkış      │
│ RGB LED - Kırmızı │    D6      │ PWM çıkış          │
│ RGB LED - Yeşil   │    D7      │ PWM çıkış          │
│ RGB LED - Mavi    │    D8      │ PWM çıkış          │
└────────────────────────────────────────────────────────┘

Güç Bağlantıları:
┌────────────────────────────────────────────────────────┐
│ Bileşen           │ Güç Kaynağı │ Açıklama            │
├──────────────────────────────────────────────────────┤
│ NodeMCU          │ USB/5V      │ Mikrodenetleyici    │
│ Röleler          │ 5V & GND    │ ESP8266'dan        │
│ RGB LED          │ 5V & GND    │ ESP8266'dan        │
│ Su Pompası       │ 12V & GND   │ Harici adaptör     │
│ LED Şerit        │ 12V & GND   │ Harici adaptör     │
└────────────────────────────────────────────────────────┘

RGB LED Renk Kodları:
┌────────────────────────────────────────────────────────┐
│ Nem Değeri │ Renk    │ Anlamı                         │
├──────────────────────────────────────────────────────┤
│   0-30%    │ Kırmızı │ Kritik nem seviyesi           │
│  30-50%    │ Sarı    │ Düşük nem seviyesi            │
│  50-80%    │ Yeşil   │ İdeal nem seviyesi            │
│  80-100%   │ Mavi    │ Yüksek nem seviyesi           │
└────────────────────────────────────────────────────────┘
```

## 🚀 Kurulum

1. Arduino IDE'yi açın
2. Gerekli kütüphaneleri yükleyin:
   - ESP8266WiFi
   - ESP8266WebServer
   - ESP8266mDNS
   - ArduinoOTA
   - LittleFS
   - ArduinoJson

3. NodeMCU kartını Arduino IDE'ye ekleyin:
   - File -> Preferences
   - Additional Board Manager URLs: `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
   - Tools -> Board -> Boards Manager -> ESP8266

4. Doğru board ayarlarını seçin:
   - Board: "NodeMCU 1.0 (ESP-12E Module)"
   - CPU Frequency: "80 MHz"
   - Flash Size: "4MB (FS:2MB OTA:~1019KB)"
   - Upload Speed: "115200"

5. Kodu yükleyin ve çalıştırın

## 💻 Web Arayüzü

Sistem ilk çalıştığında bir WiFi erişim noktası oluşturur:
- SSID: `BitkiBakim`
- Şifre: `12345678`

1. Bu ağa bağlanın
2. Tarayıcıda `192.168.4.1` adresine gidin
3. WiFi ayarları menüsünden kendi ağınıza bağlanın

## ⚙️ Ayarlar

Web arayüzünden aşağıdaki ayarları yapabilirsiniz:
- Nem eşik değeri (%)
- Sulama süresi (saniye)
- LED başlangıç saati
- LED bitiş saati

Tüm ayarlar otomatik olarak kaydedilir ve sistem yeniden başlatıldığında korunur.

## 🔧 Sorun Giderme

1. WiFi bağlantı sorunu:
   - Sistem AP modunda başlar
   - Ayarları sıfırlamak için cihazı yeniden başlatın

2. Sulama çalışmıyor:
   - Röle bağlantılarını kontrol edin
   - Su pompası güç kaynağını kontrol edin

3. LED çalışmıyor:
   - Röle bağlantılarını kontrol edin
   - LED şerit güç kaynağını kontrol edin

4. Nem sensörü hatalı:
   - Sensör bağlantılarını kontrol edin
   - Sensörü temizleyin
   - Kalibrasyon yapın

## 📝 Lisans

Bu proje MIT lisansı altında lisanslanmıştır. Detaylar için [LICENSE](LICENSE) dosyasına bakın.
