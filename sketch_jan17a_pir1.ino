/*
  This example code is for Detecting People with PIR Motion Sensor.
  Vcc -> 5v
  Out -> pin 22
*/

int PIRpin = 22;                    // choose the input pin (for PIR sensor)
int PIRstate = 0;                   // we start, assuming no motion detected

void setup() {
  Serial.begin(9600);
  pinMode(PIRpin, INPUT);           // initialize the PIR sensor pin as an input:
}

void loop() {
  PIRstate = digitalRead(PIRpin);   // read the state of the pushbutton value:
  Serial.println(PIRstate);

  // check if the PIR sensor is triggered. If it is, the PIRstate is HIGH:
  if (PIRstate == HIGH) {
    Serial.println("Motion Detected");
    delay(5000);
  
  } else {
    Serial.println("Clear");;
  }
}