
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "DHT.h"

#define DHTPIN D3     
#define DHTTYPE DHT22

WiFiUDP udp;
DHT dht(DHTPIN, DHTTYPE);

int port = 1234;

void setup() {
  pinMode(D3, INPUT);
  Serial.begin(9600);
  dht.begin();
  
  WiFi.begin("WiFi Name", "WiFi Password");
    while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  udp.begin(port);
}

void loop() {
  delay(2000);

  float h = dht.readHumidity();
  Serial.print(h);
}
