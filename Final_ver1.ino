#include <Stepper.h>

//Stepper Pins
int in1Pin = 12;
int in2Pin = 11;
int in3Pin = 10;
int in4Pin = 9;

//Actuator Pins
int actuator1[] = {40, 41}; // the one that goes up and down
int actuator2[] = {42, 43}; // the one that pushes

//Motor Pins
int motor1[] = {44, 45};
int motor2[] = {47, 46};

//Motor Strings
String mforward ="fwd";
String mbackward ="bwd";
String mleft = "left";
String mright = "right";

int steps = 10; //Default value is 10

//Names Of Strips 
String strip="";
String strip1="Strip1";
int strip1loc=36;//31
String strip2="Strip2";
int strip2loc=95;
String strip3="Strip3";
int strip3loc=150;

//Actuator Strings
String actuator_pos_fw="FW";
String actuator_pos_bw="BW";
String first_actuator="Actuator1";
String second_actuator="Actuator2";
//String firstValue;
//String secondValue;
//String thirdValue;
//String fourthValue;
//boolean str_chr=false;
//boolean str_chr2=false;
int actuatorCount=3;
int actuatorDelay=0;
int resetPin = 35; //for stepper
int resetPin2 = 36; //for reset, the one inside
int resetPin3 = 37; //for delay, the one outside

int sensorValue = 0;

String alwaysRotate = "rotate";

boolean check = true;
boolean check2 = true;
boolean check3 = true;

Stepper motor(200, in1Pin, in2Pin, in3Pin, in4Pin);  

int stepCount = 0; 
//boolean Rdirection = true;


void setup() {
  // put your setup code here, to run once:
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(resetPin, INPUT);
  pinMode(resetPin2, INPUT);
  pinMode(resetPin3, INPUT);
  
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  
  int i; //for actuators & motors
  for( i = 0; i < 2; i++){
    pinMode(actuator1[i], OUTPUT);
    pinMode(actuator2[i], OUTPUT);
    
    pinMode(motor1[i], OUTPUT);
    pinMode(motor2[i], OUTPUT);
    }

  motor.setSpeed(10); //Default value = 10
  
  while (!Serial);
  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
    
   if (Serial.available() > 0){ 
    
     strip = Serial.readString();
      if(strip==strip1){
       reset2();
       reset();
       while(stepCount!=strip1loc)
         StepperLeft();
       delay(2000);
       findDelay();
       delay(2000);
       reset2();
       delay(2000);
       Serial.println("Going For Column using delay..." );
       int c = actuatorDelay * actuatorCount;
       actuator1_down();
       delay(c);
       actuator1_stop();
       Serial.println("Reached desired column" );
       checkUsingIR();
       delay(5000);
       Serial.println("Going to push" );
       actuator2_up();
       delay(2000);
       actuator2_down();
       delay(2000);
       actuator2_stop();
       Serial.println("---Completed Successfully!!---");    
    
     }
     
     
     else if(strip==strip2){
       while(stepCount!=strip2loc)
           StepperRight();
       checkUsingIR();
     }
     
     else if(strip==strip3){
       while(stepCount!=strip3loc)
         StepperRight();
       checkUsingIR();
     }
     else if(strip==alwaysRotate){
//       while(true){
//       StepperLeft();
          test(15, 200, 100, 0); // speed, revolution, count, dir
//       }
     }
     else if(strip==actuator_pos_fw){
//     while (true){
       test2(actuator_pos_fw,1000); //test2 for actuator 1
       //test3(actuator_pos_fw,1000); // test3 for actuator 2
//       }
     }
     else if(strip==actuator_pos_bw){
//      while (true){
       test2(actuator_pos_bw,1000);
       test3(actuator_pos_bw,1000);
//        }
     }
     else if(strip==mforward){
     motor_forward();
     delay(1000);
     motor_stop();
     }
     else if(strip==mbackward){
     motor_backward();
     delay(1000);
     motor_stop();
     }
     else if(strip==mleft){
     motor_left();  
     delay(5000);
     motor_stop();     
     }
     else if(strip==mright){
     motor_right();
     delay(5000);
     motor_stop();     
     }     
     else{
     //do nothing
     }
   
   }
   
}

void reset(){ //this method resets the stepper

  while(check){
    //StepperLeft();
    StepperRight();
    if(digitalRead(resetPin) == LOW){
      check=false;
    }
  }
  stepCount=0;
  Serial.println("After Reset" );
  Serial.print("Steps:" );
  Serial.println(stepCount);
  check=true;
}


//Not Working
//void reset2(){
//  Serial.println("Now Restarting....." );
//
//  while(digitalRead(resetPin3) == HIGH){
//  actuator1_down();
//  actuator2_down();
// }
//  actuator1_stop();
//  actuator2_stop();
//  Serial.println("Restarting Completed ! !" );
//}

//Working
void reset2(){ //this method resets the actuators
  Serial.println("Now Restarting....." );

  while(check2){
  //actuator1_down();
  actuator1_up();
  //actuator2_down();
    if(digitalRead(resetPin2) == LOW){
      check2=false;
    }
  }
  actuator1_stop();
  actuator2_stop();
  delay(1000);
  //for correction
  actuator1_down();
  delay(300);
  actuator1_stop();
  Serial.println("Restarting Completed ! !" );
  check2=true;
}


//Not Working
//void findDelay(){
//  Serial.println("Looking for delay....." );
//  int x = 0;
//  x = millis();
//  while(digitalRead(resetPin2) == HIGH){
//  actuator1_up();
//  //actuator2_up();
// }
//  actuator1_stop();
//  //actuator2_stop();
//  int y = 0;
//  y = millis();
//  int z = y-x;
//  actuatorDelay = z / 5;
//  Serial.println("Search Completed ! !" );
//}

//Working
void findDelay(){
  Serial.println("Looking for delay....." );
  int x = 0;
  x = millis();
  
  //delay(10000);
  while(check3){
    //actuator1_up();
    actuator1_down();
    //actuator2_up();
    if(digitalRead(resetPin3) == LOW){
      check3=false;
     // break;
    }
  }
  actuator1_stop();
  //actuator2_stop();
  int y = 0;
  y = millis();
  int z = y-x;
  actuatorDelay = z / 5;
  //actuatorDelay = 2000;
  Serial.println("Search Completed ! !" );
  check3=true;
}



void checkUsingIR(){
   // read the input on analog pin 0:
    sensorValue = analogRead(A0);
    printSensorValue();     
   if(sensorValue > 50 ) {
     // print out the value you read:
     printSensorValue();
     delay(500);
     Serial.println("Strip found");
     // motor.step(0);
     delay(500);
     } 
     else {
     printSensorValue();
     Serial.println("No Strip found Going left");
        StepperLeft();
     sensorValue = analogRead(A0);
     printSensorValue();
        if(sensorValue < 50){
          Serial.println("No Strip found Going right");
          StepperRight();
          delay(1000);
          StepperRight();
          printSensorValue();
          checkUsingIR();
        }
        delay(500);
     }
}
  void printSensorValue(){
    Serial.println("Sensor Value : ");
       Serial.print(sensorValue);
       Serial.println(""); 
  }

//Method-Calls for Actions...

//Steppers.........................
void StepperRight(){
  motor.step(steps);
  Serial.print("Right steps:" );
  //Serial.print("Going for strips using steps:" );
  Serial.println(stepCount);
  //stepCount++;
  //delay(500);
}

void StepperLeft(){
  motor.step(-steps);
  //Serial.print("Going for strips using steps:" );
  Serial.print("Left steps:" );
  Serial.println(stepCount);
  stepCount++;
  delay(500);
}
//........................................

//Actuators...............
  void actuator1_up(){
  digitalWrite(actuator1[0], HIGH);
  digitalWrite(actuator1[1], LOW);
  Serial.println("Going Up......");  
  }
  void actuator1_down(){
  digitalWrite(actuator1[0], LOW);
  digitalWrite(actuator1[1], HIGH);
  Serial.println("Going Down......");
  }
  void actuator1_stop(){
  digitalWrite(actuator1[0], LOW);
  digitalWrite(actuator1[1], LOW);
  Serial.println("Act 1 Stopping......");
  }

  void actuator2_up(){
  digitalWrite(actuator2[0], HIGH);
  digitalWrite(actuator2[1], LOW);
  Serial.println("Act2 Going Up......");
  }
  void actuator2_down(){
  digitalWrite(actuator2[0], LOW);
  digitalWrite(actuator2[1], HIGH);
  Serial.println("Act2 Going Down......");
  }
  void actuator2_stop(){
  digitalWrite(actuator2[0], LOW);
  digitalWrite(actuator2[1], LOW);
  Serial.println("Act2 Stopping......");
  }
//...............................

//Motor
  void motor_forward(){
  digitalWrite(motor1[0], HIGH);
  //digitalWrite(motor1[1], LOW);
  
  digitalWrite(motor2[0], HIGH);
  //digitalWrite(motor2[1], LOW);
  
  Serial.println("Forward");
  }
  
  void motor_backward(){
  //digitalWrite(motor1[0], LOW);
  digitalWrite(motor1[1], HIGH);
  
  //digitalWrite(motor2[0], LOW);
  digitalWrite(motor2[1], HIGH);  
  Serial.println("Backward");
  }
  
  void motor_left(){
  digitalWrite(motor1[0], HIGH);
  
  digitalWrite(motor2[1], HIGH);
  Serial.println("Left");
  }
  
  void motor_right(){
  digitalWrite(motor1[1], HIGH);
  
  digitalWrite(motor2[0], HIGH);
  Serial.println("Right");
  }
  
  void motor_stop(){
  digitalWrite(motor1[0], LOW);
  digitalWrite(motor1[1], LOW);
  
  digitalWrite(motor2[0], LOW);
  digitalWrite(motor2[1], LOW);

  }

//


//tester methods
void test(int spd, int stepsPerRevolution, int count, int dir) {
  Serial.println("clockwise");
  motor.setSpeed(spd);
  for(int x=0; x < count; x++) {
    if(dir == 0)
      motor.step(stepsPerRevolution);
    else
      motor.step(-stepsPerRevolution);
     Serial.println(x);
  }
  delay(500);
}
//test2 for actuator 1
  void test2(String s,int d){
  if (s==actuator_pos_fw){
    actuator1_up();
    delay(d);
    actuator1_stop();
    }
    else if(s==actuator_pos_bw){
    actuator1_down();
    delay(d);
    actuator1_stop();
    }
}
//test2 for actuator 2
  void test3(String s,int d){
  if (s==actuator_pos_fw){
    actuator2_up();
    delay(d);
    actuator2_stop();
    }
    else if(s==actuator_pos_bw){
    actuator2_down();
    delay(d);
    actuator2_stop();
    }
}
