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

Minim minim;
AudioPlayer SoundFile;
AudioPlayer MainTheme;

Serial ArduinoPort;

Table table;

float RightArmVal, LeftArmVal, TorsoVal;
int RightArm, LeftArm, Torso;
float scale=1;
int mouth = 0;

boolean playBack = false;

void setup() {
  size(600, 600);
  frameRate(40);

  printArray(Serial.list());
  ArduinoPort = new Serial(this, Serial.list()[9], 9600);

  // for Minim
  minim = new Minim(this);
  SoundFile = minim.loadFile ("jack b du song.mp3", 1024);
  //SoundFile = minim.loadFile ("Main Theme - Super Smash Bros. Brawl.mp3", 1024);

  //Create Table
  table = loadTable("IAMJACKBDU.csv", "header");//, "LeftArm", "Torso", "Mouth"););
  //table = loadTable("NewSSSB.csv", "header");//, "LeftArm", "Torso", "Mouth"););

}

void draw() {
  background(0);
  if (playBack == true) {
    readCSV();
  } else {
    resetSound();
  }
  println("PLAYBACK:  " + playBack);
}
void keyPressed() {
  if (keyCode == ' ') {
    playBack = !playBack;
  }
}
void readCSV() {
  SoundFile.play();
  for (int i = 2; i <table.getRowCount(); i++) {
    RightArm = table.getInt(i, "RightArm");   
    LeftArm = table.getInt(i, "LeftArm");
    Torso = table.getInt(i, "Torso");
    mouth = table.getInt(i, "Mouth");
    Sendvals();
    delay(22);//22 seems to work. Takes care of lag
  }
  SoundFile.pause();
  SoundFile.rewind();
  playBack = false;
}