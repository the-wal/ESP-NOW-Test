#include <PubSubClient.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <ArduinoJson.h>


//WiFi
char myHostName[20];
char APSSID[20] = "ESP32_";
char WiFiSSID[20] = "The_Wal";
char WiFiPassword[20] = "ecab133222";


//MQTT
WiFiClient espClient;
PubSubClient client(espClient);

char mqttServer[20] = "10.0.0.30";
int mqttPort = 1883;
char mqttUser[20] = "";
char mqttPassword[20] = "";


char *sClientName = "ESP";
char clientId[23] = "";


#define MESSAGELEN 100
struct __attribute__((packed)) SENSOR_DATA {
  char topic[MESSAGELEN] = "";
  char payload[MESSAGELEN] = "";
} sensorData;
