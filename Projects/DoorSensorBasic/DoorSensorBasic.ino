#include <ArduinoJson.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>

//Required to read internal vcc
extern "C" int rom_phy_get_vdd33();

//Global stuff
RTC_DATA_ATTR int bootCount = 0;


//ESP-NOW
//SSID receiver name to scan for
char ReceiverSSID[20] = "ESP32_";
//Added RTC_DATA_ATTR to the line below so the slave data will survive wakeup from deep-sleep
RTC_DATA_ATTR esp_now_peer_info_t slave;
#define CHANNEL 1 
//really not used in this project.
#define DELETEBEFOREPAIR 0 
//really not used in this project.


//This struct is what is sent to the receiver.
//The receiver does not process any of the data other then extract the 
//topic and payload and send it on to the mqtt server.
//The reason for this is I have three sensor types at this stage on my system at home.
//Each with a different topic.
//ESP32/DOOR/         Door sensor
//ESP32/Remote/       Four buttone remote
//ESP32/PHOTO/        Six touch sensor photoframe

#define MESSAGELEN 100
byte mac[6];
char *arrTopic = "ESP32/DOOR/";
struct __attribute__((packed)) SENSOR_DATA {
  char topic[MESSAGELEN] = "";
  char payload[MESSAGELEN] = "";
} sensorData;



//Reed switch pin number
gpio_num_t reedSwitch = GPIO_NUM_33;


//Don't know why but this code needs to be in the same tab as 
//#include <esp_now.h>
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  if (status == ESP_NOW_SEND_SUCCESS)
  {
    Serial.println("Packet sent ok");
  } else {
    Serial.println("Well shit something went wrong");
    ESP.restart();
  }
  putMeToSleep();
}
