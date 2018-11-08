// These constants won't change.  They're used to give names
// to the pins used:
const int numOfPins = 3; // this is the number of pins that we will be reading from
const int numOfVals = numOfPins * 2; // this is the number of pins that we will be reading from
const int analogInPins[] = {A1, A2, A3};
const String designatedBodyPart[] = {"Left Arm", "Torso", "Right Arm"};
//const int analogOutPin = 9; // Analog output pin that the LED is attached to

int sensorVals[numOfPins];        // value read from the pot
int sendVals[numOfVals];
//int outputValue = 0;        // value output to the PWM (analog out)

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  for (int i = 0; i < numOfPins; i++) {
    sensorVals[i] = 0;
  }
}

void loop() {
  for (int i = 0; i < numOfPins; i++) {
    sensorVals[i] = analogRead(analogInPins[i]);
  }
  //  sensorVals[0] = analogRead(analogInPins[A1]);
  //  sensorVals[1] = analogRead(analogInPins[A2]);
  //  sensorVals[2] = analogRead(analogInPins[A3]);

  sendVals[0] = byte(sensorVals[0] >> 8);
  sendVals[1] = byte(sensorVals[0] & 255);
  sendVals[2] = byte(sensorVals[1] >> 8);
  sendVals[3] = byte(sensorVals[1] & 255);
  sendVals[4] = byte(sensorVals[2] >> 8);
  sendVals[5] = byte(sensorVals[2] & 255);


  //debug();

  sendSerial();

  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(100);
}
void sendSerial() {
  //while (Serial.available()) {
  for (int i = 0; i < numOfVals; i++) {
    Serial.write(sendVals[i]);
    //    Serial.print(sendVals[i]);
    //    Serial.print(", ");
  }
  //  Serial.print("            ");
  //  Serial.print((sendVals[0] << 8) + (sendVals[1]));
  //  Serial.print(", ");
  //  Serial.print((sendVals[2] << 8) + (sendVals[3]));
  //  Serial.print(", ");
  //  Serial.print((sendVals[4] << 8) + (sendVals[5]));
  //  Serial.print(", ");
  //  Serial.println();

  //}
}
void debug() {
  // print the results to the serial monitor:
  for (int i = 0; i < numOfPins; i++) {
    Serial.print(" || ");
    Serial.print(designatedBodyPart[i]);
    Serial.print(" : ");
    Serial.print(sensorVals[i]);
  }
  Serial.println();
}
/*
  Analog input, analog output, serial output

  Reads an analog input pin, maps the result to a range from 0 to 255
  and uses the result to set the pulsewidth modulation (PWM) of an output pin.
  Also prints the results to the serial monitor.

  The circuit:
   potentiometer connected to analog pin 0.
   Center pin of the potentiometer goes to the analog pin.
   side pins of the potentiometer go to +5V and ground
   LED connected from digital pin 9 to ground

  created 29 Dec. 2008
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/
