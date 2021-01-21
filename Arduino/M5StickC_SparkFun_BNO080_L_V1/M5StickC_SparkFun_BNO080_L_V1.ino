#include <M5StickC.h>
#include <Wire.h>
#include "SparkFun_BNO080_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_BNO080

#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx//
//---Output Mode Selection---//
// Set bool wifi_udp_output = false >>>if you wanna use USB Cable (Serial Com Port)
bool serial_output = true; //Using USB Cable...Ideal for Realtime Camera Tracking Solution <<<running with LiveLink_Data_Sender>>>
bool wifi_udp_output = false; // Using Wifi...Ideal for external control over network (data transfer speed might be vary based on your router and arduino board!!!)

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx//

float arbitrary1, arbitrary2, arbitrary3, arbitrary4, arbitrary5, arbitrary6;
float qw, qx, qy, qz;
float roll, pitch, yaw;

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx//

//Wifi SSID and password
const char * networkSSID = "xxxxx"; //your wifi SSID
const char * networkPass = "12345"; //your wifi password


/*
  //uncomment for M5StickC Static IP
  // Set your Static IP address
  IPAddress local_IP(10, 134, 1, 236);
  IPAddress gateway(10, 134, 1, 254);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress primaryDNS(10, 134, 1, 131);   //optional
  IPAddress secondaryDNS(8, 8, 8, 8); //optional
*/

//-------------------------------------------//

//UE4 LiveLink IP & Port
const char * udpAddress = "88.888.8.888"; // your UE4_LiveLink IP Address
const int udpPort = 20000; //Port

//UE4 LONET_LiveLink Data Packet Structure
byte prefix = 0XF2;
uint8_t buffer[35];

//-------------------------------------------//

//create UDP instance
WiFiUDP udp;

//-------------------------------------------//

//General Variables
bool networkConnected = false;


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx//

BNO080 IMU;

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx//

//0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000//

void setup()
{

  Serial.begin(115200);
  while (!Serial);

  // Initialize the M5StickC object
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(0, 0);

  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx//

  if (wifi_udp_output) {
    /*
      //uncomment for Static IP
      // config Static IP
      if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
        Serial.println("STA Failed to configure");
      }
    */
    //--------------------------------------------------------------------//

    delay(100); //wait 100ms before moving on
    connectToNetwork(); //starts Wifi connection
    while (!networkConnected) {
      delay(200);
    }

    //----------------------------------------//

    //This initializes udp and transfer buffer
    udp.begin(udpPort);
  }

  //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx//

  //--------------------------------------------------------------------//
  Wire.begin(0, 26); //M5StickC SDA(G0), SCL(G26)
  Wire.setClock(400000); //Increase I2C data rate to 400kHz
  //--------------------------------------------------------------------//

  // check BNO080 wiring / connection

  if (IMU.begin() == false)
  {
    Serial.println("BNO080 not detected at default I2C address. Check your jumpers and the hookup guide. Freezing...");
    while (1);
  }
  IMU.enableARVRStabilizedGameRotationVector(5); //Send data update every 5ms

  ////

  M5.Lcd.fillScreen(BLACK);

  //--------------------------------------------------------------------//

}

//000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000//

void loop()
{
  //--------------------------------------------------------------------//

  reading_imu_data();

  //--------------------------------------------------------------------//

  sending_imu_data();

  //--------------------------------------------------------------------//

  print_imu_data();

  //--------------------------------------------------------------------//

}

//000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000//

void connectToNetwork()
{
  //logger("Connecting to SSID: " + String(networkSSID), "info");
  M5.Lcd.println("Connecting to SSID: " + String(networkSSID));
  Serial.println("Connecting to SSID: " + String(networkSSID));

  WiFi.disconnect(true);
  WiFi.onEvent(WiFiEvent);

  WiFi.mode(WIFI_STA); //station
  WiFi.setSleep(false);

  WiFi.begin(networkSSID, networkPass);
}

//000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000//

void WiFiEvent(WiFiEvent_t event)
{
  switch (event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      M5.Lcd.println("Network connected!");
      M5.Lcd.println("SSID: " + String(networkSSID));
      M5.Lcd.println(WiFi.localIP());
      M5.Lcd.println(WiFi.macAddress());

      Serial.println("Network connected!");
      Serial.println("SSID: " + String(networkSSID));
      Serial.println(WiFi.localIP());
      Serial.println(WiFi.macAddress());

      networkConnected = true;
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      M5.Lcd.println("Network connection lost!");
      Serial.println("Network connection lost!");

      networkConnected = false;
      break;
  }
}

//000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000//

void reading_imu_data()
{
  if (IMU.dataAvailable() == true)
  {
    qx = IMU.getQuatI();
    qy = IMU.getQuatJ();
    qz = IMU.getQuatK();
    qw = IMU.getQuatReal();

    roll = (IMU.getRoll()) * 180.0 / PI; // Convert roll to degrees
    pitch = (IMU.getPitch()) * 180.0 / PI; // Convert pitch to degrees
    yaw = (IMU.getYaw()) * 180.0 / PI; // Convert yaw / heading to degrees
  }
}

//000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000//

void sending_imu_data()
{

  //arbitrary6 = analogRead(inputPin1);

  arbitrary1 = qx;
  arbitrary2 = qy;
  arbitrary3 = qz;
  arbitrary4 = qw;
  arbitrary5 = 0; //not in use
  arbitrary6 = 0; //not in use



  //--------------------------------------------------------------------//

  if (wifi_udp_output) {
    udp.beginPacket(udpAddress, udpPort);
    memset(buffer, prefix, 1);
    udp.write(buffer, 1);
    udp.printf(",Z_Tracker_01,%5.5f, %5.5f, %5.5f, %5.5f, %5.5f, %5.5f",
               arbitrary1, arbitrary2, arbitrary3, arbitrary4, arbitrary5, arbitrary6);

    udp.endPacket();
  }

  //--------------------------------------------------------------------//

  if (serial_output) {
    Serial.printf(",Z_Tracker_01,%5.5f, %5.5f, %5.5f, %5.5f, %5.5f, %5.5f\r\n",
                  arbitrary1, arbitrary2, arbitrary3, arbitrary4, arbitrary5, arbitrary6);

  }

  //--------------------------------------------------------------------//

}

//000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000//

void print_imu_data()
{

  //M5 battery level
  int batteryLevel = floor(100.0 * (((M5.Axp.GetVbatData() * 1.1 / 1000) - 3.0) / (4.07 - 3.0)));
  batteryLevel = batteryLevel > 100 ? 100 : batteryLevel;

  //--------------------------------------------------------------------//

  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(WHITE, BLACK);

  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("IMU INFO");

  M5.Lcd.setCursor(70, 0);
  M5.Lcd.println("Battery: " + String(batteryLevel) + "%");

  M5.Lcd.setCursor(0, 70);
  M5.Lcd.printf("%5.2f, %5.2f, %5.2f, %5.2f", qw, qx, qy, qz);


}

//000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000//
