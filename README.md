

![alt_tag](https://github.com/joemcmanus/MerakiLED/blob/master/images/P8240361.jpg)

# MerakiLED
Visualizing Cisco Meraki Data via Sparkfun Qwiic Devices 

Do you know what is going on in yoru network? With rich data from sources like NetFlow, Meraki API and SNMP it can be visualized in many ways. 
Our screens are full of data making it difficult to see new alerts. This project moves 3 pieces of data; performance, alerts and througput visualized on a devices in the real world. 

This is built using a Sparkfun ESP32 Thing board with QWIIC connectors and 3 Sparkfun Qwiic LED sticks. Data is obtained via the Meraki API using python and passed to the ESP32 using MQTT. 

# Usage

Simply run ./netperf.py... Okay there are a few more steps to get that to work.

# Installation Linux

Install the prerequisites

     # sudo pip3 install meraki
     # apt install mosquitto-clients mosquitto  

Modify the configuration file

    # vi netperf.conf 
    [apiKey]
    key=yourKeyHere
    
    [mx]
    serial=QQQQ-QQQQ-1234
    
    [ms]
    serial=QQQQ-QQQQ-1234
    
    [org]
    orgId=123456
    
    [mqtt]
    server=localhost 

    [local]
    bandwidth=123

    [opHours]
    startTime=6
    endTime=18

# Install on ESP32

To use the Sparkfun ESP32 Qwiic you will need to load new ESP libraries and set your board to Adafruit Feather, also you'll need the correct usb driver for the ESP32. 

Also you'll need to assign new i2C addresses to the 2nd and 3rd Qwiic LED sticks. You can follow this example sketch: https://github.com/sparkfun/SparkFun_Qwiic_LED_Stick_Arduino_Library/blob/master/examples/Example10_ChangeAddress/Example10_ChangeAddress.ino


# Run

Simple as ./netperf.py 


# Parts

 - Sparkfun ESP32 Wroom Thing Plus https://www.sparkfun.com/products/15663
 - Sparkfun SparkX LED Stick https://www.sparkfun.com/products/14783
 - Pelican 1060 Case https://www.pelican.com/us/en/product/cases/micro/1060 
 - Qwiic Cable kit 
 - USB Power supply 
 - Standoffs
