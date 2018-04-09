
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include "Adafruit_VL6180X.h"

#define samples 10
#define vstepsize 4
#define rstepsize 1
#define finished_threshold 1

unsigned long start_time = 0;
unsigned long end_time = 0;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor2 = AFMS.getStepper(200, 2);
Adafruit_StepperMotor *myMotor1 = AFMS.getStepper(200, 1);
Adafruit_VL6180X vl = Adafruit_VL6180X();

void setup() {
//    while (!Serial) {
//    delay(1);
//  }
  pinMode(8, INPUT_PULLUP);
  myMotor2->setSpeed(40);  // 10 rpm   
  Serial.begin(115200);           // set up Serial library at 9600 bps
  

  AFMS.begin();  // create with the default frequency 1.6KHz

  //myMotor2->step(2000,FORWARD,MICROSTEP);
  //myMotor1->step(2000, FORWARD, SINGLE);

  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  //Serial.println("Adafruit VL6180x test!");
  if (! vl.begin()) {
    //Serial.println("Failed to find sensor");
    while (1);
  }
  //Serial.println("Sensor found!");

zero();  
scan();
Serial.println("done");
myMotor1-> release();
myMotor2-> release();
scan_time();

}


void loop() {
}


void zero(){
  while(digitalRead(8) == LOW){
    myMotor1->step(vstepsize, BACKWARD, INTERLEAVE);
  }
  
}
void scan(){
  delay(3000);
  start_time = millis();
  myMotor1->step(5, FORWARD, MICROSTEP);
  int vlimit = 380/vstepsize;
  int rlimit = 200/rstepsize;
  int finished = 0;
  for(int vstep = 0; vstep < vlimit; vstep++){
    for(int rstep = 0; rstep < rlimit; rstep++){
      double distance = 0;
      int samplecount = 0;
      double convergence = 1;
      while(convergence > 0.005){
      double old = distance;
      distance = 0;
      samplecount = 0;
      for (int i = 0; i < samples; i++){
          uint8_t range = vl.readRange();
          double dist = range;
          if (dist > 0 && dist < 100){
            distance = dist + distance;
            samplecount = samplecount + 1;
          }

      }
      if(samplecount > 0){
      distance = distance / samplecount;
      distance = distance*1.031 - 9.79;     //calibration
      }
      else{
        finished = finished +1;
      }
      //Serial.println(distance);
      convergence = abs(distance - old)/distance;
      //Serial.println(convergence);
    }
      int height_factor = vstep*vstepsize;
      Serial.print(vstep);
      Serial.print(",");
      double stepToDeg = rstep*1.8;
      Serial.print(stepToDeg);
      Serial.print(",");
      Serial.print(distance);
      Serial.println();
      myMotor2->step(rstepsize,FORWARD,MICROSTEP);
    }
    if(finished >= finished_threshold){
      break;
    }
    else {
      finished = 0;
    }
    Adafruit_StepperMotor *myMotor1 = AFMS.getStepper(200, 1);
    myMotor1->step(vstepsize, FORWARD, MICROSTEP);
    myMotor1-> release();

  }
}

void scan_time(){
  end_time = millis();
  unsigned long total_time = end_time - start_time;
long day = 86400000;
long hour = 3600000;
long minute = 60000;
long second =  1000;

int days = total_time / day ;                                //number of days
int hours = (total_time % day) / hour;                       //the remainder from days division (in milliseconds) divided by hours, this gives the full hours
int minutes = ((total_time % day) % hour) / minute ;         //and so on...
int seconds = (((total_time % day) % hour) % minute) / second;

 Serial.print(days,DEC);
 Serial.print(hours,DEC);
 Serial.print(minutes,DEC);
 Serial.print(seconds,DEC);
 Serial.println();  
}


