#include <BME280I2C.h>
#include <Wire.h>

#define SERIAL_BAUD 115200
#define gasPIN A0

BME280I2C bme;    
void setup()
{
  Serial.begin(SERIAL_BAUD);

  while(!Serial) {} // Wait

  int timer = 100;

  for(timer; timer >=0; timer--){
    Serial.print("SecondsLeft: ");
    Serial.print(timer);
    Serial.println(" | warming up");
    delay(10);
  }

  Wire.begin();

  while(!bme.begin())
  {
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }

  switch(bme.chipModel())
  {
     case BME280::ChipModel_BME280:
       Serial.println("Found BME280 sensor! Success.");
       break;
     case BME280::ChipModel_BMP280:
       Serial.println("Found BMP280 sensor! No Humidity available.");
       break;
     default:
       Serial.println("Found UNKNOWN sensor! Error!");
  }
}

void loop()
{

  float sensorValue = analogRead(gasPIN);

  Serial.print("SensorValue: ");
  Serial.print(sensorValue);

  Serial.print("        ");
  delay(500);

  printBME280Data(&Serial);
  delay(500);
}

//////////////////////////////////////////////////////////////////
void printBME280Data
(
   Stream* client
)
{
   float temp(NAN), hum(NAN), pres(NAN);

   BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
   BME280::PresUnit presUnit(BME280::PresUnit_Pa);

   bme.read(pres, temp, hum, tempUnit, presUnit);

   client->print("Temp: ");
   client->print(temp);
   client->print("°"+ String(tempUnit == BME280::TempUnit_Celsius ? 'C' :'F'));
   client->print("\t\tHumidity: ");
   client->print(hum);
   client->print("% RH");
   client->print("\t\tPressure: ");
   client->print(pres);
   client->println("Pa");

   delay(1000);
}

