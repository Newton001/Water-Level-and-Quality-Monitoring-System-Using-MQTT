
#include <WiFiNINA.h>
#include <BlynkSimpleWiFiNINA.h>
#include <Arduino_MKRENV.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04
#define BLYNK_PRINT Serial

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement


char auth[] = "uHqjEyPIN448FmkS-xlVObo2JOxSImE4";
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "DON VINTON";
char pass[] = "Kellino";
int pump = A6;

void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  lcd.begin();
  lcd.backlight();
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  lcd.setCursor(0,0);
  lcd.print("LEVEL: ");
  //Serial.print(distance);
  lcd.setCursor(0,8);
  lcd.print("hello");
  lcd.setCursor(0,15);
  Serial.print(" L");
  lcd.print(" L");

}
void waterlevel() {
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  lcd.clear();
  Serial.print("LEVEL: ");
  lcd.setCursor(0,0);
  lcd.print("LEVEL: ");
  Serial.print(distance);
  lcd.setCursor(0,8);
  lcd.print(distance);
  lcd.setCursor(0,15);
  Serial.print(" L");
  lcd.print(" L");
}


void loop()
{
  Blynk.run();
  waterlevel();
}
