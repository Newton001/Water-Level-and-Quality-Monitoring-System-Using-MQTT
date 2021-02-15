# Water-Level-and-Quality-Monitoring-System-Using-MQTT
Water is an important resource for the livelihood of human beings. The quality of water used affects the health of the consumers. Storage vessels tend to develop microbes that lower the water quality. Microbes cause infectious diseases such as flu and measles. There is also strong evidence that microbes may contribute to many non–infectious chronic diseases such as cancer and coronary heart disease. The level of water in an enclosed tank is often unknown leading to a lack of water when there is no supply of water into the tank or water overflow due to lack of observation of the level of water in the tank. This project seeks to address this by the use of an ultrasonic sensor, analog pH sensor, and turbidity sensor to obtain the data on the state of water in the tank. The level of water and water condition data is sent to a mobile application for observation, and initiation of a required response for instance automation of water pumping into the tank as well as switching off of the pumping action. The proposed project will utilize Message Queuing Telemetry Transport (MQTT) IoT technology.
 
# How it works?
This project uses ultrasonic sensor,  which measures the water level, Analog pH sensor, which Measures the pH state of Water. The data is sent to NodeMCU that incoroporates an onboard ESP8266 to trasnmit the data to a Blynk App.
The WiFi module is connected to internet and shares the data to Blynk platform. This data is displayed on real time basis. Over smart phone using Blynk app you can see the water level and quality parameters. With few modification you can get approximate liters of water available in the tank as well as its state.

We are going to use NodeMCU microcontroller and Blynk IoT app for monitoring.

## Parts List

 1. Arduino MKR Wi-Fi 1010 x 1
 2. 5V SMPS power supply x 1
 3. Ultrasonic Sensor x 1
 4. Analog pH sensor x 1
 5. Buzzer (Optional) x 1
 6. Resistor 330Ω x 1
 7. Relay x 1
 8. 3.3V Regulator LD33V x 1
 9. 4 Core Long Cable
 10. LED x 5
 11. Box to mount entire project x 1
 
 ![GitHub Logo](/images/logo.png)
 

This project will allow you to monitor the water level of an overhead tank or underground pump. You can configure to auto start / stop the pump or motor based on water level.
The Circuit connection for the Ultrasensor and the NodeMCU is as shown below



