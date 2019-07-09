#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11  
#define dht_dpin 14  // D8
DHT dht(dht_dpin, DHTTYPE); 
#define BUZZER_PIN  D6
LiquidCrystal_I2C lcd(0x27, 16, 2);

//U-SONIC
const int trigPin = 2;  //D4
const int echoPin = 0;  //D3
long duration;
int distance;

const char *ssid     = "HMSECURE";
const char *password = "up16h7896$";

const long utcOffsetInSeconds = 19800;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "0.in.pool.ntp.org", utcOffsetInSeconds);

void setup(){
  Serial.begin(115200);

  //DHT BELOW
  dht.begin();
  Serial.println("Humidity and temperature\n\n");
  delay(700);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); 
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output U-SONIC
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input U-SONIC
  
  WiFi.begin(ssid, password);
  Wire.begin(D2, D1);
  lcd.begin();
  lcd.home();
  
  while ( WiFi.status() != WL_CONNECTED ) {
    lcd.setCursor(0,0);
    lcd.print("Waiting");
    delay ( 5000 );
    Serial.print ( "." );
  }

  timeClient.begin();
}

void loop() {

  //U-SENSOR STARTS
  // Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*0.034/2;
Serial.print(distance);

// Prints the distance on the Serial Monitor
if (distance <= 30)
{   
    digitalWrite(BUZZER_PIN, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    
    float h = dht.readHumidity();
    float t = dht.readTemperature();         
    
    lcd.print("Temp -> ");
    lcd.print(t);
    lcd.print(" ");
    lcd.print((char)223);
    lcd.print("C");
    
    lcd.setCursor(0, 1);
    lcd.print("Humid -> ");
    lcd.print(h);
    lcd.print("%");
    
    delay(5000);
  }
else
  {
  digitalWrite(BUZZER_PIN, LOW); 
  lcd.clear();
  timeClient.update();
  lcd.setCursor(5, 1);
  lcd.print(daysOfTheWeek[timeClient.getDay()]);
 
  if (timeClient.getSeconds() !=0){
  lcd.setCursor(0, 0);
  lcd.print("Time -> ");
  lcd.print(timeClient.getHours());
  lcd.print(":");
  lcd.print(timeClient.getMinutes());
  lcd.print(":");
  lcd.print(timeClient.getSeconds());
  
  delay(1000);
  }
  else{

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time -> ");
  lcd.print(timeClient.getHours());
  lcd.print(":");
  lcd.print(timeClient.getMinutes());
  lcd.print(":");
  lcd.print(timeClient.getSeconds());
  lcd.setCursor(5, 1);
  lcd.print(daysOfTheWeek[timeClient.getDay()]);
  delay(1000);
    
    }}
    
}
