#include <AccelStepper.h>

// for the Arduino Uno + CNC shield V3

#define MOTOR_X_ENABLE_PIN 8
#define MOTOR_X_STEP_PIN 2
#define MOTOR_X_DIR_PIN 5

#define MOTOR_Y_ENABLE_PIN 8
#define MOTOR_Y_STEP_PIN 3
#define MOTOR_Y_DIR_PIN 6

#define MOTOR_Z_ENABLE_PIN 8
#define MOTOR_Z_STEP_PIN 4
#define MOTOR_Z_DIR_PIN 7



AccelStepper motorX(1, MOTOR_X_STEP_PIN, MOTOR_X_DIR_PIN); 
AccelStepper motorY(1, MOTOR_Y_STEP_PIN, MOTOR_Y_DIR_PIN); 
//AccelStepper motorZ(1, MOTOR_Z_STEP_PIN, MOTOR_Z_DIR_PIN); 

// counter if needed
long count = 0;

String stringRead;

void setup()
{
  // initialize the serial port:
  Serial.begin(115200);
  // Stepper motor
  pinMode(MOTOR_X_ENABLE_PIN, OUTPUT);
  pinMode(MOTOR_Y_ENABLE_PIN, OUTPUT);
  motorX.setEnablePin(MOTOR_X_ENABLE_PIN);
  motorX.setPinsInverted(false, false, true);
  motorY.setEnablePin(MOTOR_Y_ENABLE_PIN);
  motorY.setPinsInverted(false, false, true);

  motorX.setAcceleration(100);
  motorX.moveTo(20000);
  motorX.setMaxSpeed(50);
  //motorX.setSpeed(50);

  motorY.setAcceleration(100); 
  motorY.moveTo(25);
  motorY.setMaxSpeed(5);
  //motorY.setSpeed(15);

  motorX.enableOutputs();
  motorY.enableOutputs();
}
void MotorY_Feed(int Posi = 0)
{
  //unsigned long num = motorY.currentPosition();
  //Serial.println(num);
  
    motorY.moveTo(Posi);
    //motorY.run();
}

void loop()
{
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming first byte:
    //incomingByte = Serial.read();
    // read the incoming string:
    stringRead = Serial.readString();
    if (stringRead.equals("motorx faster"))
      {
        // say what you got:
        Serial.print("I received: ");
        Serial.print(stringRead);
        Serial.println('\n');
        motorX.setSpeed(motorX.speed() + 50);
        motorX.runSpeed();
       }
      else if (stringRead.equals("motorx slower"))
      {
        // say what you got:
        Serial.print("I received: ");
        Serial.print(stringRead);
        Serial.println('\n');
        motorX.setSpeed(motorX.speed() - 50);
        motorX.runSpeed();
       }
      else if (stringRead.equals("help"))
      {
        // if nothing else matches, do the default
        // default is optional
        // say what you got:
        Serial.print("I received: ");
        Serial.print(stringRead);
        Serial.println('\n');
        Serial.print("Commands: motorx faster, motorx slower, help");
        Serial.println('\n');
      }
      else
      {
        // if nothing else matches, do the default
        // default is optional
        // say what you got:
        Serial.print("I received: ");
        Serial.print(stringRead);
        Serial.println('\n');

      }
    }
    else
    {
      //motorX.runSpeed();
      //motorY.run();
    }
  // 
  //Serial.println("Motor X");
  //motorX.run();
  //otorX.runSpeed();
  //delay(5000);
  // 
  //Serial.println("Motor Y");
  // Change direction at the limits
  if (motorY.distanceToGo() == 0){
    motorY.moveTo(-motorY.currentPosition());
  }
  
  motorX.run();
  motorY.run();
 
  //motorY.runSpeed();
  //delay(5000);

  //delay(10000);
  //motorX.stop();
  //motorY.stop();
  //increment count 
  count ++;
  if (count == 201) { count = 0;}
  Serial.println(count);
}
