#include <dht11.h>
#include <Wire.h>
#include <LPS.h>

#define DHT11PIN 9

LPS ps;
dht11 DHT11; 

void setup()
{
  Serial.begin(9600);
  ps.enableDefault();
  Wire.begin();    
}
 
void loop()
{
  int chk = DHT11.read(DHT11PIN);

  Serial.print("humdity:");             
  Serial.print((float)DHT11.humidity, 9);
  
  float pressure = ps.readPressureMillibars();
  float altitude = ps.pressureToAltitudeMeters(pressure);
  float temperature = ps.readTemperatureC();
  
  Serial.print("%\tp: ");
  Serial.print(pressure);
  Serial.print(" mbar\ta: ");
  Serial.print(altitude);
  Serial.print(" m\tt: ");
  Serial.print(temperature);
  Serial.println(" deg C");
 
  delay(1000);
} 
