# Z_Tracker
Sending data from Arduino board / sensors to Unreal Engine LiveLink over sockets.

//-----------------------------------------------------------------------------------------------------------------------//

Arduino / Devices:

M5STICK-C  https://m5stack.com/products/stick-c

SparkFun VR IMU Breakout - BNO080 (Qwiic) https://www.sparkfun.com/products/14686

Qwiic Cable/Connector https://www.sparkfun.com/products/14425

//-----------------------------------------------------------------------------------------------------------------------//

Unreal Project Example:

https://drive.google.com/drive/folders/12y9CczkizrHDU3GBrBlGe23cZ9ipb0SB?usp=sharing


//-----------------------------------------------------------------------------------------------------------------------//

Instruction:


Using USB Cable //Ideal for Realtime Camera Tracking Solution

>>>> arduino IDE //modify the arduino sketch file...set bool serial_output = true;   bool wifi_udp_output = false;

>>>> then upload to your arduino device

>>>> upload the sketch file to your arduino device

>>>> connect your arduino device with USB cable to PC that will be running "LiveLink_Data_Sender_V1/MainProgram.exe" (all the devices must be under same subnet!!!)

>>>> run MainProgram and update the setting then save it...and hit send

>>>> run LiveLink_Data_Sender_V1/MainProgram.exe
  
>>>> run the unreal project

>>>> update the blueprint by selecting available connection, check the attached screenshot as guideline
  
//--------------------------------------------------------------------------------------------------------------------------------------------------------//  
  
Using WIFI  //Ideal for external control over network (data transfer speed might be vary based on your router and arduino board!!!)
  
>>>> arduino IDE //modify the arduino sketch file... set bool serial_output = true;   bool wifi_udp_output = true;

>>>> don't forget to set your SSID and SSID password, set the IP address and port(unreal engine system)

>>>> upload the sketch file to your arduino device

>>>> run the unreal project

>>>> >>>> update the blueprint by selecting available connection, check the attached screenshot as guideline
  
 //--------------------------------------------------------------------------------------------------------------------------------------------------------//  
