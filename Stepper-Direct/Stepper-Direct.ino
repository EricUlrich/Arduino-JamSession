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
int stps=800;// Steps to move 200 full steps = 200 steps per rev

long count = 0;
int hallstate = 0;

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

void Dance()
{
  // X driver = Drum motor CCW
  digitalWrite(X_DIR, true);
  
  for (int i = 0; i < stps; i++) {
    digitalWrite(X_STP, HIGH);
    delayMicroseconds(defaultdelay); 
    digitalWrite(X_STP, LOW);
    delayMicroseconds(defaultdelay);
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

  // adding a 5 second startup delay to manually HOME both motors.
  // will add homeing code later with switches or sensors.
  if (count == 0 ) {
  delay(5000);
  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);
  }
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
  Dance();
 //delay(100);
hallstate = digitalRead(X_END);
if (hallstate == LOW) {
  delay(2000);
}
//increment count 
count ++;
//Serial.println(count);

}
