#include "Arduino.h"
#include "TLOB.h"

TLOB::TLOB(int led0Pin, int led1Pin, int led2Pin, int buttonPin){
  pinMode(led0Pin, OUTPUT);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  ledPins[0] = led0Pin;
  ledPins[1] = led1Pin;
  ledPins[2] = led2Pin;
  _buttonPin = buttonPin;
  // default debounce time
  debounceTime = 25;
  holdTimer = 0;
}

 // starts blinking, optionally specify the time led stays on and off. Always starts on.

void TLOB::blink(int led, unsigned int onTime, unsigned int offTime){

  // if no offTime is entered, offTime and onTime are equal.
  if (offTime == 0){
    offTime = onTime;
  }

  // check for valid led selection
  if (led >= 0 && led <= 2){
    // turn on blinking, the turning on and off of the leds is handled in updateleds()
    blinking[led] = true;
    blinkOn[led] = onTime;
    blinkOff[led] = offTime;
    blinkStart[led] = millis();
  }
}

void TLOB::blinkAll(unsigned int onTime, unsigned int offTime){
  blink(0, onTime, offTime);
  blink(1, onTime, offTime);
  blink(2, onTime, offTime);
}

// stops the blinking, does not turn the led off
void TLOB::stop(int led){
  // check for valid led selection
  if (led >= 0 && led <= 2){
    blinking[led] = false;
  }
}

void TLOB::stopAll(){
  // stops all blinking.
  blinking[0] = false;
  blinking[1] = false;
  blinking[2] = false;
}


// read the button values, these are updated in updateButton()

// is the button down
bool TLOB::buttonDown(){
  return buttonDown;
}

// has the button been pressed since the last updateButton(), debounced
bool TLOB::buttonPressed(){
  return buttonPressed;
}

// has the button been released since the last updateButton(), debounced or delayed if pressed within the debounce time
bool TLOB::buttonReleased(){
  return buttonReleased;
}

// how long has the button been held. 0 if not down
unsigned long TLOB::buttonHold(){
  return buttonHold;
}

// change the debounce time
int TLOB::debounceTime(unsigned int time){
  // default of time is 0
  if(time){
    debounceTime = time;
  }
  return debounceTime;
}

bool TLOB::led(int led, bool state){
  // set led
  if (led >= 0 && led <= 2){
    digitalWrite(ledPins[led], state);
    return leds[led] = state;
  }
}

bool TLOB::led(int led){
  // read led state
  if (led >= 0 && led <= 2){
    return leds[led];
  }
}

void TLOB::ledAll(bool state){
  led(0, state);
  led(1, state);
  led(2, state);
}

// select next number, wraps around from 2 to 0.
int TLOB::next(int n){
  return (n + 1) % 3;
}

// select prevous number, wraps around from 0 to 2.
int TLOB::prev(int n){
  return (n + 2) % 3;
}

// update both button & leds
void TLOB::update(){
  updateButton();
  updateLeds();
}

// update button, recommended to do at the beginning or end of every loop
void TLOB::updateButton(){

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

// update the leds for blinking, recommended to do at the end of every loop
void TLOB::updateLeds(){
  
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
  digitalWrite( ledPins[0], leds[0]);
  digitalWrite( ledPins[1], leds[1]);
  digitalWrite( ledPins[2], leds[2]);
}