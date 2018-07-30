void InitESPNow() {
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
  }
  else {
    Serial.println("ESPNow Init Failed");
    ESP.restart();
  }
}

void setupESPNOW()
{
  InitESPNow();
  esp_now_register_recv_cb(OnDataRecv);
  configDeviceAP();
  }
void configDeviceAP() {

  
  Serial.println(APSSID);

  
  bool result = WiFi.softAP(APSSID, "ThisPasswordIsNeverUsed", WiFi.channel(), 0);
  if (!result) {
    Serial.println("AP Config failed.");
  } else {
    Serial.println("AP Config Success. Broadcasting with AP: " + String(APSSID));
  }
}


void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
  mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Recv from: "); Serial.println(macStr);
  Serial.print("Last Packet Recv Data: "); Serial.println(*data);
  Serial.println("");
  memcpy(&sensorData, data, sizeof(sensorData));

  Serial.println();
  Serial.print("Received Data ");
  Serial.print(sensorData.topic);
  Serial.print("  ");
  Serial.println(sensorData.payload);
  client.publish(sensorData.topic, sensorData.payload);
}
