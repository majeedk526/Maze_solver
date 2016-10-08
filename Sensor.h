#include <Arduino.h>

template<byte ps0, byte ps1, byte ps2, byte ps3, byte ps4, byte ps5, byte ps6, byte ps7 >

class Sensor{

public:

byte vps[8] = {0,0,0,0,0,0,0,0};
volatile float error = 0;
volatile bool isTurnRequired = false;

void configure(){

  pinMode(ps0,INPUT);
  pinMode(ps1,INPUT);
  pinMode(ps2,INPUT);
  pinMode(ps3,INPUT);
  pinMode(ps4,INPUT);
  pinMode(ps5,INPUT);
  pinMode(ps6,INPUT);
  pinMode(ps7,INPUT);
  
}

void updateError(){

  vps[0] = digitalRead(ps0);
  vps[1] = digitalRead(ps1);
  vps[2] = digitalRead(ps2);
  vps[3] = digitalRead(ps3);
  vps[4] = digitalRead(ps4);
  vps[5] = digitalRead(ps5);
  vps[6] = digitalRead(ps6);
  vps[7] = digitalRead(ps7);
  

  //printArrayStatus();

 /** if(!vps[0] && vps[7]) {
      isTurnRequired = true;
      error = -1; // right turn
      //Serial.println("right turn required");
      return;
    } 
  else if (!vps[7] && vps[0]){
      isTurnRequired = true;
      error = 1; // left turn
      //Serial.println("left turn required");
      return;
    } **/

  if(vps[0]) {error=1;}
  if(vps[1]) {error=0.6;}
  if(vps[2]) {error=0.25;}
  if(vps[3]) {error=0;}
  if(vps[4]) {error=0;}
  if(vps[5]) {error=-0.25;}
  if(vps[6]) {error=-0.6;}
  if(vps[7]) {error=-1;}
}

void printArrayStatus(){

  Serial.print(vps[0]);
  Serial.print("\t");
  Serial.print(vps[1]);
  Serial.print("\t");
  Serial.print(vps[2]);
  Serial.print("\t");
  Serial.print(vps[3]);
  Serial.print("\t");
  Serial.print(vps[4]);
  
  }

};
