#include <IRLibAll.h>

IRsend mySender;
const int buttonPin = 2;
unsigned long debounceDelay = 50;
unsigned long lastDebounceTime = 0;
int buttonState = 0;
int lastButtonState = 0;
int clicks = 0;
int doubleClickTimeout = 300;
unsigned long flushClick = 0;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
}

void loop() {
  if (hitClickTimeout()) {
    if (clicks == 1) {
      mute();
    } else {
      fastForward();
    }

    flushClick = 0;
    clicks = 0;
  } else if (wasClicked()) {
    clicks++;
    if (flushClick == 0) {        
      flushClick = millis() + doubleClickTimeout;
    }
  }
}

bool hitClickTimeout() {
  return flushClick != 0 && millis() > flushClick;
}

bool wasClicked () {
  return buttonStateChanged(digitalRead(buttonPin)) && buttonState == HIGH;
}

bool buttonStateChanged(int reading) {
  bool stateChanged = false;
  
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      stateChanged = true;
    }
  }

  lastButtonState = reading;
  return stateChanged;
}

void mute () {
  Serial.println("Muting!");
  mySender.send(SONY, 0x290, 12);
}

void fastForward () {
  Serial.println("Fast forwarding!");
  mySender.send(NEC, 0xA10CE40B);
  mySender.send(NEC, 0xFFFFFFFF);
  mySender.send(NEC, 0xA10CE40B);
  mySender.send(NEC, 0xFFFFFFFF);
  mySender.send(NEC, 0xA10CE40B);
  mySender.send(NEC, 0xFFFFFFFF);
}

