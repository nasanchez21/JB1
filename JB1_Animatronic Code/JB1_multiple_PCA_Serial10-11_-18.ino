#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
/*
   For pins on PCA9685
*/
//For PCA9685 @ 0x40
#define FLtorsoPin 0
#define FLrightArmPin 1
#define FLleftArmPin 2
#define FLmouthPin 3
#define FRtorsoPin 8
#define FRrightArmPin 9
#define FRleftArmPin 10
#define FRmouthPin 11
#define MLtorsoPin 4
#define MLrightArmPin 5
#define MLleftArmPin 6
#define MLmouthPin 7
#define MRtorsoPin 14
#define MRrightArmPin 13
#define MRleftArmPin 12
#define MRmouthPin 15//

//For PCA9685 @ 0x41

#define back1torsoPin 0
#define back1rightArmPin 1
#define back1leftArmPin 2
#define back1mouthPin 3


#define back2torsoPin 4
#define back2rightArmPin 5
#define back2leftArmPin 6
#define back2mouthPin 7

#define back3torsoPin 8
#define back3rightArmPin 9
#define back3leftArmPin 10
#define back3mouthPin 11

#define back4torsoPin 14
#define back4rightArmPin 13
#define back4leftArmPin 12
#define back4mouthPin 15
/*
   For Serial
*/
#define numOfVals 3
#define serialVals 8
int softSerialValues[serialVals];
int serialCount = 0;
byte byteCount = 1;
/*
   values for servos and PCA9685
*/
const uint16_t SERVOMIN  = 160; // this is the 'minimum' pulse length count (out of 4096)
const uint16_t  SERVOMAX = 490; // this is the 'maximum' pulse length count (out of 4096)
const uint16_t  SERVOCENTER = 330;

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);


const uint16_t PWMMAX = 4096;

long prevMillis = 0;
uint8_t delayVal = 2;

uint16_t stepCounter = 0;
uint16_t servoCounter1 = SERVOMIN;// for right arm and torso servos
uint16_t servoCounter2 = SERVOMAX;// for left arm
uint16_t increment = 8;
uint16_t servoIncrement = 1;

int mouthVal = 0;
int rightArmVal = SERVOMIN;
int leftArmVal = SERVOMAX;
int torsoVal = SERVOMAX;

void setup() {
  Serial.begin(9600);
  Serial.println("JB1 Control Test");

  pwm1.begin();
  pwm1.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  pwm2.begin();
  pwm2.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  // if you want to really speed stuff up, you can go into 'fast 400khz I2C' mode
  // some i2c devices dont like this so much so if you're sharing the bus, watch
  // out for this!
  Wire.setClock(400000);
  defaultPos();
  delay(50);
}

void loop() {
  //defaultPos();
  //      diagnostic() ;
  newSerialEvent();//Must include this to recieve serial
  //
  pwm1.setPWM(FLtorsoPin, 0, torsoVal);//resting center poitions for torso servo
  pwm1.setPWM(FLrightArmPin, 0, rightArmVal);//resting poitions for right servo
  pwm1.setPWM(FLleftArmPin, 0, leftArmVal);//resting poitions for left servo
  pwm1.setPWM(FLmouthPin, 0, mouthVal);//so that in deafualt, the mouth oscillates

  pwm1.setPWM(FRtorsoPin, 0, torsoVal);//resting center poitions for torso servo
  pwm1.setPWM(FRrightArmPin, 0, rightArmVal);//resting poitions for right servo
  pwm1.setPWM(FRleftArmPin, 0, leftArmVal);//resting poitions for left servo
  pwm1.setPWM(FRmouthPin, 0, mouthVal);//so that in deafualt, the mouth oscillates

  pwm1.setPWM(MLtorsoPin, 0, torsoVal);//resting center poitions for torso servo
  pwm1.setPWM(MLrightArmPin, 0, rightArmVal);//resting poitions for right servo
  pwm1.setPWM(MLleftArmPin, 0, leftArmVal);//resting poitions for left servo
  pwm1.setPWM(MLmouthPin, 0, mouthVal);//so that in deafualt, the mouth oscillates

  pwm1.setPWM(MRtorsoPin, 0, torsoVal);//resting center poitions for torso servo
  pwm1.setPWM(MRrightArmPin, 0, rightArmVal);//resting poitions for right servo
  pwm1.setPWM(MRleftArmPin, 0, leftArmVal);//resting poitions for left servo
  pwm1.setPWM(MRmouthPin, 0, mouthVal);//so that in deafualt, the mouth oscillates

  //////////////////////////////////////////////////////////////////////////////////////////////

  pwm2.setPWM(back1torsoPin, 0, torsoVal);//resting center poitions for torso servo
  pwm2.setPWM(back1rightArmPin, 0, rightArmVal);//resting poitions for right servo
  pwm2.setPWM(back1leftArmPin, 0, leftArmVal);//resting poitions for left servo
  pwm2.setPWM(back1mouthPin, 0, mouthVal);//so that in deafualt, the mouth oscillates

  pwm2.setPWM(back2torsoPin, 0, torsoVal);//resting center poitions for torso servo
  pwm2.setPWM(back2rightArmPin, 0, rightArmVal);//resting poitions for right servo
  pwm2.setPWM(back2leftArmPin, 0, leftArmVal);//resting poitions for left servo
  pwm2.setPWM(back2mouthPin, 0, mouthVal);//so that in deafualt, the mouth oscillates

  pwm2.setPWM(back3torsoPin, 0, torsoVal);//resting center poitions for torso servo
  pwm2.setPWM(back3rightArmPin, 0, rightArmVal);//resting poitions for right servo
  pwm2.setPWM(back3leftArmPin, 0, leftArmVal);//resting poitions for left servo
  pwm2.setPWM(back3mouthPin, 0, mouthVal);//so that in deafualt, the mouth oscillates

  pwm2.setPWM(back4torsoPin, 0, torsoVal);//resting center poitions for torso servo
  pwm2.setPWM(back4rightArmPin, 0, rightArmVal);//resting poitions for right servo
  pwm2.setPWM(back4leftArmPin, 0, leftArmVal);//resting poitions for left servo
  pwm2.setPWM(back1mouthPin, 0, mouthVal);//so that in deafualt, the mouth oscillates
  //delay(50);
}

void newSerialEvent() {
  //Serial.write('N');
  for (int i =  0; i < serialVals; i ++) {
    while (!Serial.available()) {
    }
    softSerialValues[i] = Serial.read();
  }
  rightArmVal = (softSerialValues[0] << 8) + (softSerialValues[1]);
  leftArmVal = (softSerialValues[2] << 8) + (softSerialValues[3]);
  torsoVal = (softSerialValues[4] << 8) + (softSerialValues[5]);
  mouthVal = (softSerialValues[6] << 8) + (softSerialValues[7]);
}

void defaultPos() {
  //mouthVal = stepCounter;
  rightArmVal = SERVOMIN;
  leftArmVal = SERVOMAX;
  torsoVal = SERVOCENTER;
  if (millis() - prevMillis > delayVal) {
    prevMillis = millis();
    stepCounter += increment;
    //Serial.println(stepCounter);
    if ((stepCounter >= 4088) || (stepCounter <= 0))
    {
      increment = -increment;
    }
  }
  for (int i = 0; i < 16; i ++) {
    if (i == 0 || i == 4 || i == 8 || i == 14) {
      pwm1.setPWM(i, 0, torsoVal);//resting center poitions for torso servo
      pwm2.setPWM(i, 0, torsoVal);//resting center poitions for torso servo

    }
    else if (i == 1 || i == 5 || i == 9 || i == 13) {
      pwm1.setPWM(i, 0, rightArmVal);//resting poitions for right servo
      pwm2.setPWM(i, 0, rightArmVal);//resting poitions for right servo

    }
    else if (i == 2 || i == 6 || i == 10 || i == 12) {
      pwm1.setPWM(i, 0, leftArmVal);//resting poitions for left servo
      pwm2.setPWM(i, 0, leftArmVal);//resting poitions for left servo

    }
    else if (i == 3 || i == 7 || i == 11 || i == 15) {
      pwm1.setPWM(i, 0, 0);//resting poitions for right servo
      pwm2.setPWM(i, 0, 0);//resting poitions for left servo

    }
  }
}
void diagnostic() {
  mouthVal = stepCounter;
  rightArmVal = servoCounter1;
  leftArmVal = servoCounter2;
  torsoVal = servoCounter1;
  if (millis() - prevMillis > delayVal) {
    prevMillis = millis();
    stepCounter += increment;
    servoCounter1 += servoIncrement;
    servoCounter2 -= servoIncrement;
    Serial.print(stepCounter);
    Serial.print(" ||  ");
    Serial.print(servoCounter1);
    Serial.print(" ||  ");
    Serial.println(servoCounter2);
    if ((stepCounter >= 4088) || (stepCounter <= 0))
    {
      increment = -increment;
    }
    if ((servoCounter1 >= SERVOMAX) || (servoCounter1 <= SERVOMIN))
    {
      servoIncrement = -servoIncrement;
    }
  }
}
/***************************************************
  This is an example for our Adafruit 16-channel PWM & Servo driver
  PWM test - this will drive 16 PWMs in a 'wave'

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815

  These drivers use I2C to communicate, 2 pins are required to
  interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/
/***************************************************
  This is an example for our Adafruit 16-channel PWM & Servo driver
  Servo test - this will drive 8 servos, one after the other on the
  first 8 pins of the PCA9685

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815

  These drivers use I2C to communicate, 2 pins are required to
  interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
****************************************************/


/*
  THis was super helpful>>>>https://learn.adafruit.com/multi-tasking-the-arduino-part-1/a-clean-sweep
*/
