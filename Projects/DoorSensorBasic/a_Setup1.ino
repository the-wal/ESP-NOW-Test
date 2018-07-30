void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Welcome to Tony's ESP-NOW Demo");
  WiFi.mode(WIFI_STA);
  InitESPNow();
  if (bootCount == 0)
  {
    esp_wifi_set_max_tx_power(78);
    ScanForSlave();
  }
else {
    if (manageSlave() == 0)
    {
      ESP.restart();
    }
  }

  
  setUpReedSwitch();

  if (get_wakeup_reason() == 1)
  {
    Serial.println("processReedSwitch");
    processReedSwitch();

    return;
  }
  

  //Code should only get here on first boot
  Serial.println("should only get here on boot");
  setUpReedSwitch();
  bootCount++;
  putMeToSleep();
}


void loop() {
  // put your main code here, to run repeatedly:

}

