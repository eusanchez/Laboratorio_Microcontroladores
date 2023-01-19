#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define IDLE 0 //las leds parpadean por dos segundos
#define nivel1 1 //se enciende un led
#define nivel2 2 //se encienden dos leds
#define nivel3 3 //se encienden tres leds
#define nivel4 4 //se encienden cuatro leds


int playing, state, counter = 0;
int secuencia[14] = {};



int main(void)
{
  setup();
  while (1) {

    switch(state){
      case IDLE:


    }
  }
  return 0;
}






void setup(){
  DDRB = 0x08; //Configuracion del puerto
  GIMSK |=(1 << PCIE); //Habilito solo las interrupciones de tipo PCIE
  PCMSK = 0b10000000; //Solo el pin con PCINT7 (PB7) puede disparar la interrupcion PCIE0. 
  sei();
}

ISR(PCINT_B_vect)
{
  if (!playing){
    playing = ~playing;
    PORTB = 0x08; _delay_ms(1000);
    PORTB = 0x00; _delay_ms(1000);
    PORTB = 0x08; _delay_ms(1000);
    PORTB = 0x00; _delay_ms(1000);
  }
}