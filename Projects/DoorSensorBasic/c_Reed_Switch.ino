void setUpReedSwitch()
{

  //INPUT_PULLUP
  pinMode(reedSwitch, INPUT_PULLUP);
  //  pinMode(33, INPUT);
  delay(20);

  Serial.print("Inital switch read ");  Serial.println(digitalRead(reedSwitch));

  if (digitalRead(reedSwitch) == 1)
  {

    Serial.println("Setting to look for close");
    esp_sleep_enable_ext0_wakeup(reedSwitch, 0); //1 = High, 0 = Low
    // bntState = 1;
  } else {

    esp_sleep_enable_ext0_wakeup(reedSwitch, 1); //1 = High, 0 = Low
    //  bntState = 0;
    Serial.println("Setting to look for open");
  }
  //pinMode(33, INPUT_PULLUP);

}

void processReedSwitch()
{
  // pinMode(33, INPUT_PULLUP);
  delay(20);
  if (digitalRead(reedSwitch) == 1)
  {
    sendButtonData("OPEN");

  } else {
    sendButtonData("CLOSED");

  }
}

void sendButtonData(char *Status)
{

  //We add the WiFi macAddress to the topic to id each device;
  WiFi.macAddress(mac);
  //Set the sensor topic
  sprintf(sensorData.topic, "%s%02X%02X%02X%02X%02X%02X", arrTopic, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  //Create the payload JSON string
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["STATUS"] = Status;
  //Get the chip's internal voltage reading
  int internalBatReading = rom_phy_get_vdd33();
  root["Vcc"] = (double)(((uint32_t)internalBatReading * 2960) / 2798) / 1000;
  //Set packet payload
  root.printTo(sensorData.payload);

  //Create and send ESP-NOW packet
  uint8_t bs[sizeof(sensorData)];
  memcpy(bs, &sensorData, sizeof(sensorData));
  const uint8_t *peer_addr = slave.peer_addr;
  esp_err_t result = esp_now_send(peer_addr, bs, sizeof(sensorData));
  Serial.print("Send Status: ");
  if (result == ESP_OK) {
    Serial.println("Success");

  } else if (result == ESP_ERR_ESPNOW_NOT_INIT) {
    // How did we get so far!!
    Serial.println("ESPNOW not Init.");
  } else if (result == ESP_ERR_ESPNOW_ARG) {
    Serial.println("Invalid Argument");
  } else if (result == ESP_ERR_ESPNOW_INTERNAL) {
    Serial.println("Internal Error");
  } else if (result == ESP_ERR_ESPNOW_NO_MEM) {
    Serial.println("ESP_ERR_ESPNOW_NO_MEM");
  } else if (result == ESP_ERR_ESPNOW_NOT_FOUND) {
    Serial.println("Peer not found.");
  } else {
    Serial.println("Not sure what happened");
  }
}
