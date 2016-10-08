//TO do: update weight in sensors.h

#include <EnableInterrupt.h>

#include "DCMotors.h" 
#include "Sensor.h"
#include "DetectNode.h"
#include "Path.h"

#define isrPinRight 8
#define isrPinLeft 3

volatile bool isrInProcess  = false;

DCMotors<10,18,19,11,14,15> motors; //enL, L1, L2, enR, R1, R2
Sensor<2,3,4,5,6,7,8, 9> sensors;
DetectNode dnode;
Path path;

float Kp=7.5, Ki=5 ,Kd=0.08;

float P=0, D=0;
volatile float PID_value=0;
volatile float I=0;
float error=0;
float previous_error=0, previous_I=0;
char c;
byte nVisited = 0;
byte S = 100;
byte L = 101;
byte R = 102;
byte ca;

byte c1 = 2; // variable check points
byte c2 = 3; // variable check points

void setup() {
  
  motors.configure();
  sensors.configure();
  Serial.begin(9600);

  debug = false;
  
  invalidate();

  path.getPath(c1, c2);
  
}

void loop() {

      if(isrInProcess){
              //Serial.println("looping");
              return;
        }
     
      sensors.updateError();
      calculate_pid();
        
            if(dnode.isOnNode(sensors.vps)){
                ca = path.p[nVisited];
                nVisited++;

                if(ca == S){motors.drive((int)PID_value);}
                else if (ca == R) {
                    enableInterrupt(isrPinLeft, isrRightTurnComplete, RISING );
                  }
                else if (ca == L){
                    enableInterrupt(isrPinRight, isrLeftTurnComplete, RISING );
                  }
              }
          
    delay(1);
}


void invalidate(){
  sensors.error = 0;
   PID_value = 0;
   I=0;
}

void calculate_pid()
{

    error = (sensors.error);
    Serial.print("\t");
    Serial.println(error);
    
    P = error;
    I = I + error*0.001;
    D = (error - previous_error)/.001;
    
    PID_value = (Kp*P) + (Ki*I) + (Kd*D);
        
    previous_error=error;
}

void isrRightTurnComplete(){
  
    motors.stopMoving();
    disableInterrupt(isrPinLeft);
    sensors.isTurnRequired = false;
    invalidate();
    isrInProcess = false;
  }

void isrLeftTurnComplete(){
  
    motors.stopMoving();
    disableInterrupt(isrPinRight);
    sensors.isTurnRequired = false;
    invalidate();
    isrInProcess = false;
  }

  void deactivateInterrupt(){
    
    motors.stopMoving();
    disableInterrupt(isrPinRight);
    disableInterrupt(isrPinLeft);
    invalidate();
    isrInProcess = false;
    
  }

void btDebug()
{
      if(c=='1'){
          Ki+=0.1;
          Serial.println(Ki);
      }
      else if(c=='0'){  
          Kd+=0.1;
          Serial.println(Kd);
      }
      else if(c=='2'){
          Kp+=1;
          Serial.println(Kp);
      }
      else if(c=='3'){
          Ki-=0.1;
          Serial.println(Ki);
      }
      else if(c=='4'){  
          Kd-=0.1;
          Serial.println(Kd);
      }
      else if(c=='5'){
          Kp-=1;
          Serial.println(Kp);
      } else if(c=='6'){
        motors.spConst += 1;
        Serial.println(motors.spConst);
          
      } else if(c=='7'){
        motors.spConst -= 1;  
      }
}

