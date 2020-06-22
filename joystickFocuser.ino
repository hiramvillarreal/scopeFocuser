/*
   Telescope electronic focuser with Unipolar stepper motor with 2 speed
   range and Joystick control with Arduino
   This is a free software with NO WARRANTY.
*/

// include Arduino stepper motor library
#include <Stepper.h>

// define number of steps per revolution
#define STEPS 32

// define stepper motor control pins
#define IN1  10
#define IN2  11
#define IN3  12
#define IN4  13

// initialize stepper library
Stepper stepper(STEPS, IN4, IN2, IN3, IN1);

// vrxPin pot output is connected to Arduino A0
#define vrxPin  A0
#define swPin  15

//Change state detecion variables
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
bool halfSpeed = false;

void setup()
{
  pinMode(swPin, INPUT);
}

void loop()
{
  //Change state detecion for full speed or half speed 
  buttonState = digitalRead(swPin);
  if (buttonState != lastButtonState) {
    //Serial.print (halfSpeed);
    if (buttonState == LOW) {
      if (halfSpeed == false) {
        halfSpeed = true;
      }
      else {
        halfSpeed = false;
      }
      delay(50);
    }
  }
  lastButtonState = buttonState;
  // read analog value from the potentiometer
  int val = analogRead(vrxPin);

  // if the joystic is in the middle ===> stop the motor
  if (  (val > 500) && (val < 523) )
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }

  else
  {
    // move the motor in the first direction
    while (val >= 523)
    {
      //half speed if halfSpeed bool
      if (halfSpeed == true) {
        int halfVal = (val - 523) / 2;
        val = val - halfVal;
      }
      // map the speed between 5 and 500 rpm
      int speed_ = map(val, 523, 1023, 5, 500);
      // set motor speed
      stepper.setSpeed(speed_);
      // move the motor (1 step)
      stepper.step(1);

      val = analogRead(vrxPin);
    }

    // move the motor in the other direction
    while (val <= 500)
    {
      if (halfSpeed == true) {
        int halfVal = (val - 500) / 2;
        val = val - halfVal;
      }
      // map the speed between 5 and 500 rpm
      int speed_ = map(val, 500, 0, 5, 500);
      // set motor speed
      stepper.setSpeed(speed_);

      // move the motor (1 step)
      stepper.step(-1);

      val = analogRead(vrxPin);
    }
  }
}
