/*
********************************************
Authors: Newton Kelvin
         Salome Chepchirchir
PROJECT: Water Level and Quality Monitoring System Using MQTT
********************************************
*/
#include <SoftwareSerial.h>                           //we have to include the SoftwareSerial library, or else we can't use it
#define TRIGGER 4
#define ECHO    5
#define PUMP 8
#define sensorValue A0
#define rx 2                                          //define what pin rx is going to be
#define tx 3                                          //define what pin tx is going to be
#define SensorPin A1

SoftwareSerial myserial(rx, tx);                      //define how the soft serial port is going to work
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10],temp;



// NodeMCU Pin D1 > TRIGGER | Pin D2 > ECHO

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "xxxxxxxxxxxxx";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "DON VINTON";
char pass[] = "1234567890987";


void setup() {
  
  Serial.begin (9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(PUMP, OUTPUT);
}

void loop() {
  waterLevel();
  sensorph();
  turbidity();
  
}

void waterLevel(){
  long duration, distance;
  digitalWrite(TRIGGER, LOW);  
  delayMicroseconds(2); 
  
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(TRIGGER, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = (duration/2) / 29.1;

  //Sending Water Level to Blynk App for Observation

   if (distance <= 150) {
    Blynk.virtualWrite(V0, 255);
}
  else {
    Blynk.virtualWrite(V0, 0);
  }

 if (distance <= 100) {
    Blynk.virtualWrite(V1, 255);
}
  else {
    Blynk.virtualWrite(V1, 0);
  }

   if (distance <= 80) {
    Blynk.virtualWrite(V2, 255);
}
  else {
    Blynk.virtualWrite(V2, 0);
  }

   if (distance <= 40) {
    Blynk.virtualWrite(V3, 255);
}
  else {
    Blynk.virtualWrite(V3, 0);
  }

   if (distance <= 20) {
    Blynk.virtualWrite(V4, 255);
}
  else {
    Blynk.virtualWrite(V4, 0);
  }
  /* PUMP ACTIVATION AND SWITCHING */
  if (distance < 100){
    digitalWrite(PUMP, HIGH);
  }
  else if (distance > 100){
    digitalWrite(PUMP);
  }  
  
  Serial.print(distance);
  Serial.println("Centimeter:");
  Blynk.virtualWrite(V5, distance);
  delay(200);
  Blynk.run();
}

//Measure Water Turbidity

void turbidity(){
  int sensorValue = analogRead(A0);// read the input on analog pin 0:
  float voltage = sensorValue * (5.0 / 1024.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  Serial.println(voltage); // print out the value you read:
  delay(500);
}

//Measure and Average the Water ph Values

void sensorph()
{
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=3.5*phValue;                      //convert the millivolt into pH value
  Serial.print("    pH:");  
  Serial.print(phValue,2);
  Serial.println(" ");
  digitalWrite(13, HIGH);       
  delay(800);
  digitalWrite(13, LOW); 
 
}