#include <M5StickC.h>
#include <Wire.h>
#include "SparkFun_BNO080_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_BNO080

#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx//
//---Output Mode Selection---//

bool serial_output = true; //Using USB Cable...Ideal for Realtime Camera Tracking Solution <<<running with Python_LiveLink_Data_Sender>>>
bool wifi_udp_output = true; // Using Wifi...Ideal for external control over network (data transfer speed might be vary based on your router and arduino board!!!)

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx//

float qw, qx, qy, qz;
float roll, pitch, yaw;

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx//

//Wifi SSID and password
const char * networkSSID = "your SSID"; //your wifi SSID
const char * networkPass = "your password"; //your wifi password


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
const char * udpAddress = "127.0.0.1"; // your UE4_LiveLink IP Address
const int udpPort = 54321; //Port

//-------------------------------------------//

//create UDP instance
WiFiUDP udp;

//-------------------------------------------//

//General Variables
bool networkConnected = false;


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx//

BNO080 IMU;

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx//

int trigger_01 = 0;
int pre_trigger_01 = 0;

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

  M5HOMEBUTTON();

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

  DynamicJsonDocument writeData(256);

  // Create the Json string from imu sensor
  JsonObject JO = writeData.createNestedObject("Z_Tracker01");
  JO["FrameData"] = "Property";
  JsonArray values = JO.createNestedArray("UserData");
  values.add(qx);
  values.add(qy);
  values.add(qz);
  values.add(qw);
  values.add(0); //blank data
  values.add(0); //blank data
  values.add(0); //blank data
  values.add(0); //blank data
  values.add(0); //blank data
  values.add(trigger_01); //blank data

  //--------------------------------------------------------------------//

  if (wifi_udp_output) {
    // Send UDP packet
    udp.beginPacket(udpAddress, udpPort);
    serializeJson(writeData, udp);
    udp.endPacket();
  }

  //--------------------------------------------------------------------//

  if (serial_output) {
    serializeJson(writeData, Serial); // what being sent.
    Serial.println("");
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
  //M5.Lcd.printf("%5.5f, %5.5f, %5.5f\r\n", roll, pitch, yaw);

  //Serial.printf("%5.5f, %5.5f, %5.5f, %5.5f\r\n", qw, qx, qy, qz);
  //Serial.printf("%5.5f, %5.5f, %5.5f\r\n", roll, pitch, yaw);

}

//000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000//

void M5HOMEBUTTON() {

  if (digitalRead(M5_BUTTON_HOME) == LOW) {
    if (1 - pre_trigger_01 == 0) {
      pre_trigger_01 = 0;
    }
    else {
      pre_trigger_01 = 1;
    }
    delay(300);
  }

  trigger_01 = pre_trigger_01;

  //while (digitalRead(M5_BUTTON_HOME) == LOW);
}



//000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000//
