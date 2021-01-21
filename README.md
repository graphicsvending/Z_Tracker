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

Arduino IDE
Set bool wifi_udp_output = false >>>if you wanna use USB Cable (Serial Com Port)
bool serial_output = true; //Using USB Cable...Ideal for Realtime Camera Tracking Solution <<<running with LiveLink_Data_Sender>>>
bool wifi_udp_output = true; // Using Wifi...Ideal for external control over network (data transfer speed might be vary based on your router and arduino board!!!)



  
#LiveLink_Data_Sender_V1 is included

<<Using USB Cable>> Ideal for Realtime Camera Tracking Solution <<<running with LiveLink_Data_Sender>>>
  //modify the arduino sketch file...set bool serial_output = true; bool wifi_udp_output = false; then upload to your arduino device
  //Steps >>>> arduino IDE >>>> connect your arduino device with USB cale to any pc that will be running "LiveLink_Data_Sender_V1/MainProgram.exe" >>>>
    run MainProgram and update the setting then save it...and hit send >>>> run LiveLink_Data_Sender_V1/MainProgram.exe >>>>
    run the unreal project and update the blueprint by selecting available connection * >>> 
  
<<Using WIFI>> Ideal for external control over network (data transfer speed might be vary based on your router and arduino board!!!)
  modify the arduino sketch file... set bool serial_output = true; bool wifi_udp_output = true;
  dont forget to set your SSID and SSID password, set the IP address and port(unreal engine system)
  //Steps >>>> arduino IDE >>>> run the unreal project and update the blueprint by selecting available connection >>> 
