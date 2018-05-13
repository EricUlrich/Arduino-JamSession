//#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

// connect motor controller pins to Arduino digital pins
// L298N Motor Drive Controller Board Module Dual H Bridge 
// 
// motor one
const int MotorASpeed = 10; // Motor A/1 enable pin also speed
const int MotorAOutput1 = 9;
const int MotorAOutput2 = 8;
// motor two
const int MotorBSpeed = 5; // Motor B/2 enable pin also speed
const int MotorBOutput1 = 7;
const int MotorBOutput2 = 6;

// Set motor Speeds
// 50=12.5s, 55=10.3s, 60=9s, 65=8s, 70=7.2s, 75=6.5s, 80=6.1, 85=5.9s, 90=5.7
int MotorSpeedTable[] = {70,75,80,85,90,95,100,105,110};

const int buttonPin = 2;
const int ledPin = 3;

// Variables will change:
int ButtonPushCount = 0;
int ledState = LOW;
int buttonState;
int lastButtonState = HIGH;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup()  
{
  lcd.begin(20,4); 
  lcd.setCursor(3,0);
  lcd.print("Auto-Annealer");
  lcd.setCursor(8,1);
  lcd.print("****");
  lcd.setCursor(3,2);
  lcd.print("LED State: On");
  lcd.setCursor(8,3);
  lcd.print("****");

// set all the motor control pins to outputs
  pinMode(MotorASpeed, OUTPUT);
  pinMode(MotorBSpeed, OUTPUT);
  pinMode(MotorAOutput1, OUTPUT);
  pinMode(MotorAOutput2, OUTPUT);
  pinMode(MotorBOutput1, OUTPUT);
  pinMode(MotorBOutput2, OUTPUT);

  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // set initial LED state
  digitalWrite(ledPin, ledState);
  
}
void SetMotorsOn(int MotorSpeed=60) {
 // turn on motor A
 digitalWrite(MotorAOutput1, HIGH);
 digitalWrite(MotorAOutput2, LOW);
 // set speed out of possible range 0~255
 analogWrite(MotorASpeed, MotorSpeed);
 // turn on motor B
 digitalWrite(MotorBOutput1, HIGH);
 digitalWrite(MotorBOutput2, LOW);
 // set speed out of possible range 0~255
 if (!MotorSpeed) {
  analogWrite(MotorBSpeed, 0);
 }
 else {
 analogWrite(MotorBSpeed, MotorSpeed + 28);
 }
 
}
 
 void SetMotorsOff()
 {
 // now turn off motors
 digitalWrite(MotorAOutput1, LOW);
 digitalWrite(MotorAOutput2, LOW);
 digitalWrite(MotorBOutput1, LOW);
 digitalWrite(MotorBOutput2, LOW);
 }


void LCD_Write_Line(int LineNumber, int StartPosition, String LineText) {
  //LineNumber always starts with 0
  // StartPosition always starts with 0
  //lcd.begin(20,4); 
  lcd.setCursor(StartPosition,LineNumber);
  lcd.print(LineText);
  
}

void loop()
{
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        ledState = !ledState;
        String MotorPower = "Motor Speed: ";
        MotorPower += MotorSpeedTable[ButtonPushCount];
        lcd.clear();
        LCD_Write_Line(3,1, MotorPower);
        SetMotorsOn(MotorSpeedTable[ButtonPushCount]);

        if (ButtonPushCount == 9)
        {
          ButtonPushCount = 0;
        }
        else
        {
          ButtonPushCount ++;
        }
        
        String tmpStr = String(ButtonPushCount);
        LCD_Write_Line(1,1, tmpStr);
      }
    }
  }

  // set the LED:
  digitalWrite(ledPin, ledState);
  lastButtonState = reading;

  if (ledState == HIGH) {
    LCD_Write_Line(2,14,"On ");
    //SetMotorsOn(MotorSpeed);
    //LCD_Write_Line(3,8,"Motor RPM: " + MotorSpeed);
    //MotorSpeed = MotorSpeed + 5;
  }
  if (ledState != HIGH) {
    LCD_Write_Line(2,14,"Off");
    //SetMotorsOff();
  }  


}



