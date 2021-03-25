/*
  TLOB 3l1b 3 lights 1 button WIP library
*/

#ifndef TLOB_h
#define TLOB_h

#include "Arduino.h"

class TLOB {
  public:
    TLOB(int led1Pin, int led2Pin, int led3Pin, int buttonPin);
    bool buttonDown;
    bool buttonPressed;
    bool buttonReleased;
    unsigned long buttonHold;
    bool leds[3];
    int debounceTime;
    void updateButton();
    void updateLeds();
    void update();
    void allOn();
    void allOff();
    void allStop();
    int next(int n);
    int prev(int n);
    void blink(int led, unsigned int onTime = 100, unsigned int offTime = 0);
    void stop(int led);
    bool blinking[3];
    unsigned int blinkOn[3];
    unsigned int blinkOff[3];

  private:
    int _led0Pin;
    int _led1Pin;
    int _led2Pin;
    int _buttonPin;
    bool buttonState;
    unsigned long holdTimer;
    unsigned long blinkStart[3];
};

#endif