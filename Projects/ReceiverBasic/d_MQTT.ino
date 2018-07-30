void setupMQTT()
{
  client.setServer(mqttServer, 1883);
  client.setCallback(receivedCallback);
  mqttconnect();
}
void mqttconnect() {
  /* Loop until reconnected */
  while (!client.connected()) {
    Serial.print("MQTT connecting ...");
    /* connect now */
    if (client.connect(clientId, mqttUser, mqttPassword))
    {
      Serial.println("Connected to MQTT");
    } else {
      Serial.println("Not connected to mqtt");
    }


  }
}
void receivedCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received: ");
  Serial.println(topic);

  Serial.print("payload: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  /* we got '1' -> on */
  if ((char)payload[0] == '1') {
    //    digitalWrite(led, HIGH);
  } else {
    /* we got '0' -> on */
    //    digitalWrite(led, LOW);
  }

}


