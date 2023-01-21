#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define IDLE 0 //se encienden cuatro leds
#define START 1 //las leds parpadean por dos segundos
#define PLAYING 2 //se encienden dos leds
#define CHECK 3 //se enciende un led
#define RESET 4 //se encienden tres leds

int playing, state, button, turn, counter, seed_counter, indicador;
int user_input[14]; //= {};
int secuencia[14]; //= {1,2,1,2,1,2};

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
  DDRB = 0x0F; // Configuracion del puerto B, 0 es input y 1 es output
  GIMSK = 0xE8; // Habilitamos interrupciones INT0, INT1, PCIE0 y PCIE2
  //GIMSK |=(1 << PCIE); // Habilito solo las interrupciones de tipo PCIE
  PCMSK = 0b10000000; // Solo el pin con PCINT7 (PB7) puede disparar la interrupcion PCIE0. 
  PCMSK1 = 0b00000100; // Solo el pin con PCINT10 (PA2) puede disparar la interrupcion PCIE1. 
  MCUCR = 0x0A; // Cualquier cambio lógico (toggle) en INT0-1 generan una interrupción. 

  state = IDLE;
  playing = 0, button = 0, counter = 0, seed_counter = 0, indicador = 1; turn = 1; 

  sei(); // Habilitamos dentro de C las ISRs.
  srand(seed_counter); // Sirve para crear una semilla para los números random. 
  reset_arrays(); 
}

void blink(){
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
              PORTB = 0x00; 
              break;
            case 2:
              PORTB = 0b00000100;  _delay_ms(2000);
              PORTB = 0x00; 
              break;
            case 3:
              PORTB = 0b00000010;  _delay_ms(2000);
              PORTB = 0x00; 
              break;
            case 4:
              PORTB = 0b00000001;  _delay_ms(2000);
              PORTB = 0x00; 
              break;
            default:
              break;
          }
        }
}

void check(){
  for (int i = 0; i < turn; i++){
    if (secuencia[i] != user_input[i]){
      playing = 0;
      break;
    }
  }
}

/*
void led_on(){
  switch (user_input[counter]){
    case 1:
      PORTB = 0b00001000;  _delay_ms(200);
      PORTB = 0x00; 
      break;
    case 2:
      PORTB = 0b00000100;  _delay_ms(200);
      PORTB = 0x00; 
      break;
    case 3:
      PORTB = 0b00000010;  _delay_ms(200);
      PORTB = 0x00; 
      break;
    case 4:
      PORTB = 0b00000001;  _delay_ms(200);
      PORTB = 0x00; 
      break;
  default:
    break;
  }
}
*/

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
       //_delay_ms(5000); SIRVE POR MEDIO DEL DELAY. 

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
        button = 0, counter = 0, turn = 1, indicador = 1;
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


/*#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define IDLE 0 //se encienden cuatro leds
#define START 1 //las leds parpadean por dos segundos
#define PLAYING 2 //se encienden dos leds
#define CHECK 3 //se enciende un led
#define RESET 4 //se encienden tres leds

int playing, state, button, turn, counter, seed_counter;
int user_input[14]; // = {};
int secuencia[14]; //= {4,3,2,1,4,4};

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
  DDRB = 0x0F; // Configuracion del puerto B, 0 es input y 1 es output
  GIMSK = 0xE8; // Habilitamos interrupciones INT0, INT1, PCIE0 y PCIE2
  //GIMSK |=(1 << PCIE); // Habilito solo las interrupciones de tipo PCIE
  PCMSK = 0b10000000; // Solo el pin con PCINT7 (PB7) puede disparar la interrupcion PCIE0. 
  PCMSK1 = 0b00000100; // Solo el pin con PCINT10 (PA2) puede disparar la interrupcion PCIE1. 
  MCUCR = 0x0A; // Flanco negativo en INT0-1 generan una interrupción. 

  state = IDLE;
  playing = 0, button = 0, counter = 0, seed_counter = 0, turn = 1; 

  sei(); // Habilitamos dentro de C las ISRs.
  srand(seed_counter); // Sirve para crear una semilla para los números random. 
  reset_arrays(); 
}

void blink(){
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
              PORTB = 0x00; 
              break;
            case 2:
              PORTB = 0b00000100;  _delay_ms(2000);
              PORTB = 0x00; 
              break;
            case 3:
              PORTB = 0b00000010;  _delay_ms(2000);
              PORTB = 0x00; 
              break;
            case 4:
              PORTB = 0b00000001;  _delay_ms(2000);
              PORTB = 0x00; 
              break;
            default:
              break;
          }
        }
}

void check(){
  for (int i = 0; i < turn; i++){
    if (secuencia[i] != user_input[i]){
      playing = 0;
      break;
    }
  }
}

int main(){
  setup();
  while (1) {
    switch(state){
      case IDLE:
        if (button != 0){
          state = START;
          counter = 0, button = 0;
        }
        break;

      case START:
        blink();
        simon_blink();
        state = PLAYING;
        playing = 1;
        break;

      case PLAYING:
        if (button != 0) {
          user_input[counter] = button;
          counter++;
        }

        if (user_input[turn-1] != 0 ) state = CHECK;
        else {
          state = PLAYING;
          button = 0, seed_counter++;
        }
        break;
       //_delay_ms(500);  SIRVE POR MEDIO DEL DELAY. 

      case CHECK:
        check();
        switch(playing){
          case 0: 
            state = RESET; 
            break;

          case 1:
            state = PLAYING;
            counter = 0, button = 0;
            turn++;
            break;

          default:
            break;
        }
        break;
    
      case RESET:
        blink(); // Si el usuario perdió, vamos a RESET y encendemos LEDs 3 veces. 
        button = 0, counter = 0, turn = 1;
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

ISR(INT0_vect) // Pin PD2
{
  button = 1;

}  
  // PORTB = 0x0F; _delay_ms(2000);



ISR(INT1_vect) // Pin PD3
{
  button = 2;

}
 
  // PORTB = 0x00; _delay_ms(2000);


ISR(PCINT_B_vect) // Pin PB7
{
  button = 3;

}

ISR(PCINT_A_vect) // Pin PA2 
{
  button = 4;
 
}
*/