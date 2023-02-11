// Include the Servo library 
#include <Servo.h> 
// Declare the Servo pin 
int servoPin2 = 2; int servoPin3 = 3; 
// Create a servo object 
Servo Servo2, Servo3; 
void setup() { 
   // We need to attach the servo to the used pin number 
   Servo2.attach(servoPin2); 
   Servo3.attach(servoPin3); 
}
void loop(){ 
   // Make servo go to 0 degrees 
   Servo2.write(0); 
   Servo3.write(0); 
   delay(1000); 
   // Make servo go to 90 degrees 
   Servo2.write(90); 
   Servo3.write(90); 
   delay(1000); 
   // Make servo go to 180 degrees 
  Servo2.write(180); 
  Servo3.write(180); 
   delay(1000); 
}
