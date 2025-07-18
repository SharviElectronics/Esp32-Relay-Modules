#include <WiFi.h>
#include <WebServer.h>

#include <esp_wifi.h>
#define RELAY1  26
#define RELAY2  25
#define RELAY3  33
#define RELAY4  32

int RelayPins[] = {RELAY1, RELAY2, RELAY3, RELAY4};
const char* RelayNames[] = {"Relay 1", "Relay 2", "Relay 3", "Relay 4"};
// Replace with your WiFi credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

WebServer server(80);

// Web page HTML
String getHtml() {
  String html = "<!DOCTYPE html><html><head><title>Sharvi Relay Control</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>";
  html += "body{background:#f4f7f9;font-family:'Segoe UI',sans-serif;text-align:center;color:#333;padding:20px;}";
  html += ".logo{max-width:150px;margin-bottom:10px;}";
  html += ".device-icon{max-width:100px;margin:20px auto;}";
  html += "h1{color:#00695c;margin-bottom:5px;}";
  html += ".relay{margin:15px;padding:15px;border-radius:10px;background:#ffffff;box-shadow:0 2px 5px rgba(0,0,0,0.1);display:inline-block;min-width:250px;}";
  html += ".relay h2{margin:10px 0;color:#00796b;}";
  html += "button{padding:10px 25px;font-size:16px;border:none;border-radius:8px;margin-top:10px;}";
  html += ".on{background-color:#43a047;color:white;}";
  html += ".off{background-color:#e53935;color:white;}";
  html += ".footer{margin-top:40px;color:#888;font-size:13px;}";
  html += "</style></head><body>";

  // Logo (replace with your real image URL if needed)
  html += "<img src='https://avatars.githubusercontent.com/u/76428759?text=Sharvi+Electronics' class='logo'>";
  html += "<h1>Sharvi ESP32 Relay Controller</h1>";

  for (int i = 0; i < 4; i++) {
    String state = digitalRead(RelayPins[i]) ? "ON" : "OFF";
    String btnClass = digitalRead(RelayPins[i]) ? "on" : "off";

    html += "<div class='relay'>";
    html += "<h2>" + String(RelayNames[i]) + "</h2>";
    html += "<p>Status: <strong>" + state + "</strong></p>";
    html += "<a href='/toggle?r=" + String(i) + "'><button class='" + btnClass + "'>Toggle</button></a>";
    html += "</div>";
  }

  html += "<div class='footer'>Sharvi Electronics © 2025 – Industrial IoT Relay Module</div>";
  html += "</body></html>";
  return html;
}

void handleRoot() {
  server.send(200, "text/html", getHtml());
}

void handleToggle() {
  if (server.hasArg("r")) {
    int i = server.arg("r").toInt();
    if (i >= 0 && i < 4) {
      digitalWrite(RelayPins[i], !digitalRead(RelayPins[i]));
    }
  }
  handleRoot();
}
void readMacAddress(){
  uint8_t baseMac[6];
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
  if (ret == ESP_OK) {
    Serial.printf("%02X-%02X-%02X-%02X-%02X-%02X\n",
                  baseMac[0], baseMac[1], baseMac[2],
                  baseMac[3], baseMac[4], baseMac[5]);
  } else {
    Serial.println("Failed to read MAC address");
  }
}
void setup() {
  Serial.begin(115200);

  for (int i = 0; i < 4; i++) {
    pinMode(RelayPins[i], OUTPUT);
    digitalWrite(RelayPins[i], LOW);
  }

  WiFi.mode(WIFI_STA);
  WiFi.STA.begin();
  readMacAddress();
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nConnected to WiFi. IP: " + WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
