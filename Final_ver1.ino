#include <Stepper.h>

int in1Pin = 12;
int in2Pin = 11;
int in3Pin = 10;
int in4Pin = 9;
int actuator1[] = {53, 52};
int actuator2[] = {46, 47};

int steps = 10; //10

//Names Of Strips 
String strip="";
String strip1="Strip1";
int strip1loc=5;//31
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
int resetPin = 35;
int resetPin2 = 36;
int resetPin3 = 37;

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
  
  int i; //for actuators
  for( i = 0; i < 2; i++){
    pinMode(actuator1[i], OUTPUT);
    pinMode(actuator2[i], OUTPUT);
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
         StepperRight();
       delay(2000);
       findDelay();
       delay(2000);
       reset2();
       delay(2000);
       Serial.println("Going For Column using delay..." );
       int c = actuatorDelay * actuatorCount;
       actuator1_up();
       delay(c);
       actuator1_stop();
       Serial.println("Reached desired column" );
       checkUsingIR();    
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
       while(true){
//       StepperLeft();
          test(15, 200, 100, 0); // speed, revolution, count, dir
       }
     }
     else if(strip==actuator_pos_fw){
     while(true){
     actuator1_up();
     Serial.println("Going UP" );
     delay(1000);
     actuator1_down();
     Serial.println("Going DOWN" );
     delay(1000);
     }
//     actuator1_stop();
//       test2();
     }
     else if(strip==actuator_pos_bw){
     while(true){
     actuator1_down();
     Serial.println("Going DOWN" );
     delay(1000);  
   }
     //delay(1000);
     //actuator1_stop();
     }
     else{
     //do nothing
     }
   
   }
   
}

void reset(){

  while(check){
    StepperLeft();
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
void reset2(){
  Serial.println("Now Restarting....." );

  while(check2){
  actuator1_down();
  actuator2_down();
    if(digitalRead(resetPin2) == LOW){
      check2=false;
    }
  }
  actuator1_stop();
  actuator2_stop();
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
    actuator1_up();
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


void StepperRight(){
  motor.step(steps);
  Serial.print("steps:" );
  Serial.println(stepCount);
  stepCount++;
  delay(500);
}

void StepperLeft(){
  motor.step(-steps);
  Serial.print("Going for strips using steps:" );
  Serial.println(stepCount);
  
}

  void actuator1_up(){
  digitalWrite(actuator1[0], HIGH);
  digitalWrite(actuator1[1], LOW);
  }
  void actuator1_down(){
  digitalWrite(actuator1[0], LOW);
  digitalWrite(actuator1[1], HIGH);
  }
  void actuator1_stop(){
  digitalWrite(actuator1[0], LOW);
  digitalWrite(actuator1[1], LOW);
  Serial.println("Stopping......");
  }

  void actuator2_up(){
  digitalWrite(actuator2[0], HIGH);
  digitalWrite(actuator2[1], LOW);
  }
  void actuator2_down(){
  digitalWrite(actuator2[0], LOW);
  digitalWrite(actuator2[1], HIGH);
  }
  void actuator2_stop(){
  digitalWrite(actuator2[0], LOW);
  digitalWrite(actuator2[1], LOW);
  Serial.println("Stopping......");
  }


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
  void test2(String s,int d){
    actuator1_up();
//    delay(1000);
//    actuator1_down();
//    delay(1000);
//    actuator1_stop();
//        
//    actuator2_up();
//    delay(1000);
//    actuator2_down();
//    delay(1000);
//    actuator2_stop();

}
