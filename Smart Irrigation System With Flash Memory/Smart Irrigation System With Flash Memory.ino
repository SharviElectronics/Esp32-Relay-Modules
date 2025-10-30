#include <WiFi.h>
#include <WebServer.h>
#include <esp_wifi.h>
#include <Preferences.h> // 🔹 New: Include Preferences library for Flash memory

#define RELAY1  26
#define RELAY2  25
#define RELAY3  33
#define RELAY4  32

int RelayPins[] = {RELAY1, RELAY2, RELAY3, RELAY4};
const char* RelayNames[] = {"Relay 1", "Relay 2", "Relay 3", "Relay 4"};
// 🔹 New: Array of keys for storing state in flash memory
const char* RelayKeys[] = {"r1_state", "r2_state", "r3_state", "r4_state"}; 

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

WebServer server(80);
Preferences preferences; // 🔹 New: Create a Preferences object

// Function to save a specific relay state to flash
void saveRelayState(int index, bool state) {
  preferences.begin("relay_data", false); // Open namespace 'relay_data' (read/write)
  preferences.putBool(RelayKeys[index], state); // Save the state (true/false)
  preferences.end(); // Close the namespace
}

// Function to read and apply a specific relay state from flash
bool loadRelayState(int index) {
  preferences.begin("relay_data", true); // Open namespace 'relay_data' (read-only)
  // Get the stored state, defaulting to LOW (false) if no value is found
  bool state = preferences.getBool(RelayKeys[index], LOW); 
  preferences.end();
  return state;
}

// Original HTML generating function remains the same
String getHtml() {
  String html = "<!DOCTYPE html><html><head><title>Sharvi Relay Control</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>";
  html += "body{background:#f4f7f9;font-family:'Segoe UI',sans-serif;text-align:center;color:#333;padding:20px;}";
  html += ".logo{max-width:150px;margin-bottom:10px;}";
  html += ".relay{margin:15px;padding:15px;border-radius:10px;background:#ffffff;box-shadow:0 2px 5px rgba(0,0,0,0.1);display:inline-block;min-width:250px;}";
  html += ".relay h2{margin:10px 0;color:#00796b;}";
  html += "button{padding:10px 25px;font-size:16px;border:none;border-radius:8px;margin-top:10px;}";
  html += ".on{background-color:#43a047;color:white;}";
  html += ".off{background-color:#e53935;color:white;}";
  html += ".all-btns{margin-top:30px;}";
  html += ".footer{margin-top:40px;color:#888;font-size:13px;}";
  html += "</style></head><body>";

  html += "<img src='https://avatars.githubusercontent.com/u/76428759?text=Sharvi+Electronics' class='logo'>";
  html += "<h1>Sharvi ESP32 Relay Controller</h1>";

  for (int i = 0; i < 4; i++) {
    // Note: We read the state from the pin *after* the initial load in setup()
    String state = digitalRead(RelayPins[i]) == HIGH ? "ON" : "OFF";
    String btnClass = digitalRead(RelayPins[i]) == HIGH ? "on" : "off";
    html += "<div class='relay'>";
    html += "<h2>" + String(RelayNames[i]) + "</h2>";
    html += "<p>Status: <strong>" + state + "</strong></p>";
    html += "<a href='/toggle?r=" + String(i) + "'><button class='" + btnClass + "'>Toggle</button></a>";
    html += "</div>";
  }

  html += "<div class='all-btns'>";
  html += "<a href='/allon'><button class='on'>Turn All ON</button></a> ";
  html += "<a href='/alloff'><button class='off'>Turn All OFF</button></a>";
  html += "</div>";

  html += "<div class='footer'>Sharvi Electronics © 2025 - Industrial IoT Relay Module</div>";
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
      // Toggle the state
      int newState = !digitalRead(RelayPins[i]);
      digitalWrite(RelayPins[i], newState);
      
      // 🔹 Save the new state to flash
      saveRelayState(i, newState); 
    }
  }
  handleRoot();
}

// New function: Turn all ON
void handleAllOn() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(RelayPins[i], HIGH);
    // 🔹 Save the new state to flash
    saveRelayState(i, HIGH); 
  }
  handleRoot();
}

// Optional: Turn all OFF
void handleAllOff() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(RelayPins[i], LOW);
    // 🔹 Save the new state to flash
    saveRelayState(i, LOW); 
  }
  handleRoot();
}

void readMacAddress() {
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

  // 🔹 Initialization: Read and restore states from flash
  for (int i = 0; i < 4; i++) {
    pinMode(RelayPins[i], OUTPUT);
    // Read the saved state from flash memory
    bool savedState = loadRelayState(i); 
    // Apply the saved state to the relay pin
    digitalWrite(RelayPins[i], savedState);
    Serial.printf("Relay %d (%s) restored to: %s\n", i + 1, RelayNames[i], savedState ? "HIGH (ON)" : "LOW (OFF)");
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nConnected to WiFi. IP: " + WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.on("/allon", handleAllOn);  
  server.on("/alloff", handleAllOff); 

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
