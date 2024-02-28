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

int timer1 = 500;
int timer2 = 600;

int formatTime(int seconds) {
    int minutes = seconds / 60; // Convert seconds to minutes
    seconds = seconds % 60; // Remaining seconds after converting to minutes
    return minutes * 100 + seconds; // Formatting to MMSS
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  timer1 -= 1;
  timer2 -= 1; 

  display.setBrightness(0x0f);
  display2.setBrightness(0x0f);

  if(n % 2 == 1) {
    display.showNumberDecEx(formatTime(n), (0x80 >> 0), false);
  } else {
    display.showNumberDecEx(formatTime(n), (0x80 >> 1), false);
  }

  delay(DELAY);
}