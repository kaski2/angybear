const int buttonPin = 2;
const int ledPin = 12;
const int ledPin2 = 3;
const int pirPin = 4;
const int buzzer = 8; 
int buttonState = 0;
bool buttonToggle = false;
#include "pitches.h";

int calibrationTime = 30;
int lastState = 0;
long unsigned int lowIn;
long unsigned int pause = 5000;
boolean lockLow = true;
boolean takeLowTime;
int PIRValue = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(pirPin, INPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  delay(2000);
}

bool checkButtonState(){
// read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {
    return true;
  }else {
    return false;
  }
}

int PIR() {
   if(digitalRead(pirPin) == HIGH) {
      if(lockLow) {
         PIRValue = 1;
         lockLow = false;
         Serial.println("Motion detected.");
         delay(50);
      }
      takeLowTime = true;
   }
   if(digitalRead(pirPin) == LOW) {
      if(takeLowTime){
         lowIn = millis();takeLowTime = false;
      }
      if(!lockLow && millis() - lowIn > pause) {
         PIRValue = 0;
         lockLow = true;
         Serial.println("Motion ended.");
         delay(50);
      }
   }
   return PIRValue;
}

void song(){
  for(int i=0; i<sizeof(notes);i++){
    if(!checkButtonState()){
      if(notes[i] == 0){
        noTone(buzzer);
        delay(pauses[i]);
      } else{
        tone(buzzer, notes[i]);
        delay(pauses[i]);
      }
    }else {
      noTone(buzzer);
      break;
    }
  }
  analogWrite(buzzer, 0);
}



void loop() {
  if(PIR() == 1 && lastState == 0){
    lastState = 1;
    digitalWrite(ledPin, HIGH);
    digitalWrite(ledPin2, HIGH);
    song();
  }else if(PIR() == 0){
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin2, LOW);
    lastState = 0;
  }
}
