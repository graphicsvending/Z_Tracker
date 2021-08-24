import serial
import socket
import sys
#-------------------------------------------------------------------------------------#
#file = open('C:/Users/user/Documents/Visual Studio 2019/Tracker_UI_V1/Tracker_UI_V1/bin/Debug/dist/UDP_Packet_Sender/User_Seting.cfg')
file = open('User_Seting.cfg')
all_lines = file.readlines()
#-------------------------------------------------------------------------------------#
UDP_IP = str(all_lines[3].strip())
UDP_PORT = int(all_lines[4].strip())
#PREFIX = bytes.fromhex("F2")
#MESSAGE = ",Hello,1,2,3,4,5,6"
#BUFFER = PREFIX + bytes(MESSAGE, "utf-8")
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)


#-------------------------------------------------------------------------------------#
# Arduino COM Port
SERIAL_PORT = str(all_lines[7].strip())
# be sure to set this to the same rate used on the Arduino
SERIAL_RATE = int(all_lines[8].strip())
#-------------------------------------------------------------------------------------#


#-------------------------------------------------------------------------------------#
try:
    SERIAL_COM = serial.Serial(SERIAL_PORT, SERIAL_RATE, timeout=1)

except:
    print("COM Port Not Found!!!")
    exit()    

#-------------------------------------------------------------------------------------#
with SERIAL_COM as ser:
    if ser.isOpen(): 
        ser.flush()
        while True:
            try:
                reading = ser.readline().decode('utf-8').strip()

                BUFFER = bytes(reading, "utf-8")
                sock.sendto(BUFFER, (UDP_IP, UDP_PORT)) 
                print(BUFFER)
            except:
                print("Exception")
                ser.close()
                exit() 

#-------------------------------------------------------------------------------------#

