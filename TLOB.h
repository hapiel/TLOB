/*
  TLOB 3L1B 3 leds 1 button library 0.1
*/

#ifndef TLOB_h
#define TLOB_h

#include "Arduino.h"

class TLOB {
  public:
    TLOB(int led0Pin, int led1Pin, int led2Pin, int buttonPin, int buttonMode = 0);

    void blink(int led, unsigned int onTime = 100, unsigned int offTime = 0);
    void blinkAll(unsigned int onTime = 100, unsigned int offTime = 0);
    void stop(int led);
    void stopAll();

    bool buttonDown();
    bool buttonPressed();
    bool buttonReleased();
    unsigned long buttonHold();

    int debounceTime(unsigned int time = 0);

    bool led(int led, bool state);
    bool led(int led);
    void ledAll(bool state);

    int next(int n);
    int prev(int n);

    void update();
    void updateButton();
    void updateLeds();


  private:
    int _debounceTime;
    int ledPins[3];
    int _buttonPin;
    bool buttonState;
    unsigned long holdTimer;
    unsigned long blinkStart[3];
    int _buttonMode;
    bool _buttonDown;
    bool _buttonPressed;
    bool _buttonReleased;
    unsigned long _buttonHold;
    bool leds[3];
    bool blinking[3];
    unsigned int blinkOn[3];
    unsigned int blinkOff[3];
};

#endif