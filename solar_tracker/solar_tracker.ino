// Include the Servo library 
#include <Servo.h> 

// Creamos dos objetos tipo 'servo', uno del eje horizontal y otro para el vertical. 
Servo servo_h, servo_v; 

// Declaramos variables globales para definir los limites de operacion de los servos.

// Limites del servo horizontal.
int position_h = 0; 
int servo_h_LimitLow = 0;
int servo_h_LimitHigh = 180;


// Limites del servo vertical.
int position_v = 60; 
int servo_v_LimitLow = 0;
int servo_v_LimitHigh = 180;



void setup() { 
  // Pegamos los 2 servos a algún pin de I/O para poder controlarlos; se conectan al pin 2 y 3, respectivamente.
  servo_h.attach(2);
  servo_v.attach(3);
  Serial.begin(9600);

  // Configuramos por defecto la angulación de los servos en 180 y 45 grados. 
  servo_h.write(0);
  servo_v.write(0);
  delay(2000);
}

void horizontal_rotation(){

}

void loop(){ 
  int TL = analogRead(A1); // top left
  int TR = analogRead(A2); // top right
  int BL = analogRead(A3); // bottom left
  int BR = analogRead(A4); // bottom right

  /* Calculamos los promedios de los lados (arriba, abajo, izquierda y derecha)*/
  int average_top = (TL + TR) / 2;
  int average_bottom = (BL + BR) / 2;
  int average_left = (TL + BL) / 2;
  int average_right = (TR + BR) / 2;

  // Comenzamos a comparar los promedios de luz que se reciben de los LDRs


  // Primeros controlamos el servo horizontal (servo ubicado en la base)
  if (average_left > average_right) position_h -= 20 ;
  else position_h += 20;

  if (position_h < servo_h_LimitLow) position_h = servo_h_LimitLow;
  if (position_h > servo_h_LimitHigh) position_h = servo_h_LimitHigh;
  servo_h.write(position_h);

  // Seguimos con la logica del servo vertical (servo que gira el panel solar de arriba hacia abajo)
  
  if (average_top > average_bottom) position_v -= 20 ;
  else position_v += 20;

  if (position_v < servo_v_LimitLow) position_v = servo_v_LimitLow;
  if (position_v > servo_v_LimitHigh) position_v = servo_v_LimitHigh;
  servo_v.write(position_v);





  
  Serial.print("Valor analogico de LDR TL: ");
  Serial.print(TL);
  Serial.print("\n");
  

  Serial.print("Valor analogico de LDR TR: ");
  Serial.print(TR);
  Serial.print("\n");

  Serial.print("Valor analogico de LDR BL: ");
  Serial.print(BL);
  Serial.print("\n");

  Serial.print("Valor analogico de LDR BR: ");
  Serial.print(BR);
  Serial.print("\n");

 

  Serial.print("Left average: ");
  Serial.print(average_left);
  Serial.print("\n");

  Serial.print("Right average: ");
  Serial.print(average_right);
  Serial.print("\n");
  
  

  //delay(1000);
}






