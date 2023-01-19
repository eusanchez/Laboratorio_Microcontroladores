//Ana Eugenia Sanchez y Adrian Montero
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//Estados de niveles
#define inicio 0 //las leds parpadean por dos segundos
#define nivel1 1 //se enciende un led
#define nivel2 2 //se encienden dos leds
#define nivel3 3 //se encienden tres leds
#define nivel4 4 //se encienden cuatro leds
#define nivel5 5 //se encienden cinco leds
#define nivel6 6 //se encienden seis leds
#define nivel7 7 //se encienden siete leds
#define nivel8 8 //se encienden ocho leds
#define nivel9 9 //se encienden nueve leds
#define nivel10 //se encienden diez leds 
#define nivel11 //se encienden once leds
#define nivel12 //se encienden doce leds
#define nivel13 //se encienden trece leds
#define nivel14 //se encienden catorce leds
 
// Variables globales
int estado;
int boton = 0;

void timer_setup(){ //Configuracion del timer
  TCCR0A=0x00;   //Se usa el modo normal de operacion del timer
  TCCR0B=0x00;
  TCCR0B |= (1<<CS00)|(1<<CS02);   //prescaling usando el 1024
  
  TCNT0=0;
  TIMSK|=(1<<TOIE0); //habilitando la interrupcion en TOIE0
}

void setup_boton(){//Configuracion de los puertos del mcu
  DDRB = (1<<DDB4)|(1<<DDB3)|(1<<DDB2)|(1<<DDB1)|(1<<DDB0); // Configuracion de los puertos LED
  GIMSK |= (1<<INT1);     // Habilitando en INT1 (external interrupt) PD3 = INT1
  PORTB &= (0<<PB0)|(0<<PB1)|(0<<PB2)|(0<<PB3)|(0<<PB4); // Iniciar con todos los pines en cero. 
  sei();
}

//Maquina de estados
void fsm(){
  switch (estado){
    case inicio:
      PORTB = 0x00; _delay_ms (500); 
      PORTB = 0x08; _delay_ms (500);
        if(boton == 1) {
          estado = nivel1;
        }
        else {
          estado = inicio;
        }
        break;

    case nivel1:
      PORTB = 0x00; _delay_ms (100000); 
      break;
  }
}

// Interrupt service routine
ISR (INT1_vect){        // Interrupt service routine     
  boton = 1;
}


int main(void){
  DDRB = 0x00; //configuracion de puerto
  DDRD = 0x01;
  setup_boton();
  //timer_setup();
  estado  = inicio;  // Estado inicial de la maquina, carros avanzando
  while (1) {
    fsm();
  }
}