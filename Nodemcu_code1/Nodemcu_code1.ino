#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

BlynkTimer timer;

void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, millis() / 3000); //was 1000
}



//Define Variables for the Turbidity Sensor
float volt;
float ntu;  
int sensorPin = A0;

//Variables used in Ultrasonic level to measure Distance
const unsigned int TRIG_PIN = D2;
const unsigned int ECHO_PIN = D6;
const unsigned int BAUD_RATE = 9600;
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

int maximumRange = 30; // maximum height of tank in cm
float v;                // volume in cubic cm
int r = 14.5;           // radius of tank in cm
int cap;                // capacity in liters
int actual_height;      // actual height in cm

//Define Variable for automation of pump
int pump = D5;

//Define Variable for collection pH values
int moisture = D1;
int output_value ;


const char auth[] = "G7j9rbEvFnn50WVkNnOwZzxhMti368hj";
const char ssid[] = "DON VINTON";
const char pass[] = "Vinton001";
//char ssid[] = "dekut";
//char pass[] = "dekut@ict2020?";

void setup()
{
  // Debug console
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(pump, OUTPUT);
  Wire.begin(D2, D1);
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  Blynk.begin(auth, ssid, pass);
  lcd.print("WATER MONITOR STARTS!"); // Start Printing
  timer.setInterval(1000L, send_blynk); 
}

void loop()
{
  Blynk.run();
  timer.run(); 
}

void turbidityval() {
  float ntuval = (analogRead(sensorPin)) * (5.0 / 690.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TURBIDITY:");
  lcd.setCursor(1, 1);
  lcd.print(ntuval);
  lcd.print(" NTU");
  delay(2000);
  Blynk.virtualWrite(V4, "add", millis() / 1000 , "NTU", ntuval);
}

void moist() {
  output_value = analogRead(moisture);
  output_value = map(output_value, 0, 1023, 0, 14);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ANALOG PH VAL:");
  Serial.print("Mositure : ");
  lcd.setCursor(1, 1);
  lcd.print(output_value);
  Serial.print(output_value);
  Serial.println("%");
  Blynk.virtualWrite(V3, "add", millis() / 1000 , "pH Value", output_value);
  delay(200);
}

void waterlevel() {
  digitalWrite(TRIG_PIN, LOW);//First set the trigpin low to avoid any noise present due to noise
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);//Send a pitch note to cause an echo effect
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);//Set the pin low
  const unsigned long duration = pulseIn(ECHO_PIN, HIGH);// Reads the echoPin, returns the sound wave travel time in microseconds
  int distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  actual_height = maximumRange - distance;
  v = ((3.14 * (r * r)) * (actual_height)); // formula to calculate volume in cubic cm
  cap = v / 1000; // final capacity in liters
  Blynk.virtualWrite(V5, cap); //Send the Value to Blynk Mobile App
  //water level logics

  lcd.clear();
  lcd.setCursor(0, 0); //Defining positon to write from first row,first column .
  lcd.print("WATER VOL:"); //You can write 16 Characters per line .
  delay(1000);//Delay used to give a dynamic effect
  lcd.setCursor(10, 0); //Defining positon to write from second row,first column .
  lcd.print(cap);
  lcd.setCursor(14, 0); //Defining positon to write from second row,first column
  lcd.print("L"); //You can write 16 Characters per line .
  delay(100);

  if (cap == 20)
  {
    Serial.println("Tank is Full!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tank is Full!");
    lcd.setCursor(1, 1);
    lcd.print("Pump goes Off");
    digitalWrite(pump, LOW);
    delay(200);
  }
  else if (cap < 6)
  {
    Serial.println("Tank is Empty!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tank is Empty!");
    lcd.setCursor(1, 0);
    lcd.print("Pump goes On");
    digitalWrite(pump, HIGH);
    delay(200);
  }
  else {
    Serial.println("Tank is Full!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WATER LEVEL OK!");
    lcd.setCursor(1, 1);
    lcd.print("PUMP HIBERNATES");
    digitalWrite(pump, LOW);
    delay(200);
  }
  delay(2000);
}
void send_blynk(){
  waterlevel();
  turbidityval();
  moist();
}
