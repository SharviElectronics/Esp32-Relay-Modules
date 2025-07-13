#define RELAY1  26
#define RELAY2  25
#define RELAY3  33
#define RELAY4  32
#define RelayArray[] = {26, 25, 33, 32}
int i =0;
void setup() {
  Serial.begin(115200);

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);

  // Turn all relays off initially
  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, LOW);
  digitalWrite(RELAY3, LOW);
  digitalWrite(RELAY4, LOW);
}

void loop() {
  for(i=0; i<4; i++)
    {
      digitalWrite(RelayArray[i], !digitalRead(RelayArray[i]));
      delay(1000);
    }
  delay(1000);
}
