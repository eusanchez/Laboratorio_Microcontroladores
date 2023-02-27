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
int position_v = 0; 
int servo_v_LimitLow = 0;
int servo_v_LimitHigh = 180;

int speed = 2; // Velocidad de operacion de los servos, conviene dejarlo entre 5-10
int tolerance = 25; // Intervalo decentre entre 50-100



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


void loop(){ 
  
  int TL = analogRead(A1); // top left
  int TR = analogRead(A2); // top right
  int BL = analogRead(A3); // bottom left
  int BR = analogRead(A4); // bottom right

  // Calculamos los promedios de los lados (arriba, abajo, izquierda y derecha)
  int average_top = (TL + TR) / 2;
  int average_bottom = (BL + BR) / 2;
  int average_left = (TL + BL) / 2;
  int average_right = (TR + BR) / 2;

  // Diferencias entre las 4 direcciones para comparar con la tolerancia escogida
  int diff_horizontal = abs(average_left - average_right);
  int diff_vertical = abs(average_top - average_bottom);


  // Comenzamos a comparar los promedios de luz que se reciben de los LDRs


  // Primeros controlamos el servo horizontal (servo ubicado en la base)
  if (diff_horizontal > tolerance) {
    if (average_left > average_right) position_h += speed ;
    else position_h -= speed;
  }

  if (position_h < servo_h_LimitLow) position_h = servo_h_LimitLow;
  if (position_h > servo_h_LimitHigh) position_h = servo_h_LimitHigh;
  servo_h.write(position_h);

  // Seguimos con la logica del servo vertical (servo que gira el panel solar de arriba hacia abajo)
  
  if (diff_vertical > tolerance) {
    if (average_top > average_bottom) position_v -= speed ;
    else position_v += speed;
  }

  if (position_v < servo_v_LimitLow) position_v = servo_v_LimitLow;
  if (position_v > servo_v_LimitHigh) position_v = servo_v_LimitHigh;
  servo_v.write(position_v);

  
  
  // Leemos el puerto A5, procesamos senal analogica y la pasamos a trave sdel puerto serial
  float solar_out = (analogRead(A5) *5 ) / 1023; // Pasamos lectura de 0-1023 a 0-5V

  Serial.print("Vout: ");
  Serial.print(solar_out);
  Serial.print("\n");
  
  delay(25);
  
}






