/*************************************************** 
	This is an example for the SHT21 Humidity & Temp Sensor

	These sensors use I2C to communicate, 2 pins are required to  
	interface
 ****************************************************/
 
#include <Arduino.h>
#include <Wire.h>

#define SHT21_SLEEP // use LowPower.h to use power down sleep instead of delay()
#include "SHT21.h"

SHT21 sht21 = SHT21();

void setup()
{
	Serial.begin(9600);

	while (!Serial)
		delay(10);     // will pause Zero, Leonardo, etc until serial console opens

	Serial.println("SHT21 test");
	if (! sht21.begin(0x40)) { 
		Serial.println("Couldn't find SHT21");
		while (1) delay(1);
	}
}


void loop()
{
	if (sht21.startMeasurement()) 
	{
		float t = sht21.readTemperature() / 10.0;
		float h = sht21.readHumidity();
		Serial.print("Temp *C = "); Serial.println(t);
		Serial.print("Hum. % = "); Serial.println(h);
	}
	else
	{
		Serial.println("Failed to read sensor data");
	}
	Serial.println();

	delay(1000);
}