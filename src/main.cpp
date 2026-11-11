#include <Arduino.h>
#include <Servo.h>

// Create servo objects
Servo FR_Shoulder;
Servo FR_Elbow;
Servo FR_Ankle;
Servo FL_Shoulder;
Servo FL_Elbow;
Servo FL_Ankle;
Servo BR_Shoulder;
Servo BR_Elbow;
Servo BR_Ankle;
Servo BL_Shoulder;
Servo BL_Elbow;
Servo BL_Ankle;

// Pin assignments
int FR_ShoulderPin = 2;
int FR_ElbowPin = 3;
int FR_AnklePin = 4;
int FL_ShoulderPin = 5;
int FL_ElbowPin = 6;
int FL_AnklePin = 7;
int BR_ShoulderPin = 8;
int BR_ElbowPin = 9;
int BR_AnklePin = 10;
int BL_ShoulderPin = 11;
int BL_ElbowPin = 12;
int BL_AnklePin = 13;

int C1Pin = 52;
long C1PulseWidth = 0;
long lastC1PulseWidth = 1500;

int buttonPin = 22;
int buttonState = 0;

// Position variables
int liftUp = 60;
int liftDown = 120;
int oppositeLiftUp = 180 - liftUp;
int oppositeLiftDown = 180 - liftDown;

int stateChanges[] = {60, 120, 180, 240};
int FR_ShoulderPos[] = {120, 60, 60, 120, 120};
int FR_ElbowPos[] = {120, 120, 60, 60, 120};
int FR_AnklePos[] = {60, 60, 120, 120, 60};
int FL_ShoulderPos[] = {60, 120, 120, 60, 60};
int FL_ElbowPos[] = {60, 60, 120, 120, 60};
int FL_AnklePos[] = {150, 150, 90, 90, 150};

int count = 0;
int stepDelay = 1;

void setup()
{
  // initialize pins
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(C1Pin, INPUT);
  FR_Shoulder.attach(FR_ShoulderPin);
  FR_Elbow.attach(FR_ElbowPin);
  FR_Ankle.attach(FR_AnklePin);
  FL_Shoulder.attach(FL_ShoulderPin);
  FL_Elbow.attach(FL_ElbowPin);
  FL_Ankle.attach(FL_AnklePin);
  BR_Shoulder.attach(BR_ShoulderPin);
  BR_Elbow.attach(BR_ElbowPin);
  BR_Ankle.attach(BR_AnklePin);
  BL_Shoulder.attach(BL_ShoulderPin);
  BL_Elbow.attach(BL_ElbowPin);
  BL_Ankle.attach(BL_AnklePin);

  // Start neutral
  FR_Shoulder.write(FR_ShoulderPos[0]);
  FR_Elbow.write(FR_ElbowPos[0]);
  FR_Ankle.write(FR_AnklePos[0]);
  FL_Shoulder.write(FL_ShoulderPos[0]);
  FL_Elbow.write(FL_ElbowPos[0]);
  FL_Ankle.write(FL_AnklePos[0]);

  Serial.begin(9600);
  delay(1000);
}

void loop()
{
  buttonState = digitalRead(buttonPin);
  C1PulseWidth = pulseIn(C1Pin, HIGH, 25000);
  if (C1PulseWidth < 1000 || C1PulseWidth > 2000)
  {
    C1PulseWidth = lastC1PulseWidth;
  }
  if (buttonState == LOW)
  {
    if (C1PulseWidth > 1700)
    {
      stepCycle(count);
      count++;
    }
    else if (C1PulseWidth < 1350)
    {
      stepCycle(count);
      count--;
    }
  }
  lastC1PulseWidth = C1PulseWidth;
}

void stepCycle(int state)
{
  int offset;
  state = state % stateChanges[3];
  if (state < 0)
  {
    state += stateChanges[3];
  }

  // lift leg
  if (state <= stateChanges[0])
  {
    offset = state;
    FR_Shoulder.write(FR_ShoulderPos[0] - offset);
    FL_Shoulder.write(FL_ShoulderPos[2] - offset);
    delay(stepDelay);
  }
  // swing forward
  else if (state <= stateChanges[1])
  {
    offset = state - stateChanges[0];
    FR_Elbow.write(FR_ElbowPos[1] - offset);
    FR_Ankle.write(FR_AnklePos[1] + offset);
    FL_Elbow.write(FL_ElbowPos[3] - offset);
    FL_Ankle.write(FL_AnklePos[3] + offset);
    delay(stepDelay);
  }
  // leg down
  else if (state <= stateChanges[2])
  {
    offset = state - stateChanges[1];
    FR_Shoulder.write(FR_ShoulderPos[2] + offset);
    FL_Shoulder.write(FL_ShoulderPos[0] + offset);
    delay(stepDelay);
  }
  // push backwards
  else if (state <= stateChanges[3])
  {
    offset = state - stateChanges[2];
    FR_Elbow.write(FR_ElbowPos[3] + offset);
    FR_Ankle.write(FR_AnklePos[3] - offset);
    FL_Elbow.write(FL_ElbowPos[1] + offset);
    FL_Ankle.write(FL_AnklePos[1] - offset);
    delay(stepDelay);
  }
}
