//#include <//ThingSpeak.h>  //To connect our //thingspeak channel with the esp8266 through this code.
#include <ESP8266WiFi.h>  // To connect the esp with internet
#include <DHT.h>
#include <Wire.h>
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#define BLYNK_TEMPLATE_ID "TMPL3QvgJZwBE"
#define BLYNK_TEMPLATE_NAME "SMART"
#define BLYNK_AUTH_TOKEN "EMqq5tulvm8wgkA6ICNcF8m_WP5ve6Ze"

char auth[] = "EMqq5tulvm8wgkA6ICNcF8m_WP5ve6Ze";  //Enter your Blynk Auth token

//---------------------Enter wifi credentials -----------------//

const char* ssid = "DjObDm";      //Name of your wifi network
const char* pass = "Dj@151610$";  // Wifi password

/* Change these values based on your calibration values */
int soilWet = 55;  // Define max value we consider soil 'wet'
int soilDry = 50;  // Define min value we consider soil 'dry'

#define sensorPin A0  //connect the sensor to analog pin of esp8266
#define motorPin D5
float Humidity = 0.0;
float Temperature = 0.0;


//---------------------Channel Details--------------------//

unsigned long Channel_ID = 1836891;            // Channel ID
const char* WriteAPIKey = "DT7DAXUCWMI03F39";  // Your write API Key
WiFiClient client;

#define DHTPIN 2  //D4 pin
#define DHTTYPE DHT11
BlynkTimer timer;

DHT dht(DHTPIN, DHTTYPE);
long delayStart = millis();
int motor;

void setup() {
  pinMode(motorPin, OUTPUT);
  digitalWrite(motorPin, HIGH);
  motor = 0;
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 8080);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)  //This loop will work untill esp connects to the wifi
  {
    delay(500);
    Serial.print(".");
  }
  //ThingSpeak.begin(client);
  delay(1000);
}


void loop() {
  Humidity = dht.readHumidity();
  Temperature = dht.readTemperature();
  Serial.print("Humidity : ");
  Serial.println(Humidity);
  Serial.print("Temperature :");
  Serial.println(Temperature);
  Blynk.virtualWrite(V0, Temperature);
  Blynk.virtualWrite(V1, Humidity);
  delay(1000);
  int moisture = analogRead(sensorPin);
  moisture = map(moisture, 0, 1024, 100, 0);
  Serial.print("Soil moisture: ");
  Serial.println(moisture);
  Blynk.virtualWrite(V3, moisture);
  //ThingSpeak.writeField(Channel_ID,1,Humidity, WriteAPIKey);
  //ThingSpeak.writeField(Channel_ID,2,Temperature, WriteAPIKey);
  //ThingSpeak.writeField(Channel_ID,3,moisture, WriteAPIKey);
  // Determine status of our soil using the values already defined for wet and dry soil
  if (moisture > soilWet) {
    Serial.println("Status: Soil is too wet");
    digitalWrite(motorPin, HIGH);
    motor = 0;
    Blynk.virtualWrite(V5, motor);
    //BLYNK_WRITE(V5);

    //ThingSpeak.writeField(Channel_ID,4, false, WriteAPIKey);
  } else if (moisture <= soilWet && moisture > soilDry) {
    Serial.println("Status: Soil moisture is perfect");
    digitalWrite(motorPin, HIGH);
    motor = 0;
    Blynk.virtualWrite(V5, motor);
    //BLYNK_WRITE(V5);

    //ThingSpeak.writeField(Channel_ID,4, false, WriteAPIKey);
  } else {
    Serial.println("Status: Soil is too dry - time to water!");
    digitalWrite(motorPin, LOW);
    motor = 1;
    Blynk.virtualWrite(V5, motor);
    Serial.println("Watering...");
    delay(5000);
    digitalWrite(motorPin, HIGH);
    motor = 0;
    Blynk.virtualWrite(V5, motor);
    //BLYNK_WRITE(V5);

    //ThingSpeak.writeField(Channel_ID,4, true, WriteAPIKey);
  }
  delay(5000);  // Take a reading every 5 seconds for testing
  Serial.println();
}
