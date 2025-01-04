// Function prototypes for web server handlers
void handleRoot();
void handleData();
void handleWiFiScan();
void handleWiFiConnect();
void handleSulama();
void handleLed();
void handleAyarlar();
void handleNotFound();
void handleLogs();
void handleFileList();
void handleFileRead();
void handleFileSave();
void handleFileDelete();

void setupWebServer() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/data", HTTP_GET, handleData);
  server.on("/ayarlar", HTTP_POST, handleAyarlar);
  server.on("/wifi", HTTP_GET, handleWiFiScan);
  server.on("/wifi/baglan", HTTP_POST, handleWiFiConnect);
  server.on("/logs", HTTP_GET, handleLogs);
  server.on("/files", HTTP_GET, handleFileList);
  server.on("/files/read", HTTP_GET, handleFileRead);
  server.on("/files/save", HTTP_POST, handleFileSave);
  server.on("/files/delete", HTTP_POST, handleFileDelete);
  server.onNotFound(handleNotFound);
//  server.on("/status", HTTP_GET, handleStatus);
  server.on("/files/list", HTTP_GET, handleFileList);  
  // Log route'ları
//  server.on("/logs/dates", HTTP_GET, handleLogDates);
//  server.on("/logs/by-date", HTTP_GET, handleLogsByDate);
  server.on("/fs", HTTP_GET, handleFileManager);
  server.on("/list", HTTP_GET, handleFileListfs);
  server.on("/upload", HTTP_POST, []() {
    server.send(200, "text/plain", "");
  }, handleFileUpload);
  server.on("/delete", HTTP_POST, handleDelete);
    server.on("/read", HTTP_GET, handleFileReadfs);
  server.on("/save", HTTP_POST, handleFileSavefs);
    
  // Maksimum bağlantı sayısını sınırla
//  server.setMaxHandlers(10);
  
  // Zaman aşımı ayarları
//  server.setTimeout(10);  // Saniye cinsinden zaman aşımı
  
  // Gereksiz header'ları devre dışı bırak
  server.sendHeader("Connection", "close");
  
  // Sıkıştırma ve performans ayarları
  #ifdef ESP8266
    server.enableCORS(true);
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  #endif

  // Durum sorgulama
  //server.on("/status", handleStatus);
  
  // Sulama kontrolleri
  server.on("/sulama", HTTP_GET, handleSulama);
  server.on("/sulama", HTTP_POST, handleSulama);
  
  // LED kontrolleri
  server.on("/led", HTTP_GET, handleLed);
  server.on("/led", HTTP_POST, handleLed);
  
  // Dosya işlemleri
  //server.on("/dosya-listesi", handleDosyaListesi);
  //server.on("/dosya-oku", handleDosyaOku);
    
  // Ayarlar
  //server.on("/ayarlari-kaydet", handleAyarlariKaydet);

}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>Bitki Bakım Sistemi</title>";
  html += "<link href='https://fonts.googleapis.com/css2?family=Roboto:wght@300;400;500&display=swap' rel='stylesheet'>";
  html += "<link href='https://fonts.googleapis.com/icon?family=Material+Icons' rel='stylesheet'>";
  html += "<style>";
  html += "* {box-sizing:border-box;margin:0;padding:0;}";
  html += "body{font-family:'Roboto',sans-serif;background:#f5f5f5;color:#333;line-height:1.6;}";
  html += ".container{max-width:1200px;margin:0 auto;padding:20px;}";
  html += ".header{background:#4CAF50;color:white;padding:20px 0;margin-bottom:20px;box-shadow:0 2px 5px rgba(0,0,0,0.1);}";
  html += ".header h1{margin:0;padding:0 20px;font-size:24px;font-weight:400;}";
  html += ".grid{display:grid;grid-template-columns:repeat(auto-fit, minmax(300px, 1fr));gap:20px;margin-bottom:20px;}";
  html += ".card{background:#fff;border-radius:8px;box-shadow:0 2px 5px rgba(0,0,0,0.1);overflow:hidden;}";
  html += ".card-header{background:#f8f9fa;padding:15px;border-bottom:1px solid #eee;display:flex;align-items:center;}";
  html += ".card-header i{margin-right:10px;color:#4CAF50;}";
  html += ".card-header h2{font-size:18px;font-weight:500;margin:0;}";
  html += ".card-content{padding:20px;}";
  html += ".status-item{display:flex;align-items:center;margin-bottom:15px;padding:10px;border-radius:4px;background:#f8f9fa;}";
  html += ".status-item i{margin-right:10px;color:#4CAF50;}";
  html += ".status-item .label{font-weight:500;margin-right:10px;}";
  html += ".status-item .value{color:#666;}";
  html += ".button{display:inline-flex;align-items:center;justify-content:center;padding:10px 20px;border:none;border-radius:4px;cursor:pointer;font-size:14px;font-weight:500;text-transform:uppercase;transition:all 0.3s;margin:5px;min-width:120px;}";
  html += ".button i{margin-right:8px;}";
  html += ".button.primary{background:#4CAF50;color:white;}";
  html += ".button.primary:hover{background:#45a049;transform:translateY(-1px);}";
  html += ".button.danger{background:#f44336;color:white;}";
  html += ".button.danger:hover{background:#da190b;transform:translateY(-1px);}";
  html += ".button.disabled{opacity:0.6;cursor:not-allowed;}";
  html += ".input-group{margin-bottom:15px;}";
  html += ".input-group label{display:block;margin-bottom:5px;color:#666;font-size:14px;}";
  html += ".input-group input{width:100%;padding:8px 12px;border:1px solid #ddd;border-radius:4px;font-size:14px;transition:border-color 0.3s;}";
  html += ".input-group input:focus{outline:none;border-color:#4CAF50;}";
  html += ".progress-bar{height:4px;background:#eee;border-radius:2px;margin:10px 0;overflow:hidden;}";
  html += ".progress-bar .fill{height:100%;background:#4CAF50;transition:width 0.3s ease;}";
  html += ".wifi-list{margin-top:15px;}";
  html += ".wifi-item{display:flex;align-items:center;padding:12px;border:1px solid #eee;margin:5px 0;border-radius:4px;cursor:pointer;transition:all 0.3s;}";
  html += ".wifi-item:hover{background:#f5f5f5;transform:translateX(5px);}";
  html += ".wifi-item i{margin-right:10px;color:#4CAF50;}";
  html += ".wifi-item .ssid{flex-grow:1;}";
  html += ".wifi-item .strength{color:#666;font-size:14px;}";
  html += ".toast{position:fixed;bottom:20px;right:20px;padding:12px 24px;background:#333;color:white;border-radius:4px;opacity:0;transform:translateY(100%);transition:all 0.3s;}";
  html += ".toast.show{opacity:1;transform:translateY(0);}";
  html += ".toast.success{background:#4CAF50;}";
  html += ".toast.error{background:#f44336;}";
  html += ".moisture-indicator{width:20px;height:20px;border-radius:50%;margin-left:auto;box-shadow:0 2px 5px rgba(0,0,0,0.2);}";
  html += ".log-table {";
  html += "  width: 100%;";
  html += "  border-collapse: separate;";
  html += "  border-spacing: 0 5px;";
  html += "  margin-top: 10px;";
  html += "}";
  
  html += "  .log-table th {";
  html += "    background-color: #f4f4f4;";
  html += "    color: #333;";
  html += "    padding: 10px;";
  html += "    text-align: left;";
  html += "    font-weight: bold;";
  html += "  }";
  
  html += "  .log-table td {";
  html += "    padding: 10px;";
  html += "    background-color: #f9f9f9;";
  html += "    border-bottom: 1px solid #e0e0e0;";
  html += "  }";
  
  html += "  .log-table tr:hover td {";
  html += "    background-color: #f0f0f0;";
  html += "    cursor: pointer;";
  html += "  }";
  
  html += "  .log-type-SULAMA { color: #2196F3; }";
  html += "  .log-type-LED { color: #FF9800; }";
  html += "  .log-type-SISTEM { color: #4CAF50; }";
  html += "@media (max-width: 600px) {";
  html += "  .container{padding:10px;}";
  html += "  .grid{grid-template-columns:1fr;}";
  html += "  .card-header h2{font-size:16px;}";
  html += "  .button{width:100%;margin:5px 0;}";
  html += "}";
  html += "  .modal {";
  html += "    display: none;";
  html += "    position: fixed;";
  html += "    z-index: 1000;";
  html += "    left: 0;";
  html += "    top: 0;";
  html += "    width: 100%;";
  html += "    height: 100%;";
  html += "    overflow: auto;";
  html += "    background-color: rgba(0,0,0,0.4);";
  html += "  }";
  
  html += "  .modal-content {";
  html += "    background-color: #fefefe;";
  html += "    margin: 10% auto;";
  html += "    padding: 20px;";
  html += "    border-radius: 10px;";
  html += "    width: 80%;";
  html += "    max-height: 70%;";
  html += "    overflow-y: auto;";
  html += "    box-shadow: 0 4px 6px rgba(0,0,0,0.1);";
  html += "  }";
  
  html += "  .close-button {";
  html += "    color: #aaa;";
  html += "    float: right;";
  html += "    font-size: 28px;";
  html += "    font-weight: bold;";
  html += "    cursor: pointer;";
  html += "  }";
  
  html += "  .close-button:hover {";
  html += "    color: #333;";
  html += "  }";
  
  html += "  .control-group {";
  html += "    display: flex;";
  html += "    flex-direction: column;";
  html += "    gap: 15px;";
  html += "  }";
  
  html += "  .control-item {";
  html += "    display: flex;";
  html += "    align-items: center;";
  html += "    background-color: #f4f4f4;";
  html += "    border-radius: 10px;";
  html += "    padding: 10px;";
  html += "    box-shadow: 0 2px 5px rgba(0,0,0,0.1);";
  html += "    transition: all 0.3s ease;";
  html += "  }";
  
  html += "  .control-icon {";
  html += "    display: flex;";
  html += "    align-items: center;";
  html += "    justify-content: center;";
  html += "    width: 50px;";
  html += "    height: 50px;";
  html += "    border-radius: 50%;";
  html += "    margin-right: 15px;";
  html += "    transition: transform 0.3s ease;";
  html += "  }";
  
  html += "  .water-icon {";
  html += "    background-color: rgba(33, 150, 243, 0.2);";
  html += "    color: #2196F3;";
  html += "  }";
  
  html += "  .light-icon {";
  html += "    background-color: rgba(255, 152, 0, 0.2);";
  html += "    color: #FF9800;";
  html += "  }";
  
  html += "  .control-icon i {";
  html += "    font-size: 24px;";
  html += "  }";
  
  html += "  .control-label {";
  html += "    flex-grow: 1;";
  html += "    font-weight: 500;";
  html += "    color: #333;";
  html += "  }";
  
  html += "  .power-button {";
  html += "    width: 50px;";
  html += "    height: 50px;";
  html += "    border-radius: 50%;";
  html += "    display: flex;";
  html += "    align-items: center;";
  html += "    justify-content: center;";
  html += "    cursor: pointer;";
  html += "    transition: all 0.3s ease;";
  html += "  }";
  
  html += "  .power-button.active {";
  html += "    background-color: #4CAF50;";
  html += "    color: white;";
  html += "    box-shadow: 0 0 10px rgba(76, 175, 80, 0.5);";
  html += "  }";
  
  html += "  .power-button.inactive {";
  html += "    background-color: #F44336;";
  html += "    color: white;";
  html += "  }";
  
  html += "  pre {";
  html += "    background-color: #f4f4f4;";
  html += "    padding: 15px;";
  html += "    border-radius: 5px;";
  html += "    max-height: 400px;";
  html += "    overflow-y: auto;";
  html += "    white-space: pre-wrap;";
  html += "    word-wrap: break-word;";
  html += "  }";
  html += "</style></head><body>";
  
  // Header
  html += "<div class='header'><h1><i class='material-icons'>eco</i> Bitki Bakım Sistemi</h1></div>";
  
  html += "<div class='container'>";
  html += "<div class='grid'>";
  
  // Durum Kartı
  html += "<div class='card'>";
  html += "<div class='card-header'><i class='material-icons'>dashboard</i><h2>Sistem Durumu</h2></div>";
  html += "<div class='card-content' id='statusCard'></div>";
  html += "</div>";
  
// Manuel Kontrol Bölümü
  html += "  <div class='card'>";
  html += "    <h3>🌱 Manuel Kontrol</h3>";
  html += "    <div class='control-group'>";
  html += "      <div class='control-item'>";
  html += "        <div class='control-icon water-icon'>";
  html += "          <i class='material-icons'>water</i>";
  html += "        </div>";
  html += "        <div class='control-label'>Sulama Sistemi</div>";
  html += "        <div class='power-button' id='sulamaButton' onclick='toggleSulama()'>";
  html += "          <i class='material-icons'>power_settings_new</i>";
  html += "        </div>";
  html += "      </div>";
  html += "      <div class='control-item'>";
  html += "        <div class='control-icon light-icon'>";
  html += "          <i class='material-icons'>lightbulb</i>";
  html += "        </div>";
  html += "        <div class='control-label'>LED Aydınlatma</div>";
  html += "        <div class='power-button' id='ledButton' onclick='toggleLED()'>";
  html += "          <i class='material-icons'>power_settings_new</i>";
  html += "        </div>";
  html += "      </div>";
  html += "    </div>";
  html += "  </div>";
  
  // Ayarlar Kartı
  html += "<div class='card'>";
  html += "<div class='card-header'><i class='material-icons'>settings</i><h2>Ayarlar</h2></div>";
  html += "<div class='card-content'>";
  html += "<div class='input-group'>";
  html += "<label>Nem Eşik Değeri (%)</label>";
  html += "<input type='range' id='nemEsik' min='0' max='100' oninput='updateRangeValue(this)'>";
  html += "<div class='progress-bar'><div class='fill' id='nemEsikBar'></div></div>";
  html += "<span id='nemEsikValue'></span>";
  html += "</div>";
  html += "<div class='input-group'>";
  html += "<label>Sulama Süresi (saniye)</label>";
  html += "<input type='number' id='sulamaSuresi' min='1' max='300'>";
  html += "</div>";
  html += "<div class='input-group'>";
  html += "<label>LED Başlangıç Saati</label>";
  html += "<input type='time' id='ledBaslangic'>";
  html += "</div>";
  html += "<div class='input-group'>";
  html += "<label>LED Bitiş Saati</label>";
  html += "<input type='time' id='ledBitis'>";
  html += "</div>";
  html += "<button class='button primary' onclick='ayarlariKaydet()'><i class='material-icons'>save</i>Ayarları Kaydet</button>";
  html += "</div>";
  html += "</div>";
  
  // WiFi Ayarları Kartı
  html += "<div class='card'>";
  html += "<div class='card-header'><i class='material-icons'>wifi</i><h2>WiFi Ayarları</h2></div>";
  html += "<div class='card-content'>";
  html += "<button class='button primary' onclick='wifiTara()'><i class='material-icons'>refresh</i>WiFi Ağlarını Tara</button>";
  html += "<div id='wifiList' class='wifi-list'></div>";
  html += "</div>";
  html += "</div>";
  
  // Log Kartı
html += "<div class='card'>";
  html += "  <h3>📋 Sistem Günlüğü</h3>";
  html += "  <table class='log-table'>";
  html += "    <thead>";
  html += "      <tr>";
  html += "        <th>Tarih</th>";
  html += "        <th>Zaman</th>";
  html += "        <th>Tür</th>";
  html += "        <th>Açıklama</th>";
  html += "      </tr>";
  html += "    </thead>";
  html += "    <tbody id='logTableBody'>";
  html += "      <tr><td colspan='4'>Günlük yükleniyor...</td></tr>";
  html += "    </tbody>";
  html += "  </table>";
  html += "</div>";
  
  // Dosya Listesi Kartı
  html += "<div class='card'>";
  html += "<div class='card-header'><i class='material-icons'>folder</i><h2>Dosya Listesi</h2></div>";
  html += "<div class='card-content'>";
  html += "<button class='button primary' onclick='dosyaListesiGetir()'><i class='material-icons'>refresh</i>Dosya Listesini Getir</button>";
  html += "<div id='dosyaListesi' class='wifi-list'></div>";
  html += "</div>";
  html += "</div>";
  
  html += "</div>"; // grid end
  html += "</div>"; // container end
  
  html += "<div id='toast' class='toast'></div>";

  // JavaScript
  html += "<script>";
  html += "function updateStatus() {";
  html += "  fetch('/data').then(response => response.json()).then(data => {";
  html += "    let statusHtml = `";
  html += "      <div class='status-item'>";
  html += "        <i class='material-icons'>${data.nem < 30 ? 'warning' : 'check_circle'}</i>";
  html += "        <span class='label'>Nem Değeri:</span>";
  html += "        <span class='value'>${data.nem}%</span>";
  html += "        <div class='moisture-indicator' style='background-color: rgb(${data.nemColor.r}, ${data.nemColor.g}, ${data.nemColor.b})'></div>";
  html += "      </div>`;";
  html += "    statusHtml += `";
  html += "      <div class='status-item'>";
  html += "        <i class='material-icons'>${data.sulama ? 'opacity' : 'water_off'}</i>";
  html += "        <span class='label'>Sulama Durumu:</span>";
  html += "        <span class='value'>${data.sulama ? 'Aktif' : 'Pasif'}</span>";
  html += "      </div>`;";
  html += "    statusHtml += `";
  html += "      <div class='status-item'>";
  html += "        <i class='material-icons'>${data.led ? 'lightbulb' : 'lightbulb_outline'}</i>";
  html += "        <span class='label'>LED Durumu:</span>";
  html += "        <span class='value'>${data.led ? 'Açık' : 'Kapalı'}</span>";
  html += "      </div>`;";
  html += "    document.getElementById('statusCard').innerHTML = statusHtml;";
  html += "    document.getElementById('nemEsik').value = data.nemEsik;";
  html += "    document.getElementById('nemEsikValue').textContent = data.nemEsik + '%';";
  html += "    document.getElementById('nemEsikBar').style.width = data.nemEsik + '%';";
  html += "    document.getElementById('sulamaSuresi').value = data.sulamaSuresi;";
  html += "    document.getElementById('ledBaslangic').value = String(data.ledBaslangic).padStart(2,'0') + ':00';";
  html += "    document.getElementById('ledBitis').value = String(data.ledBitis).padStart(2,'0') + ':00';";
  html += "    updateButtonStates(data);";
  html += "  });";
  html += "}";
  
  html += "function updateButtonStates(data) {";
  html += "  const sulamaToggle = document.getElementById('sulamaToggle');";
  html += "  const ledToggle = document.getElementById('ledToggle');";
  html += "  sulamaToggle.checked = data.sulama;";
  html += "  ledToggle.checked = data.led;";
  html += "}";
  
  html += "function updateRangeValue(input) {";
  html += "  document.getElementById('nemEsikValue').textContent = input.value + '%';";
  html += "  document.getElementById('nemEsikBar').style.width = input.value + '%';";
  html += "}";
  
  html += "function showToast(message, type = 'success') {";
  html += "  const toast = document.getElementById('toast');";
  html += "  toast.textContent = message;";
  html += "  toast.className = 'toast show ' + type;";
  html += "  setTimeout(() => toast.className = 'toast', 3000);";
  html += "}";
  
  html += "function toggleSulama() {";
  html += "  const sulamaButton = document.getElementById('sulamaButton');";
  html += "  const isActive = sulamaButton.classList.contains('active');";
  
  html += "  fetch('/sulama?state=' + (isActive ? 'off' : 'on'), {";
  html += "    method: 'GET'";
  html += "  })";
  html += "    .then(response => response.json())";
  html += "    .then(data => {";
  html += "      console.log('Sulama durumu:', data.status);";
  
  html += "      if (data.status) {";
  html += "        sulamaButton.classList.add('active');";
  html += "        sulamaButton.classList.remove('inactive');";
  html += "        showToast('Sulama sistemi açıldı');";
  html += "        logEvent('SULAMA', 'Manuel sulama başlatıldı');";
  html += "      } else {";
  html += "        sulamaButton.classList.remove('active');";
  html += "        sulamaButton.classList.add('inactive');";
  html += "        showToast('Sulama sistemi kapatıldı');";
  html += "        logEvent('SULAMA', 'Manuel sulama durduruldu');";
  html += "      }";
  
  html += "      updateStatus();";
  html += "    })";
  html += "    .catch(error => {";
  html += "      console.error('Sulama hatası:', error);";
  html += "    });";
  html += "}";
  
  html += "function toggleLED() {";
  html += "  const ledButton = document.getElementById('ledButton');";
  html += "  const isActive = ledButton.classList.contains('active');";
  
  html += "  fetch('/led?state=' + (isActive ? 'off' : 'on'), {";
  html += "    method: 'GET'";
  html += "  })";
  html += "    .then(response => response.json())";
  html += "    .then(data => {";
  html += "      console.log('LED durumu:', data.status);";
  
  html += "      if (data.status) {";
  html += "        ledButton.classList.add('active');";
  html += "        ledButton.classList.remove('inactive');";
  html += "        showToast('LED sistemi açıldı');";
  html += "        logEvent('LED', 'Manuel LED açıldı');";
  html += "      } else {";
  html += "        ledButton.classList.remove('active');";
  html += "        ledButton.classList.add('inactive');";
  html += "        showToast('LED sistemi kapatıldı');";
  html += "        logEvent('LED', 'Manuel LED kapatıldı');";
  html += "      }";
  
  html += "      updateStatus();";
  html += "    })";
  html += "    .catch(error => {";
  html += "      console.error('LED hatası:', error);";
  html += "    });";
  html += "}";
  
  html += "function updateButtonStates(data) {";
  html += "  const sulamaButton = document.getElementById('sulamaButton');";
  html += "  const ledButton = document.getElementById('ledButton');";
  
  html += "  if (data.sulama) {";
  html += "    sulamaButton.classList.add('active');";
  html += "    sulamaButton.classList.remove('inactive');";
  html += "  } else {";
  html += "    sulamaButton.classList.remove('active');";
  html += "    sulamaButton.classList.add('inactive');";
  html += "  }";
  
  html += "  if (data.led) {";
  html += "    ledButton.classList.add('active');";
  html += "    ledButton.classList.remove('inactive');";
  html += "  } else {";
  html += "    ledButton.classList.remove('active');";
  html += "    ledButton.classList.add('inactive');";
  html += "  }";
  html += "}";
  
  html += "function ayarlariKaydet() {";
  html += "  const data = {";
  html += "    nemEsik: document.getElementById('nemEsik').value,";
  html += "    sulamaSuresi: document.getElementById('sulamaSuresi').value,";
  html += "    ledBaslangic: parseInt(document.getElementById('ledBaslangic').value),";
  html += "    ledBitis: parseInt(document.getElementById('ledBitis').value)";
  html += "  };";
  html += "  fetch('/ayarlar', {";
  html += "    method: 'POST',";
  html += "    headers: {'Content-Type': 'application/x-www-form-urlencoded'},";
  html += "    body: Object.keys(data).map(key => key + '=' + data[key]).join('&')";
  html += "  }).then(response => {";
  html += "    showToast('Ayarlar kaydedildi');";
  html += "    updateStatus();";
  html += "  });";
  html += "}";
  
  html += "function wifiTara() {";
  html += "  const wifiList = document.getElementById('wifiList');";
  html += "  wifiList.innerHTML = '<div class=\"status-item\"><i class=\"material-icons\">hourglass_empty</i>Ağlar taranıyor...</div>';";
  html += "  fetch('/wifi').then(response => response.json()).then(networks => {";
  html += "    let html = '';";
  html += "    networks.forEach(net => {";
  html += "      const strength = net.rssi > -50 ? 'wifi' : net.rssi > -70 ? 'wifi_4_bar' : net.rssi > -80 ? 'wifi_2_bar' : 'wifi_1_bar';";
  html += "      html += `<div class='wifi-item' onclick='wifiBaglan(\"${net.ssid}\")'>";
  html += "                <i class='material-icons'>${strength}</i>";
  html += "                <span class='ssid'>${net.ssid}</span>";
  html += "                <span class='strength'>${net.rssi}dBm</span>";
  html += "              </div>`;";
  html += "    });";
  html += "    wifiList.innerHTML = html || '<div class=\"status-item\"><i class=\"material-icons\">error_outline</i>Ağ bulunamadı</div>';";
  html += "  });";
  html += "}";
  
  html += "function wifiBaglan(ssid) {";
  html += "  const password = prompt('Ağ şifresini girin:');";
  html += "  if (password) {";
  html += "    fetch('/wifi/baglan', {";
  html += "      method: 'POST',";
  html += "      headers: {'Content-Type': 'application/x-www-form-urlencoded'},";
  html += "      body: `ssid=${encodeURIComponent(ssid)}&password=${encodeURIComponent(password)}`";
  html += "    }).then(response => {";
  html += "      showToast('Bağlantı ayarları kaydedildi. Cihaz yeniden başlatılıyor...', 'success');";
  html += "      setTimeout(() => location.reload(), 5000);";
  html += "    });";
  html += "  }";
  html += "}";
  
html += "function loadLogs() {";
  html += "  const logTableBody = document.getElementById('logTableBody');";
  html += "  logTableBody.innerHTML = '<tr><td colspan=\"4\"><i class=\"material-icons\">hourglass_empty</i> Günlük yükleniyor...</td></tr>';";
  
  html += "  fetch('/logs')";
  html += "    .then(response => {";
  html += "      if (!response.ok) {";
  html += "        throw new Error('Sunucudan yanıt alınamadı');";
  html += "      }";
  html += "      return response.json();";
  html += "    })";
  html += "    .then(data => {";
  html += "      console.log('Gelen günlük verisi:', data);";
  html += "      logTableBody.innerHTML = '';";
  
  html += "      if (data.logs && data.logs.length > 0) {";
  html += "        data.logs.reverse().forEach(log => {";
  html += "          const row = document.createElement('tr');";
  html += "          row.classList.add(`log-type-${log.type.toUpperCase().replace(' ', '')}`);";
  html += "          row.innerHTML = `";
  html += "            <td>${log.date}</td>";
  html += "            <td>${log.time}</td>";
  html += "            <td>${log.type}</td>";
  html += "            <td>${log.description}</td>`;";
  html += "          logTableBody.appendChild(row);";
  html += "        });";
  html += "      } else {";
  html += "        logTableBody.innerHTML = '<tr><td colspan=\"4\"><i class=\"material-icons\">error_outline</i> Günlük kaydı bulunamadı</td></tr>';";
  html += "      }";
  html += "    })";
  html += "    .catch(error => {";
  html += "      console.error('Günlük yükleme hatası:', error);";
  html += "      logTableBody.innerHTML = `<tr><td colspan=\"4\"><i class=\"material-icons\">error_outline</i> Hata: ${error.message}</td></tr>`;";
  html += "    });";
  html += "  }";
  
  html += "loadLogs();";
  html += "setInterval(loadLogs, 30000);";
  
  html += "function dosyaListesiGetir() {";
  html += "  const dosyaListesi = document.getElementById('dosyaListesi');";
  html += "  dosyaListesi.innerHTML = '<div class=\"status-item\"><i class=\"material-icons\">hourglass_empty</i>Dosya listesi yükleniyor...</div>';";
  html += "  fetch('/files')";
  html += "    .then(response => {";
  html += "      if (!response.ok) {";
  html += "        throw new Error('Sunucudan yanıt alınamadı');";
  html += "      }";
  html += "      return response.json();";
  html += "    })";
  html += "    .then(data => {";
  html += "      console.log('Gelen dosya listesi:', data);";  // Konsol çıktısı
  html += "      let html = '';";
  html += "      if (data.files && data.files.length > 0) {";  // data.files kontrolü
  html += "        data.files.forEach(file => {";  // data.files üzerinde döngü
  html += "          html += `<div class='wifi-item'>";
  html += "                    <i class='material-icons'>${file.type === 'file' ? 'insert_drive_file' : 'folder'}</i>";
  html += "                    <span class='ssid'>${file.name}</span>";
  html += "                    <span class='strength'>${file.size}</span>";
  html += "                  </div>`;";
  html += "        });";
  html += "        dosyaListesi.innerHTML = html;";
  html += "      } else {";
  html += "        dosyaListesi.innerHTML = '<div class=\"status-item\"><i class=\"material-icons\">error_outline</i>Dosya bulunamadı</div>';";
  html += "      }";
  html += "    })";
  html += "    .catch(error => {";
  html += "      console.error('Dosya listesi yükleme hatası:', error);";
  html += "      dosyaListesi.innerHTML = `<div class=\"status-item\"><i class=\"material-icons\">error_outline</i> Hata: ${error.message}</div>`;";
  html += "    });";
  html += "};";
  
  html += "setInterval(updateStatus, 5000);";
  html += "updateStatus();";
  html += "</script></body></html>";
  
  server.send(200, "text/html", html);
}

void handleData() {
  String json = "{";
  int nemDegeri = analogRead(NEM_SENSOR_PIN);
  nemDegeri = map(nemDegeri, 0, 1023, 0, 100);
  
  RGBColor color = getNemColor(nemDegeri);
  
  json += "\"nem\":" + String(nemDegeri) + ",";
  json += "\"sulama\":" + String(digitalRead(SULAMA_ROLE_PIN) == LOW) + ",";
  json += "\"led\":" + String(digitalRead(LED_ROLE_PIN) == LOW) + ",";
  json += "\"nemEsik\":" + String(settings.nemEsik) + ",";
  json += "\"sulamaSuresi\":" + String(settings.sulamaSuresi) + ",";
  json += "\"ledBaslangic\":" + String(settings.ledBaslangic) + ",";
  json += "\"ledBitis\":" + String(settings.ledBitis) + ",";
  json += "\"nemColor\": {\"r\":" + String(color.r) + ",\"g\":" + String(color.g) + ",\"b\":" + String(color.b) + "}";
  json += "}";
  
  server.send(200, "application/json", json);
}

void handleAyarlar() {
  if (server.method() != HTTP_POST) {
    server.send(405, "text/plain", "Method Not Allowed");
    return;
  }

  settings.nemEsik = server.arg("nemEsik").toInt();
  settings.sulamaSuresi = server.arg("sulamaSuresi").toInt();
  settings.ledBaslangic = server.arg("ledBaslangic").toInt();
  settings.ledBitis = server.arg("ledBitis").toInt();

  if (saveSettings()) {
    server.send(200, "text/plain", "OK");
  } else {
    server.send(500, "text/plain", "Ayarlar kaydedilemedi");
  }
}

void handleNotFound() {
  server.send(404, "text/plain", "Sayfa bulunamadı");
}

// Dosya boyutunu okunabilir formata çeviren yardımcı fonksiyon
String formatFileSize(size_t bytes) {
  if (bytes < 1024) return String(bytes) + " B";
  else if (bytes < 1024 * 1024) return String(bytes / 1024.0, 2) + " KB";
  else return String(bytes / (1024.0 * 1024.0), 2) + " MB";
}

// LittleFS dosya listesi alma fonksiyonu
void handleFileList() {
  DynamicJsonDocument doc(4096);  // Daha büyük JSON boyutu
  JsonArray files = doc.createNestedArray("files");
  
  // Hata ayıklama için seri port çıktısı
  Serial.println("Dosya listesi alınıyor...");
  
  int fileCount = 0;
  Dir dir = LittleFS.openDir("/");
  while (dir.next()) {
    JsonObject fileInfo = files.createNestedObject();
    fileInfo["name"] = dir.fileName();
    fileInfo["size"] = formatFileSize(dir.fileSize());
    fileInfo["type"] = dir.isFile() ? "file" : "directory";
    
    fileCount++;
    Serial.printf("Dosya: %s, Boyut: %s, Tip: %s\n", 
                  dir.fileName().c_str(), 
                  formatFileSize(dir.fileSize()).c_str(), 
                  (dir.isFile() ? "Dosya" : "Klasör"));
  }
  
  Serial.printf("Toplam %d dosya/klasör bulundu.\n", fileCount);
  
  String jsonResponse;
  serializeJson(doc, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

// Dosya içeriğini okuma fonksiyonu
void handleFileRead() {
  if (!server.hasArg("filename")) {
    server.send(400, "text/plain", "Dosya adı belirtilmedi");
    return;
  }
  
  String filename = server.arg("filename");
  
  if (!LittleFS.exists(filename)) {
    server.send(404, "text/plain", "Dosya bulunamadı");
    return;
  }
  
  File file = LittleFS.open(filename, "r");
  if (!file) {
    server.send(500, "text/plain", "Dosya açılamadı");
    return;
  }
  
  String fileContent = file.readString();
  file.close();
  
  server.send(200, "text/plain", fileContent);
}

// Dosya kaydetme fonksiyonu
void handleFileSave() {
  if (!server.hasArg("filename") || !server.hasArg("content")) {
    server.send(400, "text/plain", "Eksik parametreler");
    return;
  }
  
  String filename = server.arg("filename");
  String content = server.arg("content");
  
  File file = LittleFS.open(filename, "w");
  if (!file) {
    server.send(500, "text/plain", "Dosya oluşturulamadı");
    return;
  }
  
  file.print(content);
  file.close();
  
  server.send(200, "text/plain", "Dosya başarıyla kaydedildi");
}

// Dosya silme fonksiyonu
void handleFileDelete() {
  if (!server.hasArg("filename")) {
    server.send(400, "text/plain", "Dosya adı belirtilmedi");
    return;
  }
  
  String filename = server.arg("filename");
  
  if (!LittleFS.exists(filename)) {
    server.send(404, "text/plain", "Dosya bulunamadı");
    return;
  }
  
  if (LittleFS.remove(filename)) {
    server.send(200, "text/plain", "Dosya silindi");
  } else {
    server.send(500, "text/plain", "Dosya silinemedi");
  }
}

void handleSulama() {
  // GET veya POST parametrelerini kontrol et
  String durum = "";
  
  // GET parametresini kontrol et
  if (server.hasArg("state")) {
    durum = server.arg("state");
  } 
  // POST parametresini kontrol et
  else if (server.hasArg("durum")) {
    durum = server.arg("durum");
  }
  
  bool sulamaAktif = false;
  
  if (durum == "on" || durum == "true") {
    sulamaAktif = true;
    sulamaBaslat();
    logEvent("SULAMA", "M sulama on");
  } else if (durum == "off" || durum == "false") {
    sulamaAktif = false;
    sulamaDurdur();
    logEvent("SULAMA", "M sulama off");
  }
  
  // JSON yanıtı hazırla
  DynamicJsonDocument doc(200);
  doc["status"] = sulamaAktif;
  
  String jsonResponse;
  serializeJson(doc, jsonResponse);
  
  server.send(200, "application/json", jsonResponse);
}

void handleLed() {
  // GET veya POST parametrelerini kontrol et
  String durum = "";
  
  // GET parametresini kontrol et
  if (server.hasArg("state")) {
    durum = server.arg("state");
  } 
  // POST parametresini kontrol et
  else if (server.hasArg("durum")) {
    durum = server.arg("durum");
  }
  
  bool ledAktif = false;
  
  if (durum == "on" || durum == "true") {
    ledAktif = true;
    ledAc();
    logEvent("LED", "M LED on");
  } else if (durum == "off" || durum == "false") {
    ledAktif = false;
    ledKapat();
    logEvent("LED", "M LED off");
  }
  
  // JSON yanıtı hazırla
  DynamicJsonDocument doc(200);
  doc["status"] = ledAktif;
  
  String jsonResponse;
  serializeJson(doc, jsonResponse);
  
  server.send(200, "application/json", jsonResponse);
}

void handleLogs() {
  // LittleFS başlatma kontrolü
  if (!LittleFS.begin()) {
    server.send(500, "application/json", "{\"error\":\"Dosya sistemi başlatılamadı\"}");
    return;
  }
  
  // Log dosyası yolu
  const char* logFilePath = "/logs/system_log.txt";
  
  // Dosya varlık kontrolü
  if (!LittleFS.exists(logFilePath)) {
    server.send(404, "application/json", "{\"error\":\"Log dosyası bulunamadı\"}");
    return;
  }
  
  // Dosyayı aç
  File logFile = LittleFS.open(logFilePath, "r");
  if (!logFile) {
    server.send(500, "application/json", "{\"error\":\"Log dosyası açılamadı\"}");
    return;
  }
  
  // JSON hazırlama
  DynamicJsonDocument doc(4096);
  JsonArray logs = doc.createNestedArray("logs");
  
  // Dosya içeriğini satır satır oku
  String logContent = logFile.readString();
  logFile.close();
  
  // Son 5 log kaydını al
  int lineCount = 0;
  int lineStart = logContent.length() - 1;
  
  while (lineStart > 0 && lineCount < 5) {
    int lineEnd = logContent.lastIndexOf('\n', lineStart);
    if (lineEnd == -1) lineEnd = 0;
    
    String logLine = logContent.substring(lineEnd, lineStart);
    int firstSep = logLine.indexOf('|');
    int secondSep = logLine.indexOf('|', firstSep + 1);
    
    if (firstSep != -1 && secondSep != -1) {
      JsonObject log = logs.createNestedObject();
      
      // Zaman bilgisini al
      String timestamp = trimString(logLine.substring(0, firstSep));
      
      // Tarih bilgisini ekle (varsayılan olarak bugünün tarihi)
      String currentDate = getFormattedDate();
      
      // Olay türünü Türkçeleştir
      String eventType = trimString(logLine.substring(firstSep + 1, secondSep));
      String turkceEventType;
      
      if (eventType == "SULAMA") {
        turkceEventType = "Sulama";
      } else if (eventType == "LED") {
        turkceEventType = "LED";
      } else {
        turkceEventType = eventType;
      }
      
      // Açıklamayı al
      String description = trimString(logLine.substring(secondSep + 1));
      
      log["date"] = currentDate;
      log["time"] = timestamp;
      log["type"] = turkceEventType;
      log["description"] = description;
      
      lineCount++;
    }
    
    lineStart = lineEnd - 1;
  }
  
  // JSON yanıtı gönder
  String jsonResponse;
  serializeJson(doc, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

// Tarih formatını al
String getFormattedDate() {
  // RTC modülü varsa gerçek tarih
  #ifdef USE_RTC
    DateTime now = rtc.now();
    char dateBuffer[11];
    snprintf(dateBuffer, sizeof(dateBuffer), "%02d.%02d.%04d", 
             now.day(), now.month(), now.year());
    return String(dateBuffer);
  #else
    // Varsayılan olarak bugünün tarihi
    return "20.12.2024";  // Örnek tarih
  #endif
}

void handleWiFiScan() {
  String json = "[";
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; ++i) {
    if (i) json += ",";
    json += "{\"ssid\":\"" + WiFi.SSID(i) + "\",\"rssi\":" + String(WiFi.RSSI(i)) + "}";
  }
  json += "]";
  server.send(200, "application/json", json);
}

void handleWiFiConnect() {
  if (server.hasArg("ssid") && server.hasArg("password")) {
    String newSsid = server.arg("ssid");
    String newPassword = server.arg("password");
    
    strlcpy(wifiConfig.ssid, newSsid.c_str(), sizeof(wifiConfig.ssid));
    strlcpy(wifiConfig.password, newPassword.c_str(), sizeof(wifiConfig.password));
    saveWiFiConfig();
    
    server.send(200, "text/plain", "OK");
    delay(1000);
    ESP.restart();
  } else {
    server.send(400, "text/plain", "Eksik parametreler");
  }
}

// Boşluk temizleme fonksiyonu
String trimString(const String& str) {
  int start = 0;
  int end = str.length() - 1;
  
  while (start <= end && isspace(str.charAt(start))) {
    start++;
  }
  
  while (end >= start && isspace(str.charAt(end))) {
    end--;
  }
  
  return str.substring(start, end + 1);
}

// Zaman damgası oluşturma fonksiyonu
String getTimestamp() {
  // Eğer RTC modülü kullanılıyorsa, gerçek zamandan al
  #ifdef USE_RTC
    // RTC'den gerçek zamanı al
    DateTime now = rtc.now();
    char timestamp[20];
    snprintf(timestamp, sizeof(timestamp), "%02d:%02d:%02d", 
             now.hour(), now.minute(), now.second());
    return String(timestamp);
  #else
    // Cihaz açık kaldığı süreden zaman hesapla
    unsigned long currentMillis = millis();
    unsigned long seconds = currentMillis / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;
    
    char timestamp[20];
    snprintf(timestamp, sizeof(timestamp), "%02lu:%02lu:%02lu", 
             hours % 24, minutes % 60, seconds % 60);
    
    return String(timestamp);
  #endif
}

void logEvent(const String& eventType, const String& description) {
  // Seri port hızını kontrol et
  if (!Serial) {
    Serial.begin(115200);
    delay(100);
  }
  
  // LittleFS başlatma kontrolü
  if (!LittleFS.begin()) {
    Serial.println("HATA: LittleFS başlatılamadı - Log kaydedilemedi!");
    return;
  }
  
  // Log dizini kontrolü
  if (!LittleFS.exists("/logs")) {
    if (!LittleFS.mkdir("/logs")) {
      Serial.println("HATA: '/logs' dizini oluşturulamadı!");
      return;
    }
  }
  
  const char* logFilePath = "/logs/system_log.txt";
  
  // Dosya boyutu kontrolü
  FSInfo fs_info;
  LittleFS.info(fs_info);
  
  // Dosya sistemi doluluk kontrolü
  if (fs_info.usedBytes >= fs_info.totalBytes * 0.9) {
    Serial.println("UYARI: Dosya sistemi %90 dolu - Eski loglar silinecek");
    
    // En eski logları sil
    Dir dir = LittleFS.openDir("/logs");
    while (dir.next()) {
      LittleFS.remove(dir.fileName());
      Serial.printf("Silinen dosya: %s\n", dir.fileName().c_str());
      
      // Tekrar kontrol et
      LittleFS.info(fs_info);
      if (fs_info.usedBytes < fs_info.totalBytes * 0.7) {
        break;
      }
    }
  }
  
  // Log dosyası oluşturma
  File logFile = LittleFS.open(logFilePath, "a+");
  if (!logFile) {
    Serial.println("HATA: Log dosyası açılamadı!");
    return;
  }
  
  // Log kaydı oluşturma
  String timestamp = getTimestamp();
  String logEntry = timestamp + " | " + eventType + " | " + description + "\n";
  
  // Dosyaya yazma
  size_t bytesWritten = logFile.print(logEntry);
  logFile.close();
  
  // Yazma kontrolü
  if (bytesWritten > 0) {
    Serial.print("Log kaydedildi: ");
    Serial.print(logEntry);
  } else {
    Serial.println("HATA: Log dosyasına yazma başarısız!");
  }
}

// Sulama sistemini başlatma fonksiyonu
void sulamaBaslat() {
  digitalWrite(SULAMA_ROLE_PIN, LOW);  // Röle aktif LOW
  #ifdef DEBUG
    Serial.println("Sulama sistemi başlatıldı");
  #endif
}

// Sulama sistemini durdurma fonksiyonu
void sulamaDurdur() {
  digitalWrite(SULAMA_ROLE_PIN, HIGH);  // Röle pasif HIGH
  #ifdef DEBUG
    Serial.println("Sulama sistemi durduruldu");
  #endif
}

// LED'i açma fonksiyonu
void ledAc() {
  digitalWrite(LED_ROLE_PIN, LOW);  // Röle aktif LOW
  #ifdef DEBUG
    Serial.println("LED sistemi açıldı");
  #endif
}

// LED'i kapatma fonksiyonu
void ledKapat() {
  digitalWrite(LED_ROLE_PIN, HIGH);  // Röle pasif HIGH
  #ifdef DEBUG
    Serial.println("LED sistemi kapatıldı");
  #endif
}


// File Sistem

// Varsayılan HTML içeriğini tutan sabit değişken
// Varsayılan HTML içeriğini tutan sabit değişken
const char* defaultHTML PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="tr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>LittleFS Dosya Yöneticisi</title>
    <style>
        body{font-family:'Segoe UI',Arial,sans-serif;max-width:800px;margin:0 auto;padding:20px;background-color:#f5f5f5}
        .file-list{background:white;border-radius:8px;padding:20px;box-shadow:0 2px 4px rgba(0,0,0,0.1)}
        .file-item{display:flex;justify-content:space-between;align-items:center;padding:10px;border-bottom:1px solid #eee}
        .upload-form{background:white;padding:20px;border-radius:8px;margin-bottom:20px;box-shadow:0 2px 4px rgba(0,0,0,0.1)}
        button{background-color:#4CAF50;color:white;padding:8px 16px;border:none;border-radius:4px;cursor:pointer}
        button.delete{background-color:#f44336}
        .progress{margin-top:10px;height:20px;background-color:#f0f0f0;border-radius:10px;overflow:hidden}
        .progress-bar{height:100%;background-color:#4CAF50;width:0%;transition:width 0.3s}
    </style>
</head>
<body>
    <h1>LittleFS Dosya Yöneticisi</h1>
    <div class="upload-form">
        <h2>Dosya Yükle</h2>
        <input type="file" id="fileInput">
        <button onclick="uploadFile()">Yükle</button>
        <div class="progress">
            <div class="progress-bar" id="progressBar"></div>
        </div>
    </div>
    <div class="file-list" id="fileList">
        <h2>Dosyalar</h2>
    </div>
    <script>
        function loadFileList(){fetch('/list').then(response=>response.json()).then(files=>{const fileList=document.getElementById('fileList');let html='<h2>Dosyalar</h2>';files.forEach(file=>{html+=`<div class="file-item"><span>${file.name} (${formatBytes(file.size)})</span><button class="delete" onclick="deleteFile('${file.name}')">Sil</button></div>`;});fileList.innerHTML=html;});}
        function uploadFile(){const fileInput=document.getElementById('fileInput');const file=fileInput.files[0];if(!file)return;const formData=new FormData();formData.append('file',file);const xhr=new XMLHttpRequest();xhr.open('POST','/upload',true);xhr.upload.onprogress=(e)=>{if(e.lengthComputable){const percentComplete=(e.loaded/e.total)*100;document.getElementById('progressBar').style.width=percentComplete+'%';}};xhr.onload=()=>{if(xhr.status===200){loadFileList();fileInput.value='';document.getElementById('progressBar').style.width='0%';}};xhr.send(formData);}
        function deleteFile(filename){if(confirm(`${filename} dosyasını silmek istediğinizden emin misiniz?`)){fetch('/delete',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:`filename=${encodeURIComponent(filename)}`}).then(()=>loadFileList());}}
        function formatBytes(bytes){if(bytes===0)return'0 Bytes';const k=1024;const sizes=['Bytes','KB','MB','GB'];const i=Math.floor(Math.log(bytes)/Math.log(k));return parseFloat((bytes/Math.pow(k,i)).toFixed(2))+' '+sizes[i];}
        loadFileList();
    </script>
</body>
</html>)rawliteral";


// Dosya yöneticisi HTML sayfasını gönder
void handleFileManager() {
  if (!LittleFS.exists("/filemanager.html")) {
    // Dosya yoksa oluştur
    File file = LittleFS.open("/filemanager.html", "w");
    if (file) {
      file.print(FPSTR(defaultHTML));
      file.close();
      Serial.println("filemanager.html oluşturuldu");
    } else {
      server.send(500, "text/plain", "Dosya oluşturulamadı!");
      return;
    }
  }
  
  File file = LittleFS.open("/filemanager.html", "r");
  if (!file) {
    server.send(404, "text/plain", "Dosya bulunamadı!");
    return;
  }
  server.streamFile(file, "text/html");
  file.close();
}

// Dosya listesini JSON formatında döndür (güncellenmiş versiyon)
void handleFileListfs() {
  String path = "/";
  if (server.hasArg("dir")) {
    path = server.arg("dir");
    if (!path.startsWith("/")) path = "/" + path;
  }
  // Path'in sonunda / yoksa ekle
  if (!path.endsWith("/") && path != "/") {
    path += "/";
  }

  Dir dir = LittleFS.openDir(path);
  String output = "[";
  while (dir.next()) {
    if (output != "[") output += ',';
    output += "{\"name\":\"" + dir.fileName() + "\",";
    output += "\"size\":" + String(dir.fileSize()) + ",";
    // Klasör kontrolü
    File f = dir.openFile("r");
    bool isDir = f.isDirectory();
    f.close();
    output += "\"isDir\":" + String(isDir ? "true" : "false") + ",";
    // Path sonunda / varsa tekrar ekleme
    String fullPath = (path == "/" ? "/" : path) + dir.fileName();
    output += "\"path\":\"" + fullPath + "\"}";
  }
  output += "]";
  server.send(200, "application/json", output);
}

// Dosya yükleme işlemi
void handleFileUpload() {
  HTTPUpload& upload = server.upload();
  static File fsUploadFile;
  
  if (upload.status == UPLOAD_FILE_START) {
    String filename = "/" + upload.filename;
    fsUploadFile = LittleFS.open(filename, "w");
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile) {
      fsUploadFile.close();
    }
  }
}

// Dosya silme işlemi
void handleDelete() {
  if (!server.hasArg("filename")) {
    server.send(400, "text/plain", "filename parametresi eksik");
    return;
  }
  
  String filename = "/" + server.arg("filename");
  if (LittleFS.remove(filename)) {
    server.send(200, "text/plain", "Dosya silindi");
  } else {
    server.send(500, "text/plain", "Dosya silinemedi");
  }
}

// Dosya içeriğini okuma API'si
void handleFileReadfs() {
  if (!server.hasArg("path")) {
    server.send(400, "text/plain", "path parametresi eksik");
    return;
  }
  
  String path = server.arg("path");
  if (!path.startsWith("/")) path = "/" + path;
  
  if (LittleFS.exists(path)) {
    File file = LittleFS.open(path, "r");
    server.send(200, "text/plain", file.readString());
    file.close();
  } else {
    server.send(404, "text/plain", "Dosya bulunamadı");
  }
}

// Dosya içeriğini kaydetme API'si
void handleFileSavefs() {
  if (!server.hasArg("path") || !server.hasArg("content")) {
    server.send(400, "text/plain", "path veya content parametresi eksik");
    return;
  }
  
  String path = server.arg("path");
  if (!path.startsWith("/")) path = "/" + path;
  String content = server.arg("content");
  
  File file = LittleFS.open(path, "w");
  if (file) {
    file.print(content);
    file.close();
    server.send(200, "text/plain", "Dosya kaydedildi");
  } else {
    server.send(500, "text/plain", "Dosya kaydedilemedi");
  }
} 


