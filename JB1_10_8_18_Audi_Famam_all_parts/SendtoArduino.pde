void Sendvals() {
  byte[] serialVals = new  byte[8];

  serialVals[0] = byte(RightArm>>8);//this byte will shift the second Byte over 1
  serialVals[1] = byte(RightArm&255);//the first byte will be valued within 255
  serialVals[2] = byte(LeftArm>>8);
  serialVals[3] = byte(LeftArm&255);
  serialVals[4] = byte(Torso>>8);
  serialVals[5] = byte(Torso&255);
  serialVals[6] = byte(mouth>>8);
  serialVals[7] = byte(mouth&255);

  ArduinoPort.write(serialVals[0]);
  ArduinoPort.write(serialVals[1]);
  ArduinoPort.write(serialVals[2]);
  ArduinoPort.write(serialVals[3]);
  ArduinoPort.write(serialVals[4]);
  ArduinoPort.write(serialVals[5]);
  ArduinoPort.write(serialVals[6]);
  ArduinoPort.write(serialVals[7]);
  //println(RightArm + " || " + LeftArm + " || " + Torso + " || " + mouth);
  //println(scale + " || " + mouth + " || " + serialVals[6] + " || " + serialVals[7] );
}
void sound() {
  SoundFile.play();
  // groove.right.level() returns a value between 0 and 1. To adjust
  // the scaling and mapping of an ellipse, I will multiply by 15000
  scale = SoundFile.right.level()*13000;
  if (scale < 300) {
    mouth = 0;
  }
  else if (scale >=300 && scale <4090) {
    mouth = int(scale);
  } else  if (scale >= 4090) {
    mouth = 4090;
  }
  noStroke();
  fill(255, 0, 150);
  // We draw an ellispe coupled to the audio analysis
  ellipse(width/2, height/3, mouth/25, mouth/25); 
  //println(scale + "   " + mouth);
}
void resetSound() {
  SoundFile.pause();
  SoundFile.rewind();
}
//void Sendvals() {
//  byte[] serialVals = new  byte[8];

//  serialVals[0] = byte(RightArm>>8);//this byte will shift the second Byte over 1
//  serialVals[1] = byte(RightArm&255);//the first byte will be valued within 255
//  serialVals[2] = byte(LeftArm>>8);
//  serialVals[3] = byte(LeftArm&255);
//  serialVals[4] = byte(Torso>>8);
//  serialVals[5] = byte(Torso&255);
//  serialVals[6] = byte(mouth>>8);
//  serialVals[7] = byte(mouth&255);


//  ArduinoPort.write(serialVals[0]);
//  ArduinoPort.write(serialVals[1]);
//  ArduinoPort.write(serialVals[2]);
//  ArduinoPort.write(serialVals[3]);
//  ArduinoPort.write(serialVals[4]);
//  ArduinoPort.write(serialVals[5]);
//  ArduinoPort.write(serialVals[6]);
//  ArduinoPort.write(serialVals[7]);


//  //println(RightArm + " || " + LeftArm + " || " + Torso + " || " + mouth);
//  //println(scale + " || " + mouth + " || " + serialVals[6] + " || " + serialVals[7] );
//}
//void sound() {
//  SoundFile.play();
//  // groove.right.level() returns a value between 0 and 1. To adjust
//  // the scaling and mapping of an ellipse, I will multiply by 15000
//  scale = SoundFile.right.level()*15000;
//  if (scale >=500) {
//    mouth = int(scale);
//  } else {
//    mouth = 0;
//  }
//  noStroke();
//  fill(255, 0, 150);
//  // We draw an ellispe coupled to the audio analysis
//  ellipse(width/2, height/3, mouth/25, mouth/25); 
//  //println(scale + "   " + mouth);
//}
//void resetSound() {
//  SoundFile.pause();
//  SoundFile.rewind();
//}