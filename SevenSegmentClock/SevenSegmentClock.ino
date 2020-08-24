
#include <FastLED.h>
#include <DS3231.h>
#include <Adafruit_NeoPixel.h> 
#include <Wire.h>

#define NUM_LEDS 58
#define DATA_PIN 6

CRGB leds[NUM_LEDS];
DS3231 rtc;

const int TOTAL_SEGMENTS = 4; // Total amount of segments
const int LEDS_PER_SEGMENT = 7; // Amount of LEDs per segment
const int DISPLAY_SEGMENT[] = {23, 16, 7, 0}; // LED starting position of each segment
const int DISPLAY_NUMBER[][14] = { // True: Lit, False:  Not lit
  {false, true, true, true, true, true, true}, // 0
  {false, true, false, false, false, false, true}, // 1
  {true, true, true, false, true, true, false}, // 2
  {true, true, true, false, false, true, true}, // 3
  {true, true, false, true, false, false, true}, // 4
  {true, false, true, true, false, true, true}, // 5
  {true, false, true, true, true, true, true}, // 6
  {false, true, true, false, false, false, true}, // 7
  {true, true, true, true, true, true, true}, // 8
  {true, true, true, true, false, false, true}, // 9
};

int red = 100;
int green = 0;
int blue = 0;
char currentFade = 'r';

void setup() {
  Serial.begin(115200);
  Serial.println("Starting execution");
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(40); // Lower brightness
  
  Wire.begin();

  // The following lines can be uncommented to set the date and time
  rtc.setHour(9);     // Set the hour to 15 (24hr format)
  rtc.setMinute(30);   // Set the minute to 12
}

void loop() {
  bool h12, pm;
  int hour = rtc.getHour(h12, pm); // Get the hour
  int minute = rtc.getMinute(); // Get the minute

  int hourFirstDigit = hour / 10; // Take the first digit
  int hourSecondDigit = hour % 10; // Take the second digit

  int minuteFirstDigit = minute / 10; // Take the first digit
  int minuteSecondDigit = minute % 10; // Take the second digit
  
  int totalDelay = 0;
  while (totalDelay < 10000) { // 10 seconds
    if (currentFade == 'r') { // If red is currently fading
      red--;
      green++;
      currentFade = red <= 0 ? 'g' : 'r'; // Change currentFade if necessary
    } else if (currentFade == 'g') { // If green is currently fading
      green--;
      blue++;
      currentFade = green <= 0 ? 'b' : 'g'; // Change currentFade if necessary
    } else { // If blue is currently fading
      blue--;
      red++;
      currentFade = blue <= 0 ? 'r' : 'b'; // Change currentFade if necessary
    }
    FastLED.clear(); // Clear the LEDs
    displayNumber(3, hourFirstDigit);
    displayNumber(2, hourSecondDigit);
    displayNumber(1, minuteFirstDigit);
    displayNumber(0, minuteSecondDigit);
    leds[14].setRGB(red, green, blue); // Light the dots
    leds[15].setRGB(red, green, blue);
    FastLED.show(); // Show the current LEDs
    delay(10);
    totalDelay += 10;
  }
}

void displayNumber(int segment, int number) {
  for (int j = 0; j < LEDS_PER_SEGMENT; j++) { // Loop over each LED of said segment
    if (DISPLAY_NUMBER[number][j]) { // If this LED should be on
      leds[DISPLAY_SEGMENT[segment] + j].setRGB(red, green, blue); // Turn it on
    }
  }
}
