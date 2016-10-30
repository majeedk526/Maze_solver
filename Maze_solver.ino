#include <EnableInterrupt.h>
#include "DCMotors.h" 
#include "Sensor.h"
#include "NodeTypes.h"

#define isrPinRight 8
#define isrPinLeft 3

volatile bool isrInProcess  = false;

DCMotors<10,18,19,11,14,15> motors; //enL, L1, L2, enR, R1, R2
Sensor<2,3,4,5,6,7,8, 9, 16> sensors; // 16 for extended sensor
byte nType;

float Kp=7.5, Ki=5 ,Kd=0.08;

float P=0, D=0;
volatile float PID_value=0;
volatile float I=0;
float error=0;
float previous_error=0, previous_I=0;
char c;

byte route[50];
byte counter=0;

void setup() {
  
  motors.configure();
  sensors.configure();
  Serial.begin(9600);
  
  invalidate();
  
}

void loop() {

      if(isrInProcess){
              //Serial.println("looping");
              return;
        }
     
      sensors.updateError();
      nType = sensors.getNodeType();
      processRoute();
      calculate_pid();
        

                if(nType == -1){motors.drive((int)PID_value);}
                else if (nType == NODE_TYPE_RIGHT) {
                    enableInterrupt(isrPinLeft, isrRightTurnComplete, RISING );
                  }
                else if (nType == NODE_TYPE_LEFT){
                    enableInterrupt(isrPinRight, isrLeftTurnComplete, RISING );
                  }
              
          
    delay(1);
}

void processRoute(){

  switch (nType){
    case NODE_TYPE_GOAL:
    // write code to save to EPROM
    break;

    case -1:
    // do nothing
    break;

    default:
      route[counter] = nType;

      if(counter > 2){
        if(route[counter-2] == NODE_TYPE_LEFT && route[counter-1] == NODE_TYPE_U_TURN && route[counter] == NODE_TYPE_LEFT){
          route[counter-2] = NODE_TYPE_STRAIGHT;
          counter = counter -2; 
          return;
        }
        else if(route[counter-2] == NODE_TYPE_STRAIGHT && route[counter-1] == NODE_TYPE_U_TURN && route[counter] == NODE_TYPE_LEFT){
          route[counter-2] = NODE_TYPE_RIGHT;
          counter = counter -2; 
          return; 
        }
      }
      counter++;
      break;
  }
  
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
