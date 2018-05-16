#define EN        8  

//Direction pin
#define X_DIR     5 
#define Y_DIR     6
#define Z_DIR     7

//Step pin
#define X_STP     2
#define Y_STP     3 
#define Z_STP     4 

//End Stops
#define X_END 9
#define Y_END 10
#define Z_END 11

//DRV8825
//int defaultdelay=30; //Delay between each pause (uS)
//int stps=6400;// Steps to move 1/32  200 full steps * 32 = 6400 steps per rev

//A4988 Stepper Motor Driver
int defaultdelay=4500; //Delay between each pause (uS)
// Steps to move 200 full steps = 200 steps per rev
// half steps = 400, quarter steps = 800, eighth steps = 1600
int stps=800;

long count = 0;
int Hall_X_End = 0;
int Hall_Y_End = 0;
int Hall_Z_End = 0;

int prev_val = 1;
unsigned long startTime, endTime; //time variables

void step(boolean dir, byte dirPin, byte stepperPin, int steps, int delaytime)
{

  digitalWrite(dirPin, dir);
  //delay(100);
  for (int i = 0; i < steps; i++) {
    digitalWrite(stepperPin, HIGH);
    delayMicroseconds(delaytime); 
    digitalWrite(stepperPin, LOW);
    delayMicroseconds(delaytime); 
  }

}

// Move the main drum motor and the feeder motor in concert
void Dance()
{
  // X driver = Drum motor CCW
  digitalWrite(X_DIR, true);
  
  for (int i = 0; i < stps; i++) {
    digitalWrite(X_STP, HIGH);
    delayMicroseconds(defaultdelay); 
    digitalWrite(X_STP, LOW);
    delayMicroseconds(defaultdelay);

    //Hall_X_End = digitalRead(X_END);
    //Serial.println(Hall_X_End);
    // Y driver = Feeder motor CW 3/8 of a turn
    // adding the remaining 
    if (i <= 300) {
      digitalWrite(Y_DIR, true);
      digitalWrite(Y_STP, HIGH);
      digitalWrite(Y_STP, LOW);
    }
    else if (i >= 301 && i <= 600) {
      digitalWrite(Y_DIR, false);
      digitalWrite(Y_STP, LOW);
      digitalWrite(Y_STP, HIGH);
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(X_DIR, OUTPUT); 
  pinMode(X_STP, OUTPUT);
  pinMode(Y_DIR, OUTPUT); 
  pinMode(Y_STP, OUTPUT);
  //pinMode(Z_DIR, OUTPUT); 
  //pinMode(Z_STP, OUTPUT);
  pinMode(X_END, INPUT_PULLUP);
  pinMode(Y_END, INPUT_PULLUP);
  // Enable motors, if disabled they will spin freely
  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);
  
  // Home the motors to the hall sensors
  do
  {
    Hall_X_End = digitalRead(X_END);
    digitalWrite(X_STP, HIGH);
    delayMicroseconds(defaultdelay); 
    digitalWrite(X_STP, LOW);
    delayMicroseconds(defaultdelay);
  } while (Hall_X_End != LOW);
  delay(1000);
  do
  {
    Hall_Y_End = digitalRead(Y_END);
    digitalWrite(Y_STP, HIGH);
    delayMicroseconds(defaultdelay); 
    digitalWrite(Y_STP, LOW);
    delayMicroseconds(defaultdelay);
  } while (Hall_Y_End != LOW);
  delay(1000);


}

void loop() {
  // delay of 2000 ~ 80 RPM
  // delay of 3000 ~ 50 RPM
  // delay of 4000 ~ 38 RPM
  
  //step(false, X_DIR, X_STP, stps, 2000); //X, Clockwise
  //step(false, Y_DIR, Y_STP, stps, defaultdelay); //Y, Clockwise
  //step(false, Z_DIR, Z_STP, stps, defaultdelay); //Z, Clockwise
  //delay(1000);

 //step(true, X_DIR, X_STP, stps, defaultdelay); //X, Counterclockwise
 //step(true, Y_DIR, Y_STP, stps, defaultdelay); //Y, Counterclockwise
 //step(true, Z_DIR, Z_STP, stps, defaultdelay); //X, Counterclockwise

//get start time before the stepper rotation
startTime = micros();
//Serial.println(cur_t);
  Dance();
 //delay(100);

//increment count 
count ++;
//get stop time after the stepper rotation
endTime = micros();
// Add LCD screen print here instead of serial output
//Serial.println((endTime - startTime)); //print the start and end time in micros
Serial.println(1000000 * 60 / (endTime - startTime)); //print the rpm
//Serial.println(Hall_X_End);

}
