void SetUpWiFi()
{
  WiFi.begin();
  byte mac[6];
  WiFi.macAddress(mac);
  sprintf(myHostName, "%s%02X%02X%02X%02X%02X%02X", "ESPNOW", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  sprintf(APSSID, "%s%02X%02X%02X%02X%02X%02X", "ESP32_", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  sprintf(clientId, "%s%02X%02X%02X%02X%02X%02X", "ESP32_", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.print("My APSSID is "); Serial.println(APSSID);
  Serial.print("My Hostname is "); Serial.println(myHostName);
Serial.print("My clientId is "); Serial.println(clientId);
  WiFi.setHostname(myHostName);
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(WiFiSSID, WiFiPassword);
  esp_wifi_set_max_tx_power(78);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  Serial.print("WiFi connected on channel "); Serial.println(WiFi.channel());
  setupESPNOW();
  setupMQTT();
}
