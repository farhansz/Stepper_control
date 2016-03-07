#include <Stepper.h>

int in1Pin = 12;
int in2Pin = 11;
int in3Pin = 10;
int in4Pin = 9;

int steps = 10; //10
String strip="";
String strip1="Strip1";
int strip1loc=8;
String strip2="Strip2";
int strip2loc=13;
String strip3="Strip3";
int strip3loc=15;

int sensorValue = 0;

String alwaysRotate = "rotate";

boolean check = true;
int resetPin = 35;
Stepper motor(200, in1Pin, in2Pin, in3Pin, in4Pin);  

int stepCount = 0; 
boolean Rdirection = true;


void setup() {
  // put your setup code here, to run once:
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(resetPin, INPUT);
  
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);

  while (!Serial);
  
  Serial.begin(9600);
  motor.setSpeed(10); //10
}

void loop() {
  // put your main code here, to run repeatedly:
     
   if (Serial.available() > 0){ 
     reset();    
     strip = Serial.readString();
      if(strip==strip1){
       while(stepCount!=strip1loc)
         StepperLeft();
       checkUsingIR();
         
     }
     
     else if(strip==strip2){
       while(stepCount!=strip2loc)
           StepperLeft();
       checkUsingIR();
     }
     
     else if(strip==strip3){
       while(stepCount!=strip3loc)
         StepperLeft();
       checkUsingIR();
     }
     else if(strip==alwaysRotate){
       while(true){
//       StepperLeft();
          test(10, 200, 100, 0); // speed, revolution, count, dir
       }
     }
     else{
     //do nothing
     }
   
   }
   
}

void reset(){

  while(check){
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
}

void StepperLeft(){
  motor.step(-steps);
  Serial.print("Going for strips using steps:" );
  Serial.println(stepCount);
  stepCount++;
  delay(500);
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
