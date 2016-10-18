#include <Stepper.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 9 // Pin that will be used for the RGB strip
#define PIX 19 // Number of pixels in the strip
#define INTERNAL_MOTOR_STEPS 32 // Number of revolutions of the Internal Motor in 4-step mode
#define OUTPUT_SHAFT_REVS 32 * 64 //2048  Number of revolutions of the Output Shaft
#define In1 3 // Pin for motor driver In1
#define In2 4 // Pin for motor driver In2
#define In3 5 // Pin for motor driver In3
#define In4 6 // Pin for motor driver In4

// 4 pins connected to driver In1, In2, In3, In4  
// sequence 1-3-2-4 for proper sequencing
Stepper small_stepper(INTERNAL_MOTOR_STEPS, In1, In3, In2, In4);

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIX, PIN, NEO_GRB + NEO_KHZ800);



byte byteRead;
String stringRead;
int incomingByte = 0;  // for incoming serial data
int Steps;

enum
{
  kSetLed, // Command to request led to be set in specific state
};


void setup() {                
// Turn the Serial Protocol ON
  Serial.begin(115200);
    // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code


  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {

  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming first byte:
    //incomingByte = Serial.read();
    // read the incoming string:
    stringRead = Serial.readString();
    if (stringRead.equals("Red"))
      {
        // say what you got:
        Serial.print("I received: ");
        Serial.print(stringRead);
        Serial.println('\n');
        theaterChase(strip.Color(127, 0, 0), 50); // Red
        colorWipe(strip.Color(255, 0, 0), 50); // Red
       }
      else if (stringRead.equals("Blue"))
      {
        // say what you got:
        Serial.print("I received: ");
        Serial.print(stringRead);
        Serial.println('\n');
        theaterChase(strip.Color(0, 0, 127), 50); // Blue
        colorWipe(strip.Color(0, 0, 255), 50); // Blue
       }
      else if (stringRead.equals("Green"))
      {
        // say what you got:
        Serial.print("I received: ");
        Serial.print(stringRead);
        Serial.println('\n');
        theaterChase(strip.Color(0, 127, 0), 50); // Green
        colorWipe(strip.Color(0, 255, 0), 50); // Green        
       }
      else if (stringRead.equals("Yellow"))
      {
        // if nothing else matches, do the default
        // default is optional
        // say what you got:
        Serial.print("I received: ");
        Serial.print(stringRead);
        Serial.println('\n');
        theaterChase(strip.Color(127, 45, 25), 50); // Yellow
        colorWipe(strip.Color(255, 90, 0), 50); // Yellow
      }
      else if (stringRead.equals("Wipe"))
      {
        // if nothing else matches, do the default
        // default is optional
        // say what you got:
        Serial.print("I received: ");
        Serial.print(stringRead);
        Serial.println('\n');
        // theaterChase(strip.Color(127, 45, 25), 50); // Yellow
        colorWipe(strip.Color(0, 0, 0), 50); // Off
        strip.show(); // Initialize all pixels to 'off'
      }
      else if (stringRead.equals("Help"))
      {
        // if nothing else matches, do the default
        // default is optional
        // say what you got:
        Serial.print("I received: ");
        Serial.print(stringRead);
        Serial.println('\n');
        Serial.print("Commands: Red, Blue, Green, Yellow, Wipe, Forward, Back, Dump, Help");
        Serial.println('\n');
        // theaterChase(strip.Color(127, 45, 25), 50); // Yellow
        // colorWipe(strip.Color(0, 0, 0), 50); // Off
        // strip.show(); // Initialize all pixels to 'off'
      }
      // Move the stepper Forward 1 full rotation clockwise
      else if (stringRead.equals("Forward"))
      {
        Serial.print("I received: ");
        Serial.print(stringRead);
        Serial.println('\n');
        Steps  =  OUTPUT_SHAFT_REVS ;  // Rotate CW 1 turn
        small_stepper.setSpeed(500);   
        small_stepper.step(Steps);
        delay(1000); 
      }
      // Move the stepper Forward 1 full rotation counter clockwise
      else if (stringRead.equals("Back"))
      {
        Serial.print("I received: ");
        Serial.print(stringRead);
        Serial.println('\n');
        Steps  =  - OUTPUT_SHAFT_REVS ;  // Rotate CCW 1 turn
        small_stepper.setSpeed(500);   
        small_stepper.step(Steps);
        delay(1000); 
      }
      else
      {
        // if nothing else matches, do the default
        // default is optional
        // say what you got:
        Serial.print("I received: ");
        Serial.print(stringRead);
        Serial.println('\n');
        theaterChase(strip.Color(127, 127, 127), 50); // White
        colorWipe(strip.Color(127, 127, 127), 50); // White
      }
    // say what you got:
    //Serial.print("I received: ");
    //Serial.print(stringRead);
    //Serial.print(incomingByte);
    //Serial.println(incomingByte, DEC);
    
  }
  // theaterChase(strip.Color(127, 127, 127), 50); // White
  // theaterChase(strip.Color(127, 0, 0), 50); // Red
  // theaterChase(strip.Color(0, 0, 127), 50); // Blue

  // rainbow(20);
  // rainbowCycle(20);
  // theaterChaseRainbow(50);
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




