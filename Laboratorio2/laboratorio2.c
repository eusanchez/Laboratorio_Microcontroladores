#include <avr/io.h>
#include <avr/interrupt.h>
#include<util/delay.h>

#define IDLE 0 //se encienden cuatro leds
#define START 1 //las leds parpadean por dos segundos
#define PLAYING 2 //se encienden dos leds
#define CHECK 3 //se enciende un led
#define RESET 4 //se encienden tres leds

int playing, state, button, turn, counter, seed_counter, indicador, units_counter, time, enable;
int user_input[14]; 
int secuencia[14]; 

void reset_arrays(){
    int lb = 1, ub = 4;
    int random = (rand() % (ub - lb + 1)) + lb;

    for(int i = 0; i < 14; i++){
        user_input[i] = 0;
        secuencia[i] = random;
        random = (rand() % (ub - lb + 1)) + lb;
  }
}

void setup(){
  // Puertos e interrupciones generales y exteriores
  DDRB = 0x0F; // Configuracion del puerto B, 0 es input y 1 es output
  GIMSK = 0xE8; // Habilitamos interrupciones INT0, INT1, PCIE0 y PCIE2
  PCMSK = 0b10000000; // Solo el pin con PCINT7 (PB7) puede disparar la interrupcion PCIE0. 
  PCMSK1 = 0b00000100; // Solo el pin con PCINT10 (PA2) puede disparar la interrupcion PCIE1. 
  MCUCR = 0x0A; // Cualquier cambio lógico (toggle) en INT0-1 generan una interrupción. 

  // Config de los timers internos
  TCCR0A = 0x00; // Lo usamos en modo normal 
  TCCR0B = 0b011; //prescaling con 64
  TCNT0 = 0x00; // Inicializamos registro del contador interno del Timer0. 

  state = IDLE;
  playing = 0, button = 0, counter = 0, seed_counter = 0, units_counter = 0, enable = 0;
  indicador = 1, turn = 1, time = 977; 

  sei(); // Habilitamos dentro de C las ISRs.
  srand(seed_counter); // Sirve para crear una semilla para los números random. 
  reset_arrays(); 
}

void delay(int overflows){
  TIMSK = 0b10;
  enable = 1;
  units_counter = 0;
  TCNT0 = 0x00;
  while(units_counter < overflows){
    PORTB &= 0xFF;
  }
  TIMSK = 0b00;
  enable = 0;
}

void blink(){
  if (state == START){
    PORTB = 0x0F; delay(250);
    PORTB = 0x00; delay(250);
    PORTB = 0x0F; delay(250);
    PORTB = 0x00; delay(250);
  }
  else{
    PORTB = 0x0F; delay(250);
    PORTB = 0x00; delay(250);
    PORTB = 0x0F; delay(250);
    PORTB = 0x00; delay(250);
    PORTB = 0x0F; delay(250);
    PORTB = 0x00; delay(250);
  }
  
}

void simon_blink(){
  for(int i = 0; i < turn; i++){
    switch(secuencia[i]){
      case 1:
        PORTB = 0b00001000; delay(time);
        PORTB = 0x00; delay(244);
        break;

      case 2:
        PORTB = 0b00000100; delay(time);
        PORTB = 0x00; delay(244);
        break;

      case 3:
        PORTB = 0b00000010; delay(time);
        PORTB = 0x00; delay(244);
        break;

      case 4:
        PORTB = 0b00000001; delay(time);
        PORTB = 0x00; delay(244);
        break;

      default:
        break;
    }
  
  }
  time-=98;
}

void check(){
  for (int i = 0; i < turn; i++){
    if (secuencia[i] != user_input[i]){
      playing = 0;
      break;
    }
  }
}

int main(void){
  setup();
  while (1) {
    switch(state){
      case IDLE:
        if (button != 0){
          state = START;
          button = 0;         
        }
        break;

      case START:
        blink();
        state = PLAYING;
        break;

      case PLAYING:
        counter = 0, indicador = 1, playing = 1;
        simon_blink();
        state = CHECK;        
        break;

      case CHECK:
        if (counter == turn){
          check();
          switch(playing){
            case 0: 
              state = RESET; 
              break;

            case 1:
              state = PLAYING;
              counter = 0;
              turn++;
              break;

            default:
              break;
          }
        }
        else{
          state = CHECK;
          seed_counter++; // Siempre sumamos
        }
        break;
    
      case RESET:
        blink(); // Si el usuario perdió, vamos a RESET y encendemos LEDs 3 veces. 
        button = 0, counter = 0, turn = 1, indicador = 1, time = 977;
        state = IDLE;
        reset_arrays();
        srand(seed_counter);
        break;

      default:
        break;
    }
  }
  return 0;
}

ISR(TIMER0_OVF_vect)
{ 
  if (enable) units_counter++;
}

ISR(INT0_vect) // Pin PD2
{
  button = 1;
  if (playing){
    user_input[counter] = button;
    counter++;
  }  
  // PORTB = 0x0F; _delay_ms(2000);

}

ISR(INT1_vect) // Pin PD3
{
  button = 2;
  if (playing){
    user_input[counter] = button;
    counter++;
  }
 
  // PORTB = 0x00; _delay_ms(2000);

}

ISR(PCINT_B_vect) // Pin PB7
{
  button = 3;
  indicador = ~indicador;
  /*
  La variable indicador nos sirve para simular el comportamiento de una
  interrpución activada por flanco negativo, a como pasa con las interrupciones
  INT0 e INT1
  */

  if (playing == 1 && indicador == 1){
    user_input[counter] = button;
    counter++;
  }
}

ISR(PCINT_A_vect) // Pin PA2 
{
  button = 4;
  indicador = ~indicador;

  if (playing == 1 && indicador == 1){
    user_input[counter] = button;
    counter++;
  }
}