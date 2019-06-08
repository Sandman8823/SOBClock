
// Sandy Jones [sandyjonesmusic@gmail.com] S O B Clock
// Sat Obs Binary Clock RTC Version 1.0
// For use with Arduino Uno + DS3231 RTC

// Code segments copied shamelessly from:
// <kd5wxb@gmail.com> : DOY calculations.
// Adafruit RTC library examples for RTC integration.
// 

//This code aims to produce a binary clock interface from 6012 12 pixel RGBW rings
//To begin with, 2 rings showing DayOfYear and Hours/Minutes in Binary format. 
//Possible Future mods:
//**Third ring for seconds
//**Switch for 'normal'time reading[Day/Month]
//**Switch for local time (possibly integrated with positioning informatin from GPS)
//**GPS time version


//*****Main Code Body*****

//Included packages
#include <Wire.h>               //RTC Conncection protocol
#include "RTClib.h"             //Adafruit RTC library
#include <Adafruit_NeoPixel.h>  //Adafruit Neopixel Library for LED rings

RTC_DS3231 rtc;                  //Make a RTC DS3231 object

#define PIN 3                    //Data pin for LED rings
#define NUM_LEDS 24              //Number of LEDs
#define BRIGHTNESS 18               //Max Brightness setting

int DOY = 0;                     //Variable initialising
int yr = 0;
int hr = 0;
int mnt = 0;

int doy1 = 0;
int doy2 = 0;
int doy4 = 0;
int doy8 = 0;
int doy16 = 0;
int doy32 = 0;
int doy64 = 0;
int doy128 = 0;
int doy256 = 0;

int hr1 = 0;
int hr2 = 0;
int hr4 = 0;
int hr8 = 0;
int hr16 = 0;

int min1 = 0;
int min2 = 0;
int min4 = 0;
int min8 = 0;
int min16 = 0;
int min32 = 0;


//Description of type of pixel strip (or ring) being used
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800); 

//Matrix for fading LEDs
byte neopix_gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };
  
//Set names of weekdays
char Weekdays[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//byte DOYpixels[] = {4,3, 2, 1, 0, 11, 10 , 9, 8} ;
//byte HRpixels[] = {14,13,12,23,22};
//byte MNpixels[] = {15,16,17,18,19,20};

void setup() {
  //**Setup of RTC Clock 
 Serial.begin(9600); //Begin the Serial at 9600 Baud
 Wire.begin();
 rtc.begin();
 //Print the message if RTC is not available
 if (! rtc.begin()) {
   Serial.println("Couldn't find RTC");
   while (1);
 }
 //Setup of time if RTC lost power or time is not set
 if (rtc.lostPower()) {
   //Sets the code compilation time to RTC DS3231
   // To set to UTC, change computer time zone to UTC, upload, then change computer back to local time.
   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
 }
 

//Blingy intro light show
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

delay(200);


  pulseWhite(1); 

  // Some example procedures showing how to display to the pixels:
  colorWipe(strip.Color(255, 0, 0), 40); // Red
  colorWipe(strip.Color(0, 255, 0), 40); // Green
  colorWipe(strip.Color(0, 0, 255), 40); // Blue
  colorWipe(strip.Color(0, 0, 0, 255), 40); // White
  colorWipe(strip.Color(0,0,0), 40); //blk

}
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels()/2; i++) {
    strip.setPixelColor(i, c);
    strip.setPixelColor(i+12, c); // evenly dividing rings (12bit)
    strip.show();
    delay(wait);
  }
}

void pulseWhite(uint8_t wait) {
  //up
  for(int j = 0; j < 256 ; j++){
      for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(0,0,0, neopix_gamma[j] ) );
        }
        delay(wait);
        strip.show();
      }

  for(int j = 255; j >= 0 ; j--){
    //down
      for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(0,0,0, neopix_gamma[j] ) );
        }
        delay(wait);
        strip.show();
      }
}

int CalcDOY(int day, int month, int year) {
  
  // Given a day, month, and year (4 digit), returns 
  // the day of year. Errors return 999.
  
  int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
  
  // Verify we have a 4-digit year
  if (year < 1000) {
    return 999;
  }
  
  // Check if it is a leap year, this is confusing business
  // See: https://support.microsoft.com/en-us/kb/214019
  if (year%4  == 0) {
    if (year%100 != 0) {
      daysInMonth[1] = 29;
    }
    else {
      if (year%400 == 0) {
        daysInMonth[1] = 29;
      }
    }
   }

  // Make sure we are on a valid day of the month
  if (day < 1) 
  {
    return 999;
  } else if (day > daysInMonth[month-1]) {
    return 999;
  }
  
  int doy = 0;
  for (int i = 0; i < month - 1; i++) {
    doy += daysInMonth[i];
  }
  
  doy += day;
  return doy;

}

void loop() {
  
 //Set now as RTC time
 DateTime now = rtc.now();

DOY = CalcDOY(now.day(), now.month(), now.year());
yr = now.year();
hr = now.hour();
mnt = now.minute();

//Setting values for DOY from Binary values
doy1 = bitRead(DOY,0);
doy2 = bitRead(DOY,1);
doy4 = bitRead(DOY,2);
doy8 = bitRead(DOY,3);
doy16 = bitRead(DOY,4);
doy32 = bitRead(DOY,5);
doy64 = bitRead(DOY,6);
doy128 = bitRead(DOY,7);
doy256 = bitRead(DOY,8);

//Setting values for Hour from Binary values
hr1 = bitRead(hr,0);
hr2 = bitRead(hr,1);
hr4 = bitRead(hr,2);
hr8 = bitRead(hr,3);
hr16 = bitRead(hr,4);


//Setting values for Minute from Binary values
min1 = bitRead(mnt,0);
min2 = bitRead(mnt,1);
min4 = bitRead(mnt,2);
min8 = bitRead(mnt,3);
min16 = bitRead(mnt,4);
min32 = bitRead(mnt,5);


//ACTUAL PIXEL SETTING // **DOY**
if (doy1 == 1)
  {strip.setPixelColor(4,255,0,0);}
  else {strip.setPixelColor(4,0,0,0);}
if (doy2 == 1)
  {strip.setPixelColor(3,255,0,0);}
  else {strip.setPixelColor(3,0,0,0);}
if (doy4 == 1)
  {strip.setPixelColor(2,255,0,0);}
  else {strip.setPixelColor(2,0,0,0);}
if (doy8 == 1)
  {strip.setPixelColor(1,255,0,0);}
  else {strip.setPixelColor(1,0,0,0);}
if (doy16 == 1)
  {strip.setPixelColor(0,255,0,0);}
  else {strip.setPixelColor(0,0,0,0);}
if (doy32 == 1)
  {strip.setPixelColor(11,255,0,0);}
  else {strip.setPixelColor(11,0,0,0);}
if (doy64 == 1)
  {strip.setPixelColor(10,255,0,0);}
  else {strip.setPixelColor(10,0,0,0);}
if (doy128 == 1)
  {strip.setPixelColor(9,255,0,0);}
  else {strip.setPixelColor(9,0,0,0);}
if (doy256 == 1)
  {strip.setPixelColor(8,255,0,0);}
  else {strip.setPixelColor(8,0,0,0);}

//ACTUAL PIXEL SETTING // **HOUR** 
if (hr1 == 1)
  {strip.setPixelColor(14,0,255,0);}
  else {strip.setPixelColor(14,0,0,0);}
if (hr2 == 1)
  {strip.setPixelColor(13,0,255,0);}
  else {strip.setPixelColor(13,0,0,0);}
if (hr4 == 1)
  {strip.setPixelColor(12,0,255,0);}
  else {strip.setPixelColor(12,0,0,0);}
if (hr8 == 1)
  {strip.setPixelColor(23,0,255,0);}
  else {strip.setPixelColor(23,0,0,0);}
if (hr16 == 1)
  {strip.setPixelColor(22,0,255,0);}
  else {strip.setPixelColor(22,0,0,0);}

//ACTUAL PIXEL SETTING // **MINUTE** 
if (min1 == 1)
  {strip.setPixelColor(15,0,0,255,15);}
  else {strip.setPixelColor(15,0,0,0);}
if (min2 == 1)
  {strip.setPixelColor(16,0,0,255,15);}
  else {strip.setPixelColor(16,0,0,0);}
if (min4 == 1)
  {strip.setPixelColor(17,0,0,255,15);}
  else {strip.setPixelColor(17,0,0,0);}
if (min8 == 1)
  {strip.setPixelColor(18,0,0,255,15);}
  else {strip.setPixelColor(18,0,0,0);}
if (min16 == 1)
  {strip.setPixelColor(19,0,0,255,15);}
  else {strip.setPixelColor(19,0,0,0);}
if (min32 == 1)
  {strip.setPixelColor(20,0,0,255,15);}
  else {strip.setPixelColor(20,0,0,0);}
 
strip.show();
 
// Print RTC time to Serial Monitor
 Serial.print(now.year(), DEC);
 Serial.print('/');
 Serial.print(now.month(), DEC);
 Serial.print('/');
 Serial.print(now.day(), DEC);
 Serial.print(" (");
 Serial.print(Weekdays[now.dayOfTheWeek()]);
 Serial.print(") ");
 Serial.print(now.hour(), DEC);
 Serial.print(':');
 Serial.print(now.minute(), DEC);
 Serial.print(':');
 Serial.print(now.second(), DEC);
 Serial.print(' ');
 //Print time in binary format
 Serial.println(DOY);
 Serial.print(DOY, BIN);
 Serial.print(':');
 Serial.print(hr, BIN); 
 Serial.print(':');
 Serial.println(mnt, BIN);

 delay(1000);

}
