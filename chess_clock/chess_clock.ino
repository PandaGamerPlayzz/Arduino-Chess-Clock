#include <Arduino.h>
#include <TM1637Display.h>

// Module connection pins (Digital Pins)
#define CLK  2
#define CLK2 4
#define DIO  3
#define DIO2 5

// The amount of time (in milliseconds) between tests
#define DELAY 1000

TM1637Display display(CLK, DIO);
TM1637Display display2(CLK2, DIO2);

double timer1 = 500.0;
double timer2 = 500.0;
int player = 2;

unsigned long lastTime = micros();

int formatTime(int seconds) {
    int minutes = seconds / 60; // Convert seconds to minutes
    seconds = seconds % 60; // Remaining seconds after converting to minutes
    return minutes * 100 + seconds; // Formatting to MMSS
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  unsigned long currentTime = micros();
  unsigned long deltaTime = (currentTime - lastTime);

  Serial.println(timer2);

  if(player == 1) {
    timer1 -= deltaTime * 0.000001;

    display.setBrightness(0xff);
    display2.setBrightness(0x00);
  } else if(player == 2) {
    timer2 -= deltaTime * 0.000001;

    display.setBrightness(0x00);
    display2.setBrightness(0xff);
  }

  display.showNumberDecEx(formatTime((int)timer1), (0x80 >> 1), false);
  display2.showNumberDecEx(formatTime((int)timer2), (0x80 >> 1), false);

  lastTime = currentTime;
}