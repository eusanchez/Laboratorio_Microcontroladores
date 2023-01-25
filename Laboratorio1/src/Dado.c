//Ana Eugenia Sanchez Villalobos
#include <pic14/pic12f683.h>
typedef unsigned int word;
word __at 0x2007 __CONFIG = (_BOREN_OFF & _WDT_OFF);
 
void delay (unsigned int tiempo);
int contador = 0;
 
void main(){

    ANSEL = 0; // 
    CMCON0 = 7;
    TRISIO=0b00100000;    //GP5 as input  
    GPIO = 0; //Poner pines en abajo

	//Loop forever
	while(1)
	{
		contador = contador + 1;
		if (GP5 == 1){
			if(contador==1){
				GP0=1;
				delay(500);	
				GP0=0;
				delay(500);	
				contador=0;
				continue;
			}
			if(contador==2){
				GP1=1;
				delay(500);
				GP1=0;
				delay(500);	
				contador=0;
				continue;
			}
			if(contador==3){
				GP0=1;
				GP1=1;
				delay(500);
				GP0=0;
				GP1=0;
				delay(500);	
				contador=0;
				continue;
			}
			if(contador==4)
			{
				GP1=1;
				GP2=1;
				delay(500);
				GP1=0;
				GP2=0;
				delay(500);	
				contador=0;
				continue;
			}
			if(contador==5)
			{
				GP0=1;
				GP1=1;
				GP2=1;
				delay(500);
				GP0=0;
				GP1=0;
				GP2=0;
				delay(500);	
				contador=0;
				continue;
			}
			if(contador==6)
			{
				GP4=1;
				GP1=1;
				GP2=1;
				delay(500);
				GP4=0;
				GP1=0;
				GP2=0;
				delay(500);	
				contador=0;
				continue;
			}
		}
		if (contador == 6){
			contador=0;
			continue;
		}
	}
}

void delay(unsigned int tiempo)
{
	unsigned int i;
	unsigned int j;

	for(i=0;i<tiempo;i++)
	  for(j=0;j<1275;j++);
}
