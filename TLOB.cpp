#include "Arduino.h"
#include "TLOB.h"

TLOB::TLOB(int led0Pin, int led1Pin, int led2Pin, int buttonPin){
  pinMode(led0Pin, OUTPUT);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  _led0Pin = led0Pin;
  _led1Pin = led1Pin;
  _led2Pin = led2Pin;
  _buttonPin = buttonPin;
  // default debounce time
  debounceTime = 15;
  holdTimer = 0;
}

void TLOB::updateButton(){
  // update button on the beginning of each step. 

  // TODO: add debouncing

  // false by default
  buttonReleased = false;
  buttonPressed = false;
  buttonHold = 0;

  // get state ASAP so all reads are definitely the same, except if within debounce time.
  if (holdTimer + debounceTime < millis()){
    buttonState = digitalRead(_buttonPin);
  }

  // no button down registered yet
  if (!buttonDown) {

    // but physical button is down
    if (buttonState) {
      buttonPressed = true;

      // record time
      holdTimer = millis();
    }

  } else {

    // if button is still down
    if (buttonState) {
      // time since time recorded
      buttonHold = millis() - holdTimer;

    // button was released this step
    } else {
      buttonReleased = true;
    }
  }

  // set buttonDown to physical button state
  buttonDown = buttonState;
  
}

void TLOB::updateLeds(){
  // update leds at end of each step.
  
  // blinking
  for (int led = 0; led < 3; led++){
    if (blinking[led]){
      if ((millis() - blinkStart[led]) % (blinkOn[led] + blinkOff[led]) < blinkOn[led]){
        leds[led] = true;
      } else {
        leds[led] = false;
      }
    }
  }
  
  // update the leds
  digitalWrite( _led0Pin, leds[0]);
  digitalWrite( _led1Pin, leds[1]);
  digitalWrite( _led2Pin, leds[2]);
}

void TLOB::update(){
  updateButton();
  updateLeds();
}

void TLOB::allOn(){
  leds[0] = true;
  leds[1] = true;
  leds[2] = true;
}

void TLOB::allOff(){
  leds[0] = false;
  leds[1] = false;
  leds[2] = false;
}

int TLOB::next(int n){
  return (n + 1) % 3;
}

int TLOB::prev(int n){
  return (n + 2) % 3;
}

void TLOB::blink(int led, unsigned int onTime, unsigned int offTime){
  // starts blinking, optionally specify the time led stays on and off. Always starts on.
  if (offTime == 0){
    offTime = onTime;
  }
  blinking[led] = true;
  blinkOn[led] = onTime;
  blinkOff[led] = offTime;
  blinkStart[led] = millis();
}
    
    
void TLOB::stop(int led){
  //stops the blinking.
  blinking[led] = false;
}

void TLOB::allStop(){
  //stops all blinking.
  blinking[0] = false;
  blinking[1] = false;
  blinking[2] = false;
}