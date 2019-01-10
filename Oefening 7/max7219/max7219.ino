#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Max72xxPanel.h"

int pinCS = 10; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int numberOfHorizontalDisplays = 1;
int numberOfVerticalDisplays = 1;

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

int wait = 2000; // In milliseconds

void setup() {

  matrix.setIntensity(7); // Use a value between 0 and 15 for brightness

  matrix.setRotation(0, 1);
}

const unsigned char myBitmap [] PROGMEM = {
    0b11111111,
    0b11000001,
    0b10111101,
    0b10110101,
    0b10100101,
    0b10111101,
    0b10000001,
    0b11111111
};

void loop() {

    matrix.fillScreen(LOW);
    matrix.drawChar(1, 0, '?', HIGH, LOW, 1);
    matrix.write(); // Send bitmap to display
    delay(wait);

    matrix.fillScreen(LOW);
    matrix.drawCircle(4,4,2,1);
    matrix.write(); // Send bitmap to display
    delay(wait);

    matrix.fillScreen(LOW);
    matrix.drawTriangle(1, 1, 1, 7, 7, 7, 1);
    matrix.write(); // Send bitmap to display
    delay(wait);

    matrix.fillScreen(LOW);
    matrix.drawRoundRect(0, 0, 8, 8, 2, 1);
    matrix.write(); // Send bitmap to display
    delay(wait);

    matrix.fillScreen(LOW);
    matrix.drawBitmap(0, 0, myBitmap, 8, 8, 1);
    matrix.write(); // Send bitmap to display
    delay(wait);
}

