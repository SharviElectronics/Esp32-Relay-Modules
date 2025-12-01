#define RELAY1  13
#define RELAY2  27
#define RELAY3  26
#define RELAY4  25
#define RELAY5  33
#define RELAY6  32
#define RELAY7  14
#define RELAY8  4


int RelayArray[] = {RELAY1, RELAY2, RELAY3, RELAY4, RELAY5, RELAY6, RELAY7, RELAY8};
int i = 0;

void setup() {
  Serial.begin(115200);

  for (i = 0; i < 8; i++) {
    pinMode(RelayArray[i], OUTPUT);
    digitalWrite(RelayArray[i], LOW); // All relays off initially
  }
}

void loop() {
  for (i = 0; i < 8; i++) {
    digitalWrite(RelayArray[i], !digitalRead(RelayArray[i])); // Toggle state
    delay(1000);
  }
  delay(1000);
}
