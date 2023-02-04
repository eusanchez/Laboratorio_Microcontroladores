#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

//Configuracion de pines para el display
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

//Definicion de variables globales
float VA= {0.00};
float vAk = {0.00};
float boton = {0.00};
float interruptor = {0.00};
float k1= {4.8};
float threshold = {1.00};
bool etiqueta = true;

float corrienteA = {0.00};


//Funcion de Setup para los puertos y pantalla 
void setup(){
  Serial.begin(9600); //Inicio del puerto Serial
  //Configira los pines como salida
  pinMode(9,OUTPUT);
  //Inicia la pantalla y se selecciona el contraste
  display.begin();
  display.setContrast(75);
  
  //Muestra la flor de Adafruit
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setTextColor(BLACK);
  display.setCursor(0,1);
  display.setTextSize(1.5);
  display.println("VOLTIMETRO AC");
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(22,20);
  display.println("|INICIO|");
  display.display();
  delay(2000);
  display.clearDisplay();   //Limpia el display

}

//Funcion encargada de encender los leds de emergencia 
  
//Como la funcion AC devuelve valores RMS es importante hacer una funcion de alarma con los valores RMS
void precaucion_AC(float vA){
  if((vA >= 16.9)){ //23.9/sqrt(2) = 16.9
      digitalWrite(9, HIGH);
    }
  else{
      digitalWrite(9, LOW); //led apagado
    }   }

//------------------------------------------- Funciones para el calculo del valor maximo en modo AC ---------------------------------------\\
//Obtiene el valor de la amplitud de la onda para el primer puerto
float get_max_vA() {
  float max_v = 0.00;
  for(int i = 0; i < 100; i++) {
    float r = analogRead(A3);  
    if(max_v < r) max_v = r;
    delayMicroseconds(200);
  }
  return max_v;
}

void loop(){

  boton = analogRead(A4); // Revisa si el boton de modo (AC/DC) esta encendido
  interruptor = analogRead(A5); //Transmision


  if (interruptor > 3.00){
    while (etiqueta){
      Serial.println("Canal 1");
      Serial.println("AC/DC");
      etiqueta = false;  
    }
  }
  
  if (boton > 3.00){ // Si el boton esta encendido mida AC
    
    float VAC1 = get_max_vA();
    VAC1 = ((((VAC1*5)/1023)*k1)-12);
    if ((VAC1+0.65) == 12) VAC1 += 0.65;  
    VAC1 /= sqrt(2); // Obtiene el valor RMS

    if (interruptor > 3.00){
      if ((corrienteA >= VAC1+threshold || corrienteA <= VAC1-threshold)){
        Serial.println(VAC1);
        Serial.println("AC");
        corrienteA = VAC1;
      }
    }

    //Pantalla
    
    display.print("Voltimetro AC \n");
    display.print("--------------");
    display.print("vA: ", VAC1, " Vrms\n");
    display.display();
    display.clearDisplay();

    precaucion_AC(VAC1);
    
  } else{ // si switch esta cerrado mide DC

    //Valores DC 
    vA = analogRead(A3);


     //Transformar el rango de 0 a 5 (rango aceptado por Arduino) a 0 a 24 (rango requerido)
    vAk = (((vA*5)/1023)*k1)-12;

    if (interruptor > 3.00){
      if (corrienteA >= vAk+threshold || corrienteA <= vAk-threshold){
        Serial.println(vAk);
        Serial.println("DC");
        corrienteA = vAk;
      }
    }
    //Pantalla 
  
    display.print("Voltimetro DC \n");
    display.print("--------------");
    display.print("vA: ");
    display.print(vAk,"V\n");
    display.display();
    display.clearDisplay();

    precaucion_DC(vAk);
  }
}

