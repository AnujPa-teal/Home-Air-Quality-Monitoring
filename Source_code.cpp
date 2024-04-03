#include <DHT.h>
#include <MQ135.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>

#define DHTPIN D2
#define DHTTYPE DHT11    // If you have DHT22 then write #define DHTTYPE DHT22 
#define MQ135_PIN A0

const char *ssid =  "Hotspot/WiFi_SSID";     
const char *pass =  "Hotspot/WiFi_PASSWORD";
const char* apiKey = "Channel_apikey";
const unsigned long THINGSPEAK_CHANNEL_ID = paste_your_thingspeak_channel_id ;
const char* server = "api.thingspeak.com";

DHT dht(DHTPIN, DHTTYPE);
MQ135 gasSensor = MQ135(MQ135_PIN);

WiFiClient client;

void setup() {
  Serial.begin(9600);     // Check the Baud Rate for your NodeMCU board and put accordingly .
  // Here Baud rate is 9600 . Set Baud rate at serial monitor window to 9600 .

  delay(10);

  dht.begin();

  WiFi.begin(ssid, password);

  ThingSpeak.begin(client);

  pinMode(MQ135_PIN, INPUT);
}

void loop() {
  delay(2000);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  float airQuality = gasSensor.getPPM();

  ThingSpeak.setField(1, temperature);    // choose temp. as field 1 in thingspeak channel
  ThingSpeak.setField(2, humidity);       // choose humidity as field 2 in thingspeak channel
  ThingSpeak.setField(3, airQuality);     // choose air quality as field 3 in thingspeak channel

  int statusCode = ThingSpeak.writeFields(THINGSPEAK_CHANNEL_ID, apiKey);

  if (statusCode == 200) {
    Serial.println("Data has been sent to ThingSpeak server successfully!");
  } 
  else {
    Serial.print("Error occured while sending data. Status code: ");
    Serial.println(statusCode);
  }
}
