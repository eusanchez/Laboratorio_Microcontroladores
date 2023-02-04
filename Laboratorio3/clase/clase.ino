#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

//Configuracion de pines para el display
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

//Definicion de variables globales
float VA, VB, VC, VD = {0.00};
float vAk, vBk, vCk, vDk = {0.00};

float button, comms = {0.00};

float converting_factor = {4.8};
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

//------------------------------------------- Funciones para calcular amplitud de señales AC ---------------------------------------\\
// REVISAR PUERTOS PARA PROBAR FUNCIONALIDAD!!!!!!!!
// Conseguimos amplitud de señal AC del canal A.
float get_max_vA() {
  float max_v = 0.00;
  for(int i = 0; i < 100; i++) {
    float r = analogRead(A0);  
    if(max_v < r) max_v = r;
    delayMicroseconds(200);
  }
  return max_v;
}

// Conseguimos amplitud de señal AC del canal B.
float get_max_vB() {
  float max_v = 0.00;
  for(int i = 0; i < 100; i++) {
    float r = analogRead(A1);  
    if(max_v < r) max_v = r;
    delayMicroseconds(200);
  }
  return max_v;
}

float get_max_vC() {
  float max_v = 0.00;
  for(int i = 0; i < 100; i++) {
    float r = analogRead(A2);  
    if(max_v < r) max_v = r;
    delayMicroseconds(200);
  }
  return max_v;
}

float get_max_vD() {
  float max_v = 0.00;
  for(int i = 0; i < 100; i++) {
    float r = analogRead(A3);  
    if(max_v < r) max_v = r;
    delayMicroseconds(200);
  }
  return max_v;
}

//------------------------------------------- Luces LEDs de precaucion para los canales A a D ---------------------------------------\\
// Aqui solo se revisa el valor RMS maximo, el cual es 20/sqrt(2) = 14.14
void precaucion_AC(float vA){
  if(vA > 14.14) digitalWrite(9, HIGH); // encendemos LED de precaución del canal A
  else digitalWrite(9, LOW); // apagamos LED de precaución del canal A

  if(vB > 14.14) digitalWrite(10, HIGH); 
  else digitalWrite(9, LOW); 

  if(vC > 14.14) digitalWrite(11, HIGH);
  else digitalWrite(9, LOW);

  if(vD > 14.14) digitalWrite(12, HIGH);
  else digitalWrite(9, LOW); 
}


void precaucion_DC(float vA){
  if( vA > 20 || vA < -20) digitalWrite(9, HIGH);
  else digitalWrite(10, LOW); 

  if( vB > 20 || vB < -20) digitalWrite(10, HIGH);
  else digitalWrite(10, LOW); 

  if( vC > 20 || vC < -20) digitalWrite(11, HIGH);
  else digitalWrite(10, LOW); 

  if( vD > 20 || vD < -20) digitalWrite(12, HIGH);
  else digitalWrite(10, LOW); 
}


//----------------------------------------------- Loop principal -----------------------------------------------------------------------
void loop(){

  button = analogRead(A4); // Revisa si el button de modo (AC/DC) esta encendido
  comms = analogRead(A5); //Transmision


  if (comms > 3.00){
    while (etiqueta){
      Serial.println("Canal 1");
      Serial.println("AC/DC");
      etiqueta = false;  
    }
  }
  
  if (button > 3.00){ // Si el button esta encendido mida AC
    
    float VAC1 = get_max_vA();
    VAC1 = ((((VAC1*5)/1023)*converting_factor)-24);
    if ((VAC1+0.65) == 12) VAC1 += 0.65;  
    VAC1 /= sqrt(2); // Obtiene el valor RMS

    if (comms > 3.00){
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
    
  } else{ // si comms esta cerrado mide DC

    //Valores DC 
    vA = analogRead(A3);
    vAk = (((vA*5)/1023)*converting_factor)-24;

    if (comms > 3.00){
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

