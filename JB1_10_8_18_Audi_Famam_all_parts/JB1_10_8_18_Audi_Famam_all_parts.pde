/*
UPDATE 4-20-18:
 
 Added Minim to this code, removed default Processing sound library. Now, 
 this sketch should be able to read amplitude of a sound from an mp3 file.
 Minim does this and improves functionality. However, live recording and 
 playback won't be available through this sketch. It is better to record/edit
 audio in advance, and then import it to be analyzed. 
 
 JB1 code for controlling 3 servos
 sends servo positions to Arduino Uno, 
 which then sends servo positions to ATtiny 44
 March 14, 2018
 
 Uses ControlP5 Library and example "ControlP5 Slider"
 by Andreas Schlegel, 2010
 
 UPDATE: MARCH 16, 2018,
 This sketch also heavily borrows from Processing examples Libraries>
 Sound>IO>AudioInput example code.
 
 we should also send 4 bytes; three for motion, one for sound
 */


//Based off Of Nicholas Sanchez, Capstone Animatronic Controller
//April 16, 2017
//Based off of Color tracking via vvv
// IMA NYU Shanghai
// Kinetic Interfaces
// MOQN
// Sep 28 2016
//import processing.sound.*;

import ddf.minim.*;
import processing.serial.*;
//import controlP5.*;

//AudioIn input;
//Amplitude rms;
//SoundFile welcome;
//For Minim 
Minim minim1, minim2;
AudioPlayer SoundFile;
AudioPlayer MainTheme;


Serial ArduinoPort, ControlPort;
//ControlP5 cp5;


float RightArmVal, LeftArmVal, TorsoVal;
int RightArm, LeftArm, Torso;
float scale=1;
int mouth = 0;
//Slider abc;

//For creating CSV
Table table;//create instance of table
TableRow newRow; //new row instance
long currentMillis = 0;
long prevMillis = 0; //for counting interval
int interval = 10; //interval value for recording
int interval2 = 22;// Golden number DONOTCHANGE
int counter = 0;// for counting up

boolean record = false;
boolean playBack = false;
int[] initialVals = {0, 0, 180};
int[] recieveVals = {0, 0, 0, 0, 0, 180};// from ControlPort
void setup() {
  size(600, 600);
  frameRate(40);
  //establish serial connection
  printArray(Serial.list());
  ArduinoPort = new Serial(this, Serial.list()[9], 9600);
  ControlPort = new Serial(this, Serial.list()[8], 9600);
  // for Minim
  minim1 = new Minim(this);
  minim2 = new Minim(this);
  SoundFile = minim1.loadFile("Audi Famam.mp3", 1024);// This is the sound that Audio will be mapped too.
  MainTheme = minim2.loadFile ("Main Theme - Super Smash Bros. Brawl.mp3", 1024);// This is the sound that will play during the playback.

  //Create Table
  table = new Table();
  table.addColumn("RightArm");
  table.addColumn("LeftArm");
  table.addColumn("Torso");
  table.addColumn("Mouth");

  newRow = table.addRow();
  newRow.setInt("RightArm", RightArm);
  newRow.setInt("LeftArm", LeftArm);
  newRow.setInt("Torso", Torso);
  newRow.setInt("Mouth", mouth);
  //welcome.play();
  delay(2000);
}

void draw() {
  background(0);
  currentMillis = millis();
  convertArmVals();
  if (record == true && playBack == false) {
    sound();
    
    convertArmVals();
    Sendvals();

    updateCSV();
  } else if (record == false && playBack == true) {
    MainTheme.play();
    readCSV();
  } else if (record == false && playBack == false) {
    resetSound();
  }
  println("RECORD" + record + " || PLAYBACK" + playBack);
  println(currentMillis + "  ||  " + prevMillis);
}
void keyPressed() {
  if (key == ' ') {
    record = !record;
  }
  if (keyCode == ENTER) {
    playBack = !playBack;
  }
}

void updateCSV() {
  counter ++;
  newRow = table.addRow();//needs to be here to add new rows, else only one row will change
  //this will add to the existing rows. Input setInt(Row name , value)
  newRow.setInt("RightArm", RightArm);
  newRow.setInt("LeftArm", LeftArm);
  newRow.setInt("Torso", Torso);
  newRow.setInt("Mouth", mouth);

  saveTable(table, "data/SSBB.csv");//This function must occur at the very end of the draw
  delay(interval);
}
void readCSV() {
  //SoundFile.play();

  for (int i = 2; i <table.getRowCount(); i++) {

    RightArm = table.getInt(i, "RightArm");   
    LeftArm = table.getInt(i, "LeftArm");
    Torso = table.getInt(i, "Torso");
    mouth = table.getInt(i, "Mouth");

    Sendvals();
    delay(interval2);//inerval2 seems to work. Takes care of lag
  }

  playBack = false;
  //MainTheme.pause();
  //MainTheme.rewind();
  SoundFile.pause();
  SoundFile.rewind();
}
void convertArmVals() {
  int leftArmRaw, rightArmRaw, TorsoRaw;
  while (ControlPort.available() >0) {
    for (int i = 0; i < 6; i++) {
      recieveVals[i] = ControlPort.read();
    }
  }
  for (int i = 0; i < 6; i++) {
    println(recieveVals[i]);
  }
  leftArmRaw = (recieveVals[0] << 8) + (recieveVals[1]);
  rightArmRaw= (recieveVals[4] << 8) + (recieveVals[5]);
  TorsoRaw = (recieveVals[2] << 8) + (recieveVals[3]);
  LeftArm = int(map(leftArmRaw, 530, 160, 490, 165));
  Torso = int(map(TorsoRaw, 200, 540, 180, 480) );
  RightArm = int(map(rightArmRaw, 160, 490, 165, 490));

  println("++++++++++++++++++++++++++++");
  println(leftArmRaw);
  println(rightArmRaw);
  println(TorsoRaw);
}