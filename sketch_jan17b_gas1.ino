/*
  This example code is for Detecting Smoke, LPG, CO gases with MQ2 Sensor Module
  Vcc -> 5v
  A0 -> pin 35 (ADC)
  
*/

//Used Pins
#define MQ2pin 35

float sensorValue;  //variable to store sensor value

void setup() {
	Serial.begin(9600); // sets the serial port to 9600
	Serial.println("MQ2 warming up!");
	delay(20000); // allow the MQ2 to warm up 20 sec
  Serial.println("MQ2 warmed up!");
	
}

void loop() {
  float gasValue = analogRead(MQ2pin);// read analog input pin 35


	Serial.print("Sensor Value: ");
	Serial.println(gasValue);
	
	delay(2000); // wait 2s for next reading
}