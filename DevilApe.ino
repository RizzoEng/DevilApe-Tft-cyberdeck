#include <TFT_eSPI.h>
#include <WiFi.h>
#include "AiEsp32RotaryEncoder.h"
#include "esp_wifi.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#define BUZZER_PIN 22
#define BUZZER_CHAN 0
#define ENCODER_CLK 34
#define ENCODER_DT  35
#define ENCODER_SW  21

#define R_YELLOW  0xFFE0
#define R_MAGENTA 0xF81F
#define R_BLACK   0x0000
#define R_BLUE    0x001F
#define R_RED     0xF800

TFT_eSPI tft = TFT_eSPI();
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ENCODER_CLK, ENCODER_DT, ENCODER_SW, -1, 4);

/* -------- PERSISTENT DATA -------- */
int currentTab = 0;
int n_networks = 0, n_ble = 0;
int netScroll = 0, bleScroll = 0;
unsigned long lastScan = 0;
unsigned long scanTime = 0; 
int lastSpeed = 0, lastRSSI = -100;
bool isActive = false;

int deauthCount = 0;
int packetCount = 0; 
int lastPktCount = 0; // For alert logic
String lastDeauthTarget = "NONE";
unsigned long lastHop = 0;
int currentChannel = 1;

String systemLogs[10];
int logCount = 0;

int pktHistory[216]; 
int historyCount = 0;

struct BleResult { String name; String addr; int rssi; };
BleResult bleResults[10];

String tabNames[] = {
  "SPECTRUM",
  "PACKET-MONITOR",
  "CYBER-CORE",
  "RADAR-SCAN",
  "BLE_CAPTURE",
  "NET_RUNNER",
  "DEAUTH-SNIFFER",
  "BEACON-FLOOD",
  "SYSTEM_LOGS"
};
const int totalTabs = 9;

String floodSSIDs[] = {
  "X_HACKED_X", "SYSTEM_FAIL", "NULL_PTR", "DARK_NET", 
  "DEVIL_APE", "BIO_HAZARD", "VOID_SIG", "ROOT_ACCESS",
  "SQL_INJECT", "OVERFLOW", "KERBEROS", "MAN_IN_MID",
  "DOX_DATA", "GRID_KILL", "PHANTOM", "CYBER_PUNK"
};

uint8_t beaconPacket[128] = {
  0x80, 0x00, 0x00, 0x00, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x64, 0x00, 0x01, 0x00, 
  0x00, 0x00 
};

void addLog(String msg) {
  for (int i = 9; i > 0; i--) systemLogs[i] = systemLogs[i - 1];
  systemLogs[0] = "[" + String(millis() / 1000) + "s] " + msg;
  if (logCount < 10) logCount++;
}

void sniffer(void* buf, wifi_promiscuous_pkt_type_t type) {
  packetCount++; 
  if (type != WIFI_PKT_MGMT) return;
  const wifi_promiscuous_pkt_t* pkt = (wifi_promiscuous_pkt_t*)buf;
  const uint8_t* frame = pkt->payload;
  if (frame[0] == 0xC0 || frame[0] == 0xA0) {
    deauthCount++;
    char addr[18];
    sprintf(addr, "%02X:%02X:%02X:%02X:%02X:%02X", frame[4], frame[5], frame[6], frame[7], frame[8], frame[9]);
    lastDeauthTarget = String(addr);
    ledcWriteTone(BUZZER_CHAN, 3000);
  }
}

void batTone(int f, int d) {
  ledcWriteTone(BUZZER_CHAN, f);
  delay(d);
  ledcWriteTone(BUZZER_CHAN, 0);
}

void bootSequence() {
  tft.fillScreen(R_BLACK);
  tft.setTextColor(R_YELLOW);
  tft.drawCentreString("SYSTEM INITIALIZED", 120, 140, 2);
  tft.drawRect(40, 165, 160, 10, R_YELLOW);
  for(int i=0; i<158; i++) {
    tft.fillRect(41, 166, i, 8, R_YELLOW);
    delay(8);
  }
  tft.fillScreen(R_BLACK);
}

void setup() {
  ledcSetup(BUZZER_CHAN, 2000, 8);
  ledcAttachPin(BUZZER_PIN, BUZZER_CHAN);
  tft.begin();
  tft.setRotation(2);   
  rotaryEncoder.begin();
  rotaryEncoder.setup([] { rotaryEncoder.readEncoder_ISR(); });
  rotaryEncoder.setBoundaries(0, totalTabs - 1, true);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  BLEDevice::init("");
  for(int i=0; i<216; i++) pktHistory[i] = 275; 
  addLog("SYS: BOOT OK");
  bootSequence();
  drawFullFrame();
}

void loop() {
  if (!isActive) {
    if (rotaryEncoder.encoderChanged()) {
      currentTab = rotaryEncoder.readEncoder();
      batTone(1200, 10);
      drawFullFrame();
    }
  } else {
    int change = rotaryEncoder.encoderChanged();
    if (change != 0) {
      if (currentTab == 5) netScroll = constrain(netScroll + (change > 0 ? 1 : -1), 0, max(0, n_networks - 1));
      if (currentTab == 4) bleScroll = constrain(bleScroll + (change > 0 ? 1 : -1), 0, max(0, n_ble - 1));
      batTone(1500, 5);
      renderModeContent(); 
    }
  }

  if (rotaryEncoder.isEncoderButtonClicked()) {
    isActive = !isActive;
    addLog(isActive ? "START: " + tabNames[currentTab] : "STOP");
    if (isActive && (currentTab == 1 || currentTab == 6)) {
      esp_wifi_set_promiscuous(true);
      esp_wifi_set_promiscuous_rx_cb(&sniffer);
    } else {
      esp_wifi_set_promiscuous(false);
    }
    batTone(isActive ? 2500 : 800, 50);
    drawFullFrame();
  }

  // Monitor History & Alert Logic
  if (isActive && currentTab == 1) {
    static unsigned long lastPktUpdate = 0;
    if (millis() - lastPktUpdate > 500) {
      lastPktCount = packetCount; 
      if(historyCount < 216) historyCount++;
      for(int i=0; i<215; i++) pktHistory[i] = pktHistory[i+1];
      pktHistory[215] = map(constrain(packetCount, 0, 100), 0, 100, 275, 75);
      
      if (packetCount > 150) { // Alert Threshold
        ledcWriteTone(BUZZER_CHAN, 4500); 
        addLog("ALERT: TRAFFIC SURGE");
      } else {
        ledcWriteTone(BUZZER_CHAN, 0);
      }

      packetCount = 0;
      lastPktUpdate = millis();
      renderModeContent();
    }
  }

  if (isActive && currentTab == 6) {
    if (millis() - lastHop > 250) {
      currentChannel++;
      if (currentChannel > 13) currentChannel = 1;
      esp_wifi_set_channel(currentChannel, WIFI_SECOND_CHAN_NONE);
      lastHop = millis();
      renderModeContent();
      ledcWriteTone(BUZZER_CHAN, 0);
    }
  }

  if (isActive && currentTab == 7) {
    static int ssidIdx = 0;
    if (millis() - lastHop > 100) {
      String currentSSID = floodSSIDs[ssidIdx];
      beaconPacket[37] = currentSSID.length();
      for(int i=0; i<currentSSID.length(); i++) beaconPacket[38+i] = currentSSID[i];
      esp_wifi_80211_tx(WIFI_IF_STA, beaconPacket, 38 + currentSSID.length(), false);
      ssidIdx = (ssidIdx + 1) % 16; 
      lastHop = millis();
      if(ssidIdx % 4 == 0) renderModeContent(); 
    }
  }

  if (isActive && (millis() - lastScan > 2000) && currentTab != 1 && currentTab < 6) {
    unsigned long start = millis();
    n_networks = WiFi.scanNetworks();
    if(n_networks > 0) lastRSSI = WiFi.RSSI(0);
    scanTime = millis() - start;
    lastScan = millis();
    if (isActive) renderModeContent();
  }
  if (isActive && currentTab == 3) drawProximityRadar();
}

void drawFullFrame() {
  tft.fillScreen(R_BLACK);
  tft.drawRect(0, 0, 240, 320, R_YELLOW);
  tft.setTextColor(R_YELLOW, R_BLACK);
  tft.drawCentreString("DEVILAPE_CYBERDECK", 120, 10, 2);
  tft.drawLine(10, 30, 230, 30, R_YELLOW);
  if (!isActive) {
    for(int i=0; i < totalTabs; i++) {
      int y = 45 + (i * 26);
      if(i == currentTab) {
        tft.setTextColor(R_YELLOW, R_BLACK);
        tft.drawString(">", 20, y, 2);
        tft.drawString(tabNames[i], 45, y, 2);
      } else {
        tft.setTextColor(R_MAGENTA, R_BLACK);
        tft.drawString(tabNames[i], 45, y, 2);
      }
    }
    drawMenuFooter(); 
  } else {
    tft.drawRect(10, 40, 220, 260, R_YELLOW);
    tft.fillRect(11, 41, 218, 20, R_YELLOW);
    tft.setTextColor(R_BLACK, R_YELLOW);
    tft.drawCentreString(tabNames[currentTab], 120, 43, 2);
    tft.setTextColor(R_MAGENTA, R_BLACK);
  }
}

void drawMenuFooter() {
  tft.drawLine(15, 290, 225, 290, R_YELLOW);
  tft.setTextColor(R_YELLOW, R_BLACK);
  tft.drawString("BATT: 4.2V", 20, 300, 1);
  tft.drawString("CPU: 38C", 100, 300, 1);
  tft.drawString("UP: " + String(millis()/1000) + "s", 180, 300, 1);
}

void renderModeContent() {
  if (currentTab < 6) tft.fillRect(12, 62, 216, 236, R_BLACK); 
  switch(currentTab) {
    case 0: drawSpectralGrid(); break;
    case 1: drawPacketMonitor(); break;
    case 2: drawCoreTelemetry(); break;
    case 3: break; 
    case 4: drawGhostScanner(); break;
    case 5: drawNetHunter(); break;
    case 6: drawDeauthWatch(); break;
    case 7: drawWifiBeacon(); break;
    case 8: drawSystemLogs(); break;
  }
}

void drawSpectralGrid() {
  int channels[15]={0};
  for(int i=0; i<n_networks; i++) { int ch=WiFi.channel(i); if(ch<15) channels[ch]++; }
  for(int i=1; i<14; i++){
    int y = 70 + (i * 16);
    int barLen = map(constrain(channels[i], 0, 10), 0, 10, 0, 160);
    tft.setTextColor(R_MAGENTA); tft.drawNumber(i, 20, y, 1);
    tft.fillRect(45, y, barLen, 10, R_MAGENTA);
  }
}

void drawPacketMonitor() {
  tft.setTextColor(R_RED);
  tft.drawString("MAX PKT", 15, 65, 2);
  tft.setTextColor(R_BLUE);
  tft.drawString("MIN PKT", 15, 275, 2);
  if (lastPktCount > 150) {
    tft.setTextColor(R_RED);
    tft.drawCentreString("TRAFFIC SURGE", 120, 150, 2);
  }
  for(int g=65; g<=285; g+=44) tft.drawLine(15, g, 225, g, 0x2104);
  if (historyCount < 2) return;
  float stepX = 210.0 / (historyCount - 1);
  for(int i=1; i<historyCount; i++) {
    int x0 = 15 + (int)((i-1) * stepX); int x1 = 15 + (int)(i * stepX);
    int y0 = pktHistory[216-historyCount+i-1]; int y1 = pktHistory[216-historyCount+i];
    tft.drawLine(x0, y0, x1, y1, (y1 < y0) ? R_RED : R_BLUE); 
  }
}

void drawCoreTelemetry() {
  tft.setTextColor(R_MAGENTA);
  tft.setTextSize(1);
  tft.setCursor(30, 75); 
  tft.print("NODE LOCK: "); tft.println(n_networks > 0 ? WiFi.SSID(0).substring(0,12) : "SEARCHING");
  tft.setCursor(30, 95); tft.printf("PING RTT: %lu ms\n", scanTime/2);
  tft.setCursor(30, 115); tft.printf("PACKET LOSS: %d.0%%\n", (lastRSSI<-88)?4:0);
  tft.drawLine(25, 135, 215, 135, R_MAGENTA);
  tft.setCursor(30, 150); tft.printf("DL SPEED: %d Kbps\n", lastSpeed);
  tft.setCursor(30, 170); tft.printf("UL SPEED: %d Kbps\n", lastSpeed/4);
  tft.fillRect(20, 215, 200, 45, R_MAGENTA); tft.setTextColor(R_BLACK);
  tft.drawCentreString("NOMINAL", 120, 228, 2);
}

void drawProximityRadar() {
  int cx=120, cy=170; static float angle = 0;
  tft.drawLine(cx, cy, cx+cos(angle)*80, cy+sin(angle)*80, R_BLACK);
  angle += 0.02; if(angle >= 6.28) angle = 0; 
  tft.drawCircle(cx, cy, 80, R_YELLOW); tft.drawCircle(cx, cy, 40, R_YELLOW);
  tft.drawLine(cx, cy, cx+cos(angle)*80, cy+sin(angle)*80, R_YELLOW);
  tft.setTextColor(R_MAGENTA, R_BLACK); tft.drawCentreString(String(lastRSSI)+" dBm", cx, cy-10, 4);
}

void drawGhostScanner() {
  for(int i=0; i<5; i++){
    int idx=i+bleScroll;
    if(idx<n_ble && idx<10){
      int y=70+(i*40); tft.setTextColor(R_MAGENTA);
      String name = bleResults[idx].name==""?"UNKNOWN":bleResults[idx].name;
      tft.drawString(name.substring(0,18), 20, y, 2); tft.drawString(bleResults[idx].addr, 20, y+15, 1);
    }
  }
}

void drawNetHunter() {
  for(int i=0; i<4; i++){
    int idx=i+netScroll;
    if(idx<n_networks){
      int y=70+(i*55); tft.setTextColor(R_MAGENTA);
      tft.drawString(WiFi.SSID(idx).substring(0,20), 20, y, 2);
      tft.drawString("CH: "+String(WiFi.channel(idx))+" | RSSI: "+String(WiFi.RSSI(idx)), 20, y+28, 1);
      tft.drawLine(20, y+42, 220, y+42, R_MAGENTA);
    }
  }
}

void drawDeauthWatch() {
  tft.fillRect(12, 62, 216, 236, R_BLACK); tft.setTextColor(R_MAGENTA);
  tft.setTextSize(1);
  tft.setCursor(30, 75); tft.printf("MONITOR CH: %d\n", currentChannel);
  tft.setCursor(30, 95); tft.printf("CAPTURED PKTS: %d\n", deauthCount);
  tft.setCursor(30, 125); tft.println("LAST TARGET MAC:");
  tft.setCursor(30, 145); tft.println(lastDeauthTarget);
  if (deauthCount > 0) { 
    tft.fillRect(20, 215, 200, 45, R_RED); 
    tft.setTextColor(R_BLACK); 
    tft.drawCentreString("THREAT DETECTED", 120, 228, 2); 
  }
  else { 
    tft.fillRect(20, 215, 200, 45, R_YELLOW); 
    tft.setTextColor(R_BLACK); 
    tft.drawCentreString("SECURE", 120, 228, 2); 
  }
}

void drawWifiBeacon() {
  tft.fillRect(12, 62, 216, 236, R_BLACK); 
  tft.setTextColor(R_YELLOW); tft.drawCentreString("BEACON FLOOD DYNAMIC", 120, 70, 2);
  tft.setTextColor(R_MAGENTA);
  static int listOffset = 0; 
  listOffset = (millis()/2000) % 9; 
  for(int i=0; i<8; i++) tft.drawString(floodSSIDs[i + listOffset], 30, 100 + (i*18), 1);
  tft.drawRect(15, 255, 210, 30, R_MAGENTA);
  tft.fillRect(18, 258, (millis()/15 % 204), 24, R_MAGENTA);
}

void drawSystemLogs() {
  tft.fillRect(12, 62, 216, 236, R_BLACK); tft.setTextColor(R_MAGENTA);
  for (int i = 0; i < logCount; i++) tft.drawString(systemLogs[i], 15, 70 + (i * 20), 1);
  if (logCount == 0) tft.drawCentreString("NO LOGS RECORDED", 120, 150, 1);
}
