

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

char ssid[] = "Wifi Name";
char pass[] = "WiFi Password";

unsigned int localPort = 2390;

IPAddress timeServerIP;
const char* ntpServerName = "time.nist.gov";

const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE]; 

int sec;
int mnt;
int hou;


WiFiUDP udp;

void setup()
{
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  
  Serial.begin(115200);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());
}

void loop()
{
  WiFi.hostByName(ntpServerName, timeServerIP); 
  sendNTPpacket(timeServerIP);
  
  int cb = udp.parsePacket();
  if (cb) {
    Serial.print("packet received, length=");
    Serial.println(cb);

    udp.read(packetBuffer, NTP_PACKET_SIZE); 
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    
    unsigned long epoch = (highWord << 16 | lowWord) - 2208988800UL;
   
    Serial.print("The UTC time is ");
    hou = (epoch  % 86400L) / 3600;
    Serial.print(hou); // print the hour (86400 equals secs per day)
    Serial.print(':');
    mnt = (epoch  % 3600) / 60;
    if ( mnt < 10 ) {
      Serial.print('0');
    }
    Serial.print(mnt);
    Serial.print(':');
    sec = epoch % 60;
    if (sec < 10 ) {
      Serial.print('0');
    }
    Serial.println(sec);
    Serial.println(epoch);
    delay(1000);
    }
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
      Serial.print(hou);
      Serial.print(":");
      if (mnt < 10 ) {
        Serial.print('0');
      }
      Serial.print(mnt);
      Serial.print(":");
      if (sec < 10 ) {
        Serial.print('0');
      }
      Serial.println(sec); 
      delay(1000);    
    }   
    digitalWrite(D4, LOW);
        byte hou1 = hou / 10;
    byte hou2 = hou % 10;
    hou1 = hou1 << 4;
    byte hou3 = hou1 | hou2;

    shiftOut(D6, D5, MSBFIRST, hou3);
    byte mnt1 = mnt / 10;
    byte mnt2 = mnt % 10;
    mnt1 = mnt1 << 4;
    byte mnt3 = mnt1 | mnt2;
    shiftOut(D6, D5, MSBFIRST, mnt3);
    byte sec1 = sec / 10;
    byte sec2 = sec % 10;
    sec1 = sec1 << 4;
    byte sec3 = sec1 | sec2;
    shiftOut(D6, D5, MSBFIRST, sec3);
    digitalWrite(D4, HIGH);
    Serial.println(sec3, BIN);
    Serial.println(mnt3, BIN);
    Serial.println(hou3, BIN);
}

unsigned long sendNTPpacket(IPAddress& address)
{
  Serial.println("sending NTP packet...");
  memset(packetBuffer, 0, NTP_PACKET_SIZE);

  packetBuffer[0] = 0b11100011;   
  packetBuffer[1] = 0;     
  packetBuffer[2] = 6;     
  packetBuffer[3] = 0xEC;  
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  udp.beginPacket(address, 123);
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}
