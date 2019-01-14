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
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  Serial.begin(115200);
  
  WiFi.begin("salon", "knedleleniwe");
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
  
  digitalWrite(D4, LOW);
  byte hou1 = dt.hour / 10;
  byte hou2 = dt.hour % 10;
  hou1 = hou1 << 4;
  byte hou3 = hou1 | hou2;
  shiftOut(D6, D5, MSBFIRST, hou3);
  byte mnt1 = dt.minute / 10;
  byte mnt2 = dt.minute % 10;
  mnt1 = mnt1 << 4;
  byte mnt3 = mnt1 | mnt2;
  shiftOut(D6, D5, MSBFIRST, mnt3);
  byte sec1 = dt.second / 10;
  byte sec2 = dt.second % 10;
  sec1 = sec1 << 4;
  byte sec3 = sec1 | sec2;
  shiftOut(D6, D5, MSBFIRST, sec3);
  digitalWrite(D4, HIGH);


  Serial.print("Raw data: ");
  Serial.print(dt.year);   Serial.print("-");
  Serial.print(dt.month);  Serial.print("-");
  Serial.print(dt.day);    Serial.print(" ");
  Serial.print(dt.hour);   Serial.print(":");
  Serial.print(dt.minute); Serial.print(":");
  Serial.print(dt.second); Serial.println("");

  delay(1000);
}
