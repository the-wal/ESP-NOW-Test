void InitESPNow() {
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
    esp_now_register_send_cb(OnDataSent);

  }
  else {
    Serial.println("ESPNow Init Failed");
    ESP.restart();
  }
}
void configDeviceAP(int ch) {
  char* SSID = "Slave_1";
  bool result = WiFi.softAP(SSID, "Slave_1_Password", ch, 1);
  if (!result) {
    Serial.println("AP Config failed.");
  } else {
    Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
    delay(1000);
    Serial.println("Restarting");
    WiFi.disconnect();
    ESP.restart();
  }
}


void ScanForSlave() {
  int currentRSSI = -200;
  int currentIndex = -1;
  int currentChannel = WiFi.channel();
  Serial.print("WiFi Channel "); Serial.println(WiFi.channel());
  int8_t scanResults = WiFi.scanNetworks();
  if (scanResults == 0)
  {
    Serial.println("No WiFi devices in AP Mode found");
    // clean up ram
    WiFi.scanDelete();
    return;
  }
  Serial.print("Found "); Serial.print(scanResults); Serial.println(" devices ");
  for (int i = 0; i < scanResults; ++i)
  {
    //Search access points for ssid's that start with the value of ReceiverSSID
    int32_t RSSI = WiFi.RSSI(i);
    String SSID = WiFi.SSID(i);
    String BSSIDstr = WiFi.BSSIDstr(i);
    
    if (SSID.indexOf(ReceiverSSID) == 0)
    {
      //I have three receivers through my house so this code will find the one with the best signal
      Serial.print("currentRSSI "); Serial.println(currentRSSI);
      Serial.print("SSID ");    Serial.println(SSID);
      Serial.print("RSSI ");    Serial.println(RSSI);
      Serial.print(" Slave Channel "); Serial.println(WiFi.channel(i));
      
      if (RSSI > currentRSSI)
      {
        currentRSSI = RSSI;
        currentIndex = i;
        currentChannel = WiFi.channel(i);
        int mac[6];
        if ( 6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x%c",  &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5] ) ) {
          for (int ii = 0; ii < 6; ++ii ) {
            slave.peer_addr[ii] = (uint8_t) mac[ii];
          }
        }

        slave.channel = WiFi.channel(i); // pick a channel
        slave.encrypt = 0; // no encryption
      }
    }
  }
  
  Serial.print("currentRSSI "); Serial.println(currentRSSI);
  //Hardware resetting the chip will set the wifi channel to 1.
  //Here is the imortant bit, if the sensor wifi channel does not match we need to set the chip up as a SoftAP to the same channels
  //as the receiver then software reset the device to it comes back up on the correct channel.
  if (currentChannel != WiFi.channel())
  {
    Serial.println("changine channel");
    configDeviceAP(currentChannel);
    return;
  }
  Serial.println("Sensor is setup");
  // clean up ram
  WiFi.scanDelete();
}

bool manageSlave() {


  if (bootCount > 0) {
    if (DELETEBEFOREPAIR) {
      // deletePeer();
    }

    Serial.print("Slave Status: ");
    const esp_now_peer_info_t *peer = &slave;
    const uint8_t *peer_addr = slave.peer_addr;
    // check if the peer exists
    bool exists = esp_now_is_peer_exist(peer_addr);
    if ( exists) {
      // Slave already paired.
      Serial.println("Already Paired");
      return true;
    } else {
      // Slave not paired, attempt pair
      esp_err_t addStatus = esp_now_add_peer(peer);
      if (addStatus == ESP_OK) {
        // Pair success
        Serial.println("Pair success");
        return true;
      } else if (addStatus == ESP_ERR_ESPNOW_NOT_INIT) {
        // How did we get so far!!
        Serial.println("ESPNOW Not Init");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_ARG) {
        Serial.println("Invalid Argument");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_FULL) {
        Serial.println("Peer list full");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_NO_MEM) {
        Serial.println("Out of memory");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_EXIST) {
        Serial.println("Peer Exists");
        return true;
      } else {
        Serial.println("Not sure what happened");
        return false;
      }
    }
  } else {
    // No slave found to process
    Serial.println("No Slave found to process");
    return false;
  }
}


