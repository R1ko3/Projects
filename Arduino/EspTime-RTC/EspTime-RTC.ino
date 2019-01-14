#include <Wire.h>
#include <DS3231.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

int port = 1234;
int packetSize;

WiFiUDP udp;

DS3231 clock;
RTCDateTime dt;

void setup()
{
  Serial.begin(115200);
  
  WiFi.begin("WiFi Name", "WiFi Password");
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  
  udp.begin(port);

  // Initialize DS3231
  Serial.println("Initialize DS3231");;
  clock.begin();


}

void loop()
{
  packetSize = udp.parsePacket();
  char packetBuffer[packetSize];
  if(packetSize > 0){
    udp.read(packetBuffer, 100);
    String number =  packetBuffer;
    int epoch = number.toInt();
    Serial.println(epoch);
    clock.setDateTime(epoch);
  }
  
  dt = clock.getDateTime();

  // For leading zero look to DS3231_dateformat example

  Serial.print("Raw data: ");
  Serial.print(dt.year);   Serial.print("-");
  Serial.print(dt.month);  Serial.print("-");
  Serial.print(dt.day);    Serial.print(" ");
  Serial.print(dt.hour);   Serial.print(":");
  Serial.print(dt.minute); Serial.print(":");
  Serial.print(dt.second); Serial.println("");

  delay(1000);
}
