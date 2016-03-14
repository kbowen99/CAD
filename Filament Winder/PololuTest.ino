//Pololu Test
//Step Pin 12
//Dir Pin 11

#include <Servo.h> 

bool DEBUG = true;

int StepPin = 12;
int DirectionPin = 11;
int HighSensPin = A1;
int LowSensPin = A0;

int HighCal = 0;
int LowCal = 0;
unsigned long CurrentStep = 0;
unsigned long LastSteps = 0;
int GuidePos = 0;
int GuideDir = 1;
int OneRotationSteps = 675;
Servo Guide;


void setup() {
  // put your setup code here, to run once:

  Guide.attach(9);
  
  pinMode(StepPin, OUTPUT);
  pinMode(DirectionPin, OUTPUT);
  
  digitalWrite(DirectionPin, HIGH);
  
  Serial.begin(9600);
  
  HighCal = analogRead(HighSensPin);
  LowCal = analogRead(HighSensPin);
  
  PhotoCalibrate();
  delay(100);
  PhotoCalibrate();
  delay(100);
  PhotoCalibrate();
  delay(100);
  PhotoCalibrate();
  delay(100);
  PhotoCalibrate();
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  LaserSens();
  Guide.write(GuidePos);
  delay(100);
  
}

void StepToIt(int steps) {
 for(int i = 0; i < steps; i++){
   digitalWrite(StepPin, HIGH);
   delay(4);
   digitalWrite(StepPin, LOW);
   CurrentStep++;
   
   
     
    if (CurrentStep % OneRotationSteps == 0 && GuidePos < 150) {
     GuidePos = GuidePos + GuideDir; 
    }
    if (CurrentStep % OneRotationSteps == 0 && GuidePos == 150 || CurrentStep % OneRotationSteps == 0 && GuidePos == 0) {
     GuideDir = -(GuideDir);
     GuidePos = GuidePos + GuideDir; 
    }
   
 }
 Debug();
}

void PhotoCalibrate() {
  Serial.print("CALIBRATING HIGH SENSOR: ");
  HighCal = (HighCal + analogRead(HighSensPin)) / 2;
  Serial.println(HighCal);
  Serial.print("CALIBRATING LOW SENSOR: ");
  LowCal = (LowCal + analogRead(LowSensPin)) / 2;
  Serial.println(LowCal); 
}

void LaserSens(){
  //if Low sensor goes dark
  if (analogRead(LowSensPin) < (LowCal - 15)) {
    //Spin until High Sensor Goes Dark
    LastSteps = CurrentStep;
    while ((analogRead(HighSensPin) > (HighCal - 15)) && ((CurrentStep - LastSteps) < 75)) {
      StepToIt(5);
      delay(15); 
    }
  } 
}

void Debug() {
  if (DEBUG){
     Serial.print(CurrentStep);
     Serial.print(" Steps; ");
     Serial.print(GuidePos);
     Serial.println(" Guide Position;");
     
     Serial.print("PhotoVoltaic 1: ");
     Serial.print(analogRead(A0));
     Serial.print(" PhotoVoltaic 2: ");
     Serial.println(analogRead(A1));

  }
}
