#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#define START 5 //las leds parpadean por dos segundos
#define BUTTON_PRESSED 1 //se enciende un led
#define PLAYING 2 //se encienden dos leds
#define RESET 3 //se encienden tres leds
#define IDLE 4 //se encienden cuatro leds


int playing, state, button, counter = 0;
int turn = 1;
int user_input[14] = {};
int secuencia[14] = {4,3,2,1,2};

void fill_rands(int max){
    int lb = 1, ub = 4;
    int random = (rand() % (ub - lb + 1)) + lb;
    // random = randomRange(1, 4, random);
    for(int i = 0; i < max; i++){
        secuencia[i] = random;
        random = (rand() % (ub - lb + 1)) + lb;
        //random = randomRange(1, 4, random);
  }
}

void setup(){
  DDRB = 0x0F; // Configuracion del puerto B, 0 es input y 1 es output
  GIMSK = 0xE8; // Habilitamos interrupciones INT0, INT1, PCIE0 y PCIE2
  //GIMSK |=(1 << PCIE); // Habilito solo las interrupciones de tipo PCIE

  PCMSK = 0b10000000; // Solo el pin con PCINT7 (PB7) puede disparar la interrupcion PCIE0. 
  PCMSK1 = 0b00000100; // Solo el pin con PCINT10 (PA2) puede disparar la interrupcion PCIE1. 
  MCUCR = 0x05; // Cualquier cambio lógico (toggle) en INT0-1 generan una interrupción. 
  state = IDLE;
  sei();
  srand(8);
  // fill_rands(14);
}

void initial_blink(){
  if (state == START){
    PORTB = 0x0F;  _delay_ms(500);
    PORTB = 0x00; _delay_ms(500);
    PORTB = 0x0F; _delay_ms(500);
    PORTB = 0x00; _delay_ms(500);
  }
  else{
    PORTB = 0x0F;  _delay_ms(500);
    PORTB = 0x00; _delay_ms(500);
    PORTB = 0x0F; _delay_ms(500);
    PORTB = 0x00; _delay_ms(500);
    PORTB = 0x0F; _delay_ms(500);
    PORTB = 0x00; _delay_ms(500);
  }
  
}

void simon_blink(){
   for(int i = 0; i < turn; i++){
          switch(secuencia[i]){
            case 1:
              PORTB = 0b00001000;  _delay_ms(2000);
              PORTB = 0x00; _delay_ms(2000);
              break;
            case 2:
              PORTB = 0b00000100;  _delay_ms(2000);
              PORTB = 0x00; _delay_ms(2000);
              break;
            case 3:
              PORTB = 0b00000010;  _delay_ms(2000);
              PORTB = 0x00; _delay_ms(2000);
              break;
            case 4:
              PORTB = 0b00000001;  _delay_ms(2000);
              PORTB = 0x00; _delay_ms(2000);
              break;
          }
        }
}

void check(){
  for (int i = 0; i < turn; i++){
    if (secuencia[i] != user_input[i]){
      state = IDLE; 
      initial_blink();
      playing = 0;
      button = 0;
      break;
    }
  }
  counter = 0;
}

int main(void)
{
  setup();

  while (1) {

    switch(state){
      case IDLE:
        if (button != 0) state = START;
        else break;

      case START:
        initial_blink();
        state = PLAYING;
        break;

      case PLAYING:
        playing = 1;
        simon_blink();
        _delay_ms(5000);
        check();
        switch(playing){
          case 0:
            break;
          case 1:
            state = PLAYING;
            turn++;
            break;
        }
    }
  }

  return 0;
}

ISR(INT0_vect) // Pin PD2
{
  button = 1;
  if (playing){
    user_input[turn-1] = button;
 
  }
  
  // PORTB = 0x0F; _delay_ms(2000);

}

ISR(INT1_vect) // Pin PD3
{
  button = 2;
  if (playing){
    user_input[turn-1] = button;
  
  }
 
  // PORTB = 0x00; _delay_ms(2000);

}

ISR(PCINT_B_vect) // Pin PB7
{
  button = 3;
  if (playing){
    user_input[turn-1] = button;
  
  }
 
}

ISR(PCINT_A_vect) // Pin PA2 
{
  button = 4;
  if (playing){
    user_input[turn-1] = button;
  
  }
  
}