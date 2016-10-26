// Basic Bluetooth sketch HC-05_AT_MODE_01
// Communicate with a HC-05 using the serial monitor
//
// The HC-05 defaults to communication mode when first powered on you will need to manually enter AT mode
// The default baud rate for AT mode is 38400
// See www.martyncurrey.com for details
//
 
 
#include <SoftwareSerial.h>
SoftwareSerial BTserial(2, 3); // RX | TX
// Connect the HC-05 TX to Arduino pin 2 RX. 
// Connect the HC-05 RX to Arduino pin 3 TX through a voltage divider.
// 
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

int i = 0;
char c = ' ';
String intVal;
int redVal = 0;
int blueVal = 0;
int greenVal = 0;
#define ledPin 7 //LED PIN Normal LED
#define rgbPIN 9 // Pin that will be used for the RGB strip
#define rgdPIX 10 // Number of pixels in the strip

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(rgdPIX, rgbPIN, NEO_GRB + NEO_KHZ800);

enum
{
  kSetLed, // Command to request led to be set in specific state
};

void setup() 
{
    Serial.begin(9600);
    Serial.println("Arduino is ready");
    Serial.println("Remember to select Both NL & CR in the serial monitor");
 
    // HC-05 default serial speed for AT mode is 38400
    BTserial.begin(38400);
    // initialize the ledPin as an output:
    pinMode(ledPin, OUTPUT);

    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}
 
void loop()
{
 
    // Keep reading from HC-05 and send to Arduino Serial Monitor
    if (BTserial.available())
    {  
        intVal = BTserial.read();
        Serial.print(intVal);
        //BTserial.print(intVal);
        //BTserial.print('\n');
        Serial.print('\n');
        Serial.print(String(intVal));
        Serial.print('\n');
        //String redVal = getValue(String(intVal), '58', 0);
        //String blueVal = getValue(String(intVal), '58', 1);
        //String greenVal = getValue(String(intVal), '58', 2);
        //BTserial.println(redVal);
        //BTserial.println(blueVal);
        //BTserial.println(greenVal);
        //Serial.print(redVal);
        //Serial.print(blueVal);
        //Serial.print(greenVal);
        switch (i) {
        case 1:
          redVal = intVal.toInt();
          //colorWipe(strip.Color(255, 0, 0), 50); //Red
          Serial.print(redVal);
          break;
        case 2:
          //colorWipe(strip.Color(0, 0, 255), 50); //Blue
          // throw out the value
          break;
        case 3:
          blueVal = intVal.toInt();
          //colorWipe(strip.Color(0, 255, 0), 50); //Green
          Serial.print(blueVal);
          break;
        case 4:
          //colorWipe(strip.Color(255, 90, 0), 50); //Yellow
          // throw out the value
          break;
        case 5:
          greenVal = intVal.toInt();
          //colorWipe(strip.Color(255, 255, 255), 50); //White
          Serial.print(greenVal);
          break;
        case 6:
          //colorWipe(strip.Color(0, 0, 0), 50); //Off
          i = 0;
          break;
        default:
          // Do nothing here
          // Optional
          break;
        }
        i++;
    }
 
    // Keep reading from Arduino Serial Monitor and send to HC-05
    if (Serial.available())
    {
        intVal = Serial.readString();
        BTserial.print(intVal); 
        Serial.print(intVal);
        Serial.print('\n');
        //String redVal = getValue(String(intVal), '58', 0);
        //String blueVal = getValue(String(intVal), '58', 1);
        //String greenVal = getValue(String(intVal), '58', 2);
        //Serial.println(redVal);
        //Serial.println(blueVal);
        //Serial.println(greenVal);
        switch (intVal.toInt()) {
        case 1:
          colorWipe(strip.Color(255, 0, 0), 50);
          break;
        case 2:
          colorWipe(strip.Color(0, 255, 0), 50);
          break;
        case 3:
          colorWipe(strip.Color(0, 0, 255), 50);
          break;
        default:
          // Do nothing here
          // Optional
          break;
        }
    }
}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

String getValue(String data, char separator, int index)
{
 int found = 0;
  int strIndex[] = {
0, -1  };
  int maxIndex = data.length()-1;
  for(int i=0; i<=maxIndex && found<=index; i++){
  if(data.charAt(i)==separator || i==maxIndex){
  found++;
  strIndex[0] = strIndex[1]+1;
  strIndex[1] = (i == maxIndex) ? i+1 : i;
  }
 }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}


