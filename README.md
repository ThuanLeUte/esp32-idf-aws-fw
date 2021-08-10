Electronics design Task #3
==============================

Concept and Design of a CAN enabled Raspberry pi zero+Arduino

Design goals:
===============
The final product would be a single unit boxed in a 3D printed plastic enclosure (as small as possible)
The unit will be powered by 18V SLA battery.
The 18V will be fed to a 5V step down regulator and power a raspberry pi zero.
The pi will power an  ARDUINO MKR1010 WIFI through USB.
There will be serial data communication from pi between arduino over usb.
The CAN module will be connected to the arduino over SPI.
The CAN bus wires will be treated as INPUT as well as OUTPUT to/from the unit.
The pi will provision the arduino to connect to local wifi.
The end user will communicate only to the pi over a web server.
The arduino is expected to communicate with AWS IOT after all setup is complete.
Commands will be passed over from AWS IOT Cloud directly to Arduino which will then route it to the CAN bus.
A CSR will be created using a sketch during assembly time (and a private key will be irreversibly burned into the crypto chip)
The pi will
- host the program to pass on a CSR for the arduino to an AWS account and download the certificate and place it in the arduino.
- serve as an wireless access point.
- host a webserver (and serve the webpage over the wireless access point during no internet connection)
- The only purpose of the webserver as of now, is to allow the user to save the local wifi SSID and password.


Expectations of the task:
=========================
Design Research 
All programming - Raspberry pi scripts + Web server+Arduino sketch + AWS IOT
Wiring diagram (not professional, rough on paper/MS PAINT will do)
Enclosure design.
Input / Output connection terminal selection.
PCB design of mating board (if any)
Wiring and assembly instruction documentation.
Bill of Material documentation.
