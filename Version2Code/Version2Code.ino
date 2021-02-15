#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
#include <SPI.h>
#include <WiFiNINA.h>
#include <BlynkSimpleWiFiNINA.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "uHqjEyPIN448FmkS-xlVObo2JOxSImE4";
// Your WiFi credentials.
// Set password to "" for open networks.
//char ssid[] = "dekut";
//char pass[] = "dekut@ict2020?";

char ssid[] = "DON VINTON";
char pass[] = "Vinton001";



const unsigned int TRIG_PIN = 3;
const unsigned int ECHO_PIN = 2;
const unsigned int BAUD_RATE = 9600;

// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

int maximumRange = 800; // maximum height of tank in cm 
float v;                // volume in cubic cm
int r = 9;           // radius of tank in cm
int cap;                // capacity in liters
int actual_height;      // actual height in cm

int sensorPin = A1;

int pump = 6;

void setup() {
  pinMode(pump, OUTPUT); // Pin to connect the relay
  lcd.begin();
  Blynk.begin(auth, ssid, pass);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(BAUD_RATE);
  //lcd.backlight();//To Power ON the back light
  lcd.print("SECOND COUNT");
}

void loop() {
  Blynk.run();
  waterlevel();
  turbidityval();
}

void waterlevel() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  const unsigned long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration/582;
  actual_height = maximumRange - distance;
  v = ((3.14 * (r * r)) * (actual_height)); // formula to calculate volume in cubic cm
  cap = v / 1000; // final capacity in liters
  Blynk.virtualWrite(V5, cap);
  if (duration == 0) {
    Serial.println("Warning: no pulse from sensor");
  }
  else {
    Serial.print("distance to nearest object:");
    Serial.println(distance);
    Serial.println(" cm");
    lcd.clear();
    lcd.setCursor(0, 0); //Defining positon to write from first row,first column .
    lcd.print("WATER VOL:"); //You can write 16 Characters per line .
    delay(1000);//Delay used to give a dynamic effect
    lcd.setCursor(10, 0); //Defining positon to write from second row,first column .
    lcd.print(cap);
    lcd.setCursor(14, 0); //Defining positon to write from second row,first column
    lcd.print("L"); //You can write 16 Characters per line .
    delay(100);
  }
  delay(2000);
}

void turbidityval(){
  int sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  int turbidity = map(sensorValue, 0, 1023, 0, 255);
  delay(100);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("turbidity:");
  lcd.setCursor(1,1);
  lcd.print(turbidity);
  delay(2000);
  Serial.println(turbidity);
  Blynk.virtualWrite(V4, "add", millis() / 1000 , "NTU",  turbidity);
}
