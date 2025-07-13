#include <WiFi.h>
#include <WebServer.h>

#define RELAY1  26
#define RELAY2  25
#define RELAY3  33
#define RELAY4  32

int RelayPins[] = {RELAY1, RELAY2, RELAY3, RELAY4};

// Replace with your WiFi credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

WebServer server(80);

// HTML page
String getHtml() {
  String html = "<html><head><title>Sharvi Relay Control</title>";
  html += "<style>body{font-family:sans-serif;text-align:center;} button{padding:10px 20px;margin:10px;font-size:18px;} .on{background:green;color:white;} .off{background:red;color:white;}</style></head><body>";
  html += "<h1>Sharvi 4-Relay Control</h1>";
  for (int i = 0; i < 4; i++) {
    String state = digitalRead(RelayPins[i]) ? "ON" : "OFF";
    String btnClass = digitalRead(RelayPins[i]) ? "on" : "off";
    html += "<p>Relay " + String(i + 1) + ": <b>" + state + "</b></p>";
    html += "<a href='/toggle?r=" + String(i) + "'><button class='" + btnClass + "'>Toggle Relay " + String(i + 1) + "</button></a>";
  }
  html += "</body></html>";
  return html;
}

// Root page
void handleRoot() {
  server.send(200, "text/html", getHtml());
}

// Toggle relay
void handleToggle() {
  if (server.hasArg("r")) {
    int relayIndex = server.arg("r").toInt();
    if (relayIndex >= 0 && relayIndex < 4) {
      digitalWrite(RelayPins[relayIndex], !digitalRead(RelayPins[relayIndex]));
    }
  }
  handleRoot(); // Reload page
}

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < 4; i++) {
    pinMode(RelayPins[i], OUTPUT);
    digitalWrite(RelayPins[i], LOW); // Start OFF
  }

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}
