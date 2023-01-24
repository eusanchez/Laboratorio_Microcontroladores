//Ana Eugenia Sanchez y Adrian Montero
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Estados para el boton (No tienen nada que ver con la FSM)
#define ON  1
#define OFF 0

//Estados finitos para la maquina
#define CO 0 //comienzo
#define ST 1 //START
#define SB 2 //SIMON BLINK
#define UR 3 //USER RESPONSE

// Variables globales
int intr_count = 0;
int sec = 0;
int msec = 0;
int boton = 0;
int estado;
int turn = 0;
int secuencia[14]; //= {4,3,2,1,4,4};

//Funciones de estado 
void Comienzo(void);
void START(void);
void SIMON_BLINK(void);
void USER_RESPONSE(void);

int estado;

//Estructura FSM
struct FSM{
    void (*stateptr)(void);
    unsigned char time; 
    unsigned char next[2];
};
typedef struct FSM blink;

//Definicion FSM con sus repectivos tiempos y estado siguiente
blink fsm[3] = {
    {&Comienzo,10,{CO,ST}},
    {&START,10,{ST,SB}},
    {&SIMON_BLINK,10,{SB,UR}}
};

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


//Definicion de funciones de estado
void Comienzo(void){
  PORTB = (1<<PB0)|(1<<PB1)|(0<<PB2)|(0<<PB3); //TODO APAGADO
}
void START(void){
  PORTB ^= (1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB3);
}
void SIMON_BLINK(void){
  simon_blink();
}
void USER_RESPONSE(void){
  PORTB = (0<<PB0)|(0<<PB1)|(0<<PB2)|(0<<PB3); //TODO APAGADO
}

// Funciones miscelaneas
void timer_setup(){ //Funcion de configuracion del timer
  TCCR0A=0x00;   //Se usa el modo normal de operacion del timer
  TCCR0B=0x00;
  TCCR0B |= (1<<CS00)|(1<<CS02);   //prescaling usando el 1024
  sei();
  TCNT0=0;
  TIMSK|=(1<<TOIE0); //habilitando la interrupcion en TOIE0
}

void setup_boton(){// funcion de configuracion de los puertos del mcu
  DDRB = (1<<DDB3)|(1<<DDB2)|(1<<DDB1)|(1<<DDB0); // configuracion de los puertos de salida (LED)
  GIMSK |= (1<<INT1);     // habilitando en INT1 (external interrupt) 
  MCUCR |= (1<<ISC11);    // se configura con flanco negativo del reloj
  PORTB &= (0<<PB0)|(0<<PB1)|(0<<PB2)|(0<<PB3); // Como buena practica es necesario iniciar con todos los pines en cero. 
}


//Motor de la maquina de estados
void engine_fsm(){
  switch (estado){
    case CO:
      (fsm[estado].stateptr)(); 
      if (boton >= fsm[estado].time){
        estado = fsm[estado].next[boton]; // si hay boton y ademan pasaron 10 segundos, pase de estado
        intr_count = 0;
        sec = 0;
        msec =0;
      }
      else{
        estado = CO; // si no hay bonton quedese en el estado de carros avanzando
      }
      break;
    
    case ST:
      if (msec == fsm[estado].time){ // despues del tiempo definido pase de estado
        estado = fsm[estado].next[boton];
        intr_count = 0;
        sec = 0;
        msec =0;
      }
      else{
        estado = CO; // si no se cumple la condicion de tiempo siga parpadeando
      }
      break;

    default:
      break;
  }
}


// Interrupt service routine
ISR (INT1_vect){        // Interrupt service routine     
  boton = ON;
}

ISR (TIMER0_OVF_vect){      //Interrupt vector for Timer0
  if (intr_count == 20){  //cuenta un tercio de segundo 
    if(estado == ST){ 
      (fsm[ST].stateptr)(); // parpadear
    }
    ++msec; //contador de tercios de segundo
  }
  if (intr_count == 60){    // cuenta un segundo 
    intr_count = 0;
    ++sec; // cuenta un segundo 
  }
  else intr_count++;
}

int main(void){
  setup_boton();
  timer_setup();
  estado = CO;  // Estado inicial de la maquina, carros avanzando
  while (1) {
    engine_fsm();
  }
}
