#include <Arduino.h>
#include <TM1637Display.h>

// Module connection pins (Digital Pins)
const int CLK = 2;
const int CLK2 = 4;
const int DIO = 3;
const int DIO2 = 5;

// Button connection pins (Analog Pins)
const int BTN_P1 = A0;
const int BTN_P2 = A1;
const int BTN_RESET = A2;

const int RESET_ARDUINO = A3;

// Seven seg displays
TM1637Display display1(CLK, DIO);
TM1637Display display2(CLK2, DIO2);

double timer1 = 0;
double timer2 = 0;
int player = 2;

int btnP1State = 0;
int btnP2State = 0;
int btnResetState = 0;

int resetDownMillis = 0;
bool active = false;

unsigned long lastTime = micros();

int formatTime(int seconds) {
    int minutes = seconds / 60; // Convert seconds to minutes
    seconds = seconds % 60; // Remaining seconds after converting to minutes
    return minutes * 100 + seconds; // Formatting to MMSS
}

void resetArduino() {
  digitalWrite(RESET_ARDUINO, HIGH);
}

void checkForReset() {
  if(btnResetState == HIGH) {
    if(resetDownMillis == 0) resetDownMillis = millis();

    int differenceInSeconds = (millis() - resetDownMillis) * 0.001;
    if(differenceInSeconds > 1) {
      display1.setBrightness(0x00);
      display2.setBrightness(0x00);

      uint8_t SEG_RSET[] = {
        SEG_E | SEG_G,                                 // r
        SEG_A | SEG_C | SEG_D | SEG_F | SEG_G,         // s
        SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,         // e
        SEG_D | SEG_E | SEG_F | SEG_G                  // t
      };

      display1.setSegments(SEG_RSET);
      display2.setSegments(SEG_RSET);

      // while(digitalRead(BTN_RESET) == HIGH) delay(10);
      resetArduino();
    }
  } else {
    resetDownMillis = 0;
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(BTN_P1, INPUT);
  pinMode(BTN_P2, INPUT);
  pinMode(BTN_RESET, INPUT);

  while(digitalRead(BTN_RESET) == HIGH) delay(10);
}

void loop() {
  unsigned long currentTime = micros();
  unsigned long deltaTime = (currentTime - lastTime);

  btnP1State = digitalRead(BTN_P1);
  btnP2State = digitalRead(BTN_P2);
  btnResetState = digitalRead(BTN_RESET);

  display1.showNumberDecEx(formatTime((int)timer1), (0x80 >> 1), true);
  display2.showNumberDecEx(formatTime((int)timer2), (0x80 >> 1), true);

  checkForReset();

  if(active == true) {
    if(player == 1) {
      timer1 -= deltaTime * 0.000001;

      display1.setBrightness(0xff);
      display2.setBrightness(0x00);

      if(btnP1State == HIGH) player = 2;
    } else if(player == 2) {
      timer2 -= deltaTime * 0.000001;

      display1.setBrightness(0x00);
      display2.setBrightness(0xff);

      if(btnP2State == HIGH) player = 1;
    }
  } else if(btnP1State == HIGH && timer1 >= 60) {
    active = true;
    player = 1;
  } else if(btnP2State == HIGH && timer1 >= 60) {
    active = true;
    player = 2;
  } else if(btnResetState == HIGH) {
    timer1 += 60;
    timer2 += 60;

    while(digitalRead(BTN_RESET) == HIGH) {
      checkForReset();
      delay(10);
    }
  } else {
    display1.setBrightness(0xff);
    display2.setBrightness(0xff);
  }

  if(timer1 <= 0 && active == true) {
    timer1 = 0;
    display1.showNumberDecEx(formatTime((int)timer1), (0x80 >> 1), true);

    while(true) {
      if(digitalRead(BTN_RESET)) resetArduino();
      delay(10);
    }
  } else if(timer2 <= 0 && active == true) {
    timer2 = 0;
    display2.showNumberDecEx(formatTime((int)timer2), (0x80 >> 1), true);

    while(true) {
      if(digitalRead(BTN_RESET)) resetArduino();
      delay(10);
    }
  }

  lastTime = currentTime;
}
