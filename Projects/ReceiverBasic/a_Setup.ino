void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("welcome to Tony's ESP-NOW demo receiver");
  SetUpWiFi();
  
}

void loop() {
  // put your main code here, to run repeatedly:
    if (!client.loop())
    {
      client.disconnect();
      Serial.println("############################# Connecting to mqtt #####################################");
      mqttconnect();
      delay(50);
    }
    delay(200);
}
