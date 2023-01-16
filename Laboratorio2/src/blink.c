//Ana Eugenia Sanchez y Adrian Montero
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Estados para el boton (No tienen nada que ver con la FSM)
//#define ON  1
//#define OFF 0
//Esto es solo una idea 

//Estados de niveles
#define inicio 0 //las leds parpadean por dos segundos
#define nivel1 //se enciende un led
#define nivel2 //se encienden dos leds
#define nivel3 //se encienden tres leds
#define nivel4 //se encienden cuatro leds
#define nivel5 //se encienden cinco leds
#define nivel6 //se encienden seis leds
#define nivel7 //se encienden siete leds
#define nivel8 //se encienden ocho leds
#define nivel9 //se encienden nueve leds
#define nivel10 //se encienden diez leds 
#define nivel11 //se encienden once leds
#define nivel12 //se encienden doce leds
#define nivel13 //se encienden trece leds
#define nivel14 //se encienden catorce leds
 
// Variables globales
int estado;

int main(void){
  setup_boton();
  timer_setup();
  estado = inicio;  // Estado inicial de la maquina, carros avanzando
  while (1) {
    engine_fsm();
  }
}
