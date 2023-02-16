// Include the Servo library 
#include <Servo.h> 

// Creamos dos objetos tipo 'servo', uno del eje horizontal y otro para el vertical. 
Servo servo_h, servo_v; 

// Declaramos variables globales para definir los limites de operacion de los servos.

// Limites del servo horizontal.
int position_h = 180; 
int servo_h_LimitLow = 10;
int servo_h_LimitHigh = 170;


// Limites del servo vertical.
int position_v = 60; 
int servo_v_LimitLow = 1;
int servo_v_LimitHigh = 90;



void setup() { 
  // Pegamos los 2 servos a algún pin de I/O para poder controlarlos; se conectan al pin 2 y 3, respectivamente.
  servo_h.attach(2);
  servo_v.attach(3);
  Serial.begin(9600);

  // Configuramos por defecto la angulación de los servos en 180 y 45 grados. 
  servo_h.write(0);
  servo_v.write(0);
  delay(2500);
}

void loop(){ 
  int TL = analogRead(A1); // top left
  int TR = analogRead(A2); // top right
  int BL = analogRead(A3); // down left
  int BR = analogRead(A4); // down right
  
  Serial.print("Valor analogico de LDR TL: ");
  Serial.print(TL);
  Serial.print("\n");
  if (TL > 500 ) servo_v.write(180);
  else servo_v.write(90);

  Serial.print("Valor analogico de LDR BR: ");
  Serial.print(BR);
  Serial.print("\n");
  if (BR > 500) servo_h.write(180);
  else servo_h.write(90);

  delay(1000);
}






