#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

int port = 1234;
int packetSize;

int sec;
int mnt;
int hou;

long epoch;

WiFiUDP udp;

void setup() {
  pinMode(D4, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  
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
}

void loop() {
  //parsing packet
  packetSize = udp.parsePacket();
  char packetBuffer[packetSize];
  if(packetSize > 0){
    epoch = packetBuffer[packetSize];
    udp.read(packetBuffer, packetSize);
    Serial.println(packetBuffer);
    Serial.println(packetSize);
    hou = (epoch  % 86400L) / 3600;
    mnt = (epoch  % 3600) / 60;
    sec = epoch % 60;
  }
  //calculating time
  else{
    sec++;
    if(sec >= 60){
      sec = 0;
      mnt++;
    }
    if(mnt >= 60){
      mnt = 0;
      hou++;
    }
    if(hou > 24){
      hou = 0;
    }
  }
  //shifting out time       
  digitalWrite(D4, LOW);
  byte hou1 = hou / 10;
  byte hou2 = hou % 10;
  hou1 = hou1 << 4;
  byte hou3 = hou1 | hou2;
  shiftOut(D2, D1, MSBFIRST, hou3);
  byte mnt1 = mnt / 10;
  byte mnt2 = mnt % 10;
  mnt1 = mnt1 << 4;
  byte mnt3 = mnt1 | mnt2;
  shiftOut(D2, D1, MSBFIRST, mnt3);
  byte sec1 = sec / 10;
  byte sec2 = sec % 10;
  sec1 = sec1 << 4;
  byte sec3 = sec1 | sec2;
  shiftOut(D2, D1, MSBFIRST, sec3);
  digitalWrite(D4, HIGH);
  //pirinting time
  Serial.println(sec3, BIN);
  Serial.println(mnt3, BIN);
  Serial.println(hou3, BIN);
  Serial.println(epoch);
  delay(1000);
  }

