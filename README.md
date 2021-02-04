# Z_Tracker
Sending data from Arduino board / sensors to Unreal Engine LiveLink over sockets.

//-----------------------------------------------------------------------------------------------------------------------//

Arduino / Devices:

M5STICK-C  https://m5stack.com/products/stick-c

SparkFun VR IMU Breakout - BNO080 (Qwiic) https://www.sparkfun.com/products/14686

Qwiic Cable/Connector https://www.sparkfun.com/products/14425

//-----------------------------------------------------------------------------------------------------------------------//

Unreal Project Example:

4.26 (able to package your project)

https://drive.google.com/drive/folders/1VLsQt1B19Ilpa-ymeBVlP7oMbGEIMG13?usp=sharing


//-----------------------------------------------------------------------------------------------------------------------//

Unreal Engine:

https://www.unrealengine.com/en-US/download

//-----------------------------------------------------------------------------------------------------------------------//

UE4 Plugin:

UE4 Livelink (default plugin)
Z_TRACKERLiveLink (included)
ZT_QuaternionRotation (included)

//-----------------------------------------------------------------------------------------------------------------------//


Instruction:


Using USB Cable //Ideal for Realtime Camera Tracking Solution

>>>> arduino IDE //modify the arduino sketch file...set bool serial_output = true;   bool wifi_udp_output = false;

>>>> then upload to your arduino device

>>>> connect your arduino device with USB cable to PC that will be running "LiveLink_Data_Sender_V1/MainProgram.exe" (all the devices must be under same subnet!!!)

>>>> run MainProgram and update the setting then save it...and hit send
  
>>>> run the unreal project

>>>> update the blueprint by selecting available connection, check the attached screenshot as guideline
  
//--------------------------------------------------------------------------------------------------------------------------------------------------------//  
  
Using WIFI  //Ideal for external control over network (data transfer speed might be vary based on your router and arduino board!!!)
  
>>>> arduino IDE //modify the arduino sketch file... set bool serial_output = true;   bool wifi_udp_output = true;

>>>> don't forget to set your SSID and SSID password, set the IP address and port(unreal engine system)

>>>> upload the sketch file to your arduino device

>>>> run the unreal project

>>>> update the blueprint by selecting available connection, check the attached screenshot as guideline
  
 //--------------------------------------------------------------------------------------------------------------------------------------------------------//  
