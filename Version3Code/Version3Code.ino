/* WATER QUALITY AND LEVEL MONITORING SYSTEM USING MESSAGE QUEUING AND TELEMETRY TRANSPORT SYSTEM
   Authors: Salome Chepchirchir
           Newton Kelvin
*/


#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
#include <SPI.h>
#include <WiFiNINA.h>
#include <BlynkSimpleWiFiNINA.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).

char auth[] = "G7j9rbEvFnn50WVkNnOwZzxhMti368hj";
// Your WiFi credentials.
// Set password to "" for open networks.
//char ssid[] = "dekut";
//char pass[] = "dekut@ict2020?";

char ssid[] = "DON VINTON";
 char pass[] = "VinTon0001";

/*char ssid[] = "Nemo";
  char pass[] = "12345678";
*/
/*
  char ssid[] = "Games";
  char pass[] = "1234567890";
*/

//char ssid[] = "Jackie";
//char pass[] = "12345678";

//char ssid[] = "Janetjael";
//char pass[] = "Vinton001";


//Define Variables for the Turbidity Sensor
float volt;
float ntu;
int sensorPin = A3;

//Variables used in Ultrasonic level to measure Distance
int TRIG_PIN = 3;
int ECHO_PIN = 2;
const unsigned int BAUD_RATE = 9600;
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement


int maximumRange = 30; // maximum height of tank in cm
float v;                // volume in cubic cm
int r = 14.5;           // radius of tank in cm
int cap;                // capacity in liters
int actual_height;      // actual height in cm


//Define Variable for automation of pump
int pump = 6;


//Define Variable for collection pH values
int moisture = A4;
int output_value ;

//LED pin assignments
int led_safe = 4;
int led_danger  = 1;


void setup() {
  /*Initialization of Pins, and Communications
    Code Runs Once*/
  pinMode(led_safe, OUTPUT);
  pinMode(led_danger, OUTPUT);
  pinMode(moisture, INPUT);//Pin Connected to pH sensor
  pinMode(pump, OUTPUT); // Pin to connect the relay
  lcd.begin();
  Blynk.begin(auth, ssid, pass);
  pinMode(TRIG_PIN, OUTPUT);//Pin Connected to Trigpin of Ultrasonic sensor
  pinMode(ECHO_PIN, INPUT);//Pin Connected to Trigpin of Ultrasonic sensor
  Serial.begin(BAUD_RATE);
  //lcd.backlight();//To Power ON the back light
  lcd.print("SYSTEM IS ACTIVE!");
  //FLASH ALL LIGHTS
  for (int i; i < 10; i++) {
    digitalWrite(led_safe, HIGH);
    digitalWrite(led_danger, HIGH);
    delay(500);
    digitalWrite(led_safe, LOW);
    digitalWrite(led_danger, LOW);
    delay(200);
  }

}

void loop() {
  /*Calling of various Functions*/
  Blynk.run();
  waterlevel();
  turbidityval();
  moist();
}

void waterlevel() {
  digitalWrite(TRIG_PIN, LOW);//First set the trigpin low to avoid any noise present due to noise
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);//Send a pitch note to cause an echo effect
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);//Set the pin low
  const unsigned long duration = pulseIn(ECHO_PIN, HIGH);// Reads the echoPin, returns the sound wave travel time in microseconds
  int distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  Serial.println(distance);
  actual_height = maximumRange - distance;
  v = ((3.14 * (r * r)) * (actual_height)); // formula to calculate volume in cubic cm
  cap = v / 1000; // final capacity in liters
  Blynk.virtualWrite(V5, cap); //Send the Value to Blynk Mobile App
  //water level logics
  if (cap < 2) {
    Blynk.email("Newtonkelvin75@gmail.com", "WATER LEVEL", "Water Level is too Low!");
  }
  lcd.clear();
  lcd.setCursor(0, 0); //Defining positon to write from first row,first column .
  lcd.print("WATER VOL:"); //You can write 16 Characters per line .
  //delay(1000);//Delay used to give a dynamic effect
  lcd.setCursor(10, 0); //Defining positon to write from second row,first column .
  lcd.print(cap);
  lcd.setCursor(14, 0); //Defining positon to write from second row,first column
  lcd.print("L"); //You can write 16 Characters per line .
  delay(1000);

  if (cap <= 8)
  {
    //Serial.println("Tank is Full!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tank is Full!");
    lcd.setCursor(1, 1);
    lcd.print("Pump goes Off");
    digitalWrite(pump, LOW);
    delay(2000);
  }
  else if (cap >= 18 )
  {
    //Serial.println("Tank is Empty!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tank is Empty!");
    lcd.setCursor(1, 1);
    lcd.print("Pump goes On");
    digitalWrite(pump, HIGH);
    delay(2000);
    digitalWrite(led_danger, HIGH);
    delay(500);
    digitalWrite(led_danger, LOW);
    delay(500);
  }
}


void turbidityval() {
 
  float ntuval = (analogRead(sensorPin)) * (5.0 / 1024.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
   Serial.println(analogRead(sensorPin));
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TURBIDITY:");
  lcd.setCursor(1, 1);
  lcd.print(ntuval);
  lcd.print(" NTU");
  delay(2000);
  if (ntuval < 2.5) {
    Blynk.email("Newtonkelvin75@gmail.om", "WATER ALERT", "Water is too Cloudy");
    digitalWrite(led_danger, HIGH);
    delay(500);
    digitalWrite(led_danger, LOW);
    delay(500);
    digitalWrite(led_danger, HIGH);
    delay(500);
    digitalWrite(led_danger, LOW);
    delay(500);
    digitalWrite(led_danger, HIGH);
    delay(500);
    digitalWrite(led_danger, LOW);
    delay(500);
    digitalWrite(led_danger, HIGH);
    delay(500);
    digitalWrite(led_danger, LOW);
    delay(500);
  }
  Blynk.virtualWrite(V4, "add", millis() / 1000 , "NTU", ntuval);
}

void moist() {
  output_value = analogRead(moisture);
  //Serial.println(moisture);
  output_value = map(output_value, 0, 1023, 0, 14);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ANALOG PH VAL:");
  //Serial.print("Mositure : ");
  lcd.setCursor(1, 1);
  lcd.print(output_value);
  //Serial.print(output_value);
  Blynk.virtualWrite(V3, "PH VALUE: ", output_value);
  //Blynk.virtualWrite(V3, "add", millis() / 1000 , "pH Value", output_value);
  delay(3000);
}
