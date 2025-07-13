#define RELAY1  26
#define RELAY2  25
#define RELAY3  33
#define RELAY4  32

int RelayArray[] = {RELAY1, RELAY2, RELAY3, RELAY4};
int i = 0;

void setup() {
  Serial.begin(115200);

  for (i = 0; i < 4; i++) {
    pinMode(RelayArray[i], OUTPUT);
    digitalWrite(RelayArray[i], LOW); // All relays off initially
  }
}

void loop() {
  for (i = 0; i < 4; i++) {
    digitalWrite(RelayArray[i], !digitalRead(RelayArray[i])); // Toggle state
    delay(1000);
  }
  delay(1000);
}
