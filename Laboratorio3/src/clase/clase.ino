#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

//Configuracion de pines para el display
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

//Definicion de variables globales
float vA, vB, vC, vD = {0.00};
float vAk, vBk, vCk, vDk = {0.00};

float button, comms = {0.00};

float converting_factor = {9.6};
float threshold = {1.00};
bool etiqueta = true;

float corrienteA, corrienteB, corrienteC, corrienteD = {0.00};

//Funcion de Setup para los puertos y pantalla 
//Funcion de Setup para los puertos y pantalla 
void setup(){
  Serial.begin(9600); //Inicio del puerto Serial
  //Configira los pines como salida
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
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
void precaucion_AC(float vA, float vB, float vC, float vD){
  if(vA > 14.14) digitalWrite(8, HIGH); // encendemos LED de precaución del canal A
  else digitalWrite(8, LOW); // apagamos LED de precaución del canal A

  if(vB > 14.14) digitalWrite(9, HIGH); 
  else digitalWrite(9, LOW); 

  if(vC > 14.14) digitalWrite(10, HIGH);
  else digitalWrite(10, LOW);

  if(vD > 14.14) digitalWrite(11, HIGH);
  else digitalWrite(11, LOW); 
}


void precaucion_DC(float vA, float vB, float vC, float vD){
  if( vA > 20 || vA < -20) digitalWrite(8, HIGH);
  else digitalWrite(8, LOW); 

  if( vB > 20 || vB < -20) digitalWrite(9, HIGH);
  else digitalWrite(9, LOW); 

  if( vC > 20 || vC < -20) digitalWrite(10, HIGH);
  else digitalWrite(10, LOW); 

  if( vD > 20 || vD < -20) digitalWrite(11, HIGH);
  else digitalWrite(11, LOW); 
}


//----------------------------------------------- Loop principal -----------------------------------------------------------------------
void loop(){

  button = analogRead(A4); // Revisa si el button de modo (AC/DC) esta encendido
  comms = analogRead(A5); //Transmision


  if (comms > 3.00){
    while (etiqueta){
      Serial.println("Channel 1");
      Serial.println("Channel 2");
      Serial.println("Channel 3");
      Serial.println("Channel 4");
      Serial.println("AC/DC");
      etiqueta = false;  
    }
  }
  
  if (button > 3.00){ // Si el button esta encendido mida AC
    
    float VAC1 = get_max_vA();
    VAC1 = ((((VAC1*5)/1023)*converting_factor)-24);
   
    VAC1 /= sqrt(2); // Obtiene el valor RMS

    float VAC2 = get_max_vB();
    VAC2 = ((((VAC2*5)/1023)*converting_factor)-24);
 
    VAC2 /= sqrt(2); // Obtiene el valor RMS

    float VAC3 = get_max_vC();
    VAC3 = ((((VAC3*5)/1023)*converting_factor)-24);

    VAC3 /= sqrt(2); // Obtiene el valor RMS

    float VAC4 = get_max_vD();
    VAC4 = ((((VAC4*5)/1023)*converting_factor)-24);
    //if ((VAC4+0.65) == 12) VAC4 += 0.65;  
    VAC4 /= sqrt(2); // Obtiene el valor RMS

    if (comms > 3.00){
      Serial.println(VAC1);
      Serial.println(VAC2);
      Serial.println(VAC3);
      Serial.println(VAC4);
      Serial.println("AC");
    }

    //Pantalla
    
    display.print("Voltimetro AC \n");
    display.print("--------------");
    display.print("vA: ");
    display.print(VAC1);
    display.print(" Vrms");
    display.print("\n");
    display.print("vB: ");
    display.print(VAC2);
    display.print(" Vrms");
    display.print("\n");
    display.print("vC: ");
    display.print(VAC3);
    display.print(" Vrms");
    display.print("\n");
    display.print("vD: ");
    display.print(VAC4);
    display.print(" Vrms");
    display.print("\n");
    display.display();
    display.clearDisplay();

    precaucion_AC(VAC1, VAC2, VAC3, VAC4);

  } 
  else{ // si comms esta cerrado mide DC

    //Valores DC 
    vA = analogRead(A0);
    vB = analogRead(A1);
    vC = analogRead(A2);
    vD = analogRead(A3);


    vAk = (((vA*5)/1023)*converting_factor)-24;
    vBk = (((vB*5)/1023)*converting_factor)-24;
    vCk = (((vC*5)/1023)*converting_factor)-24;
    vDk = (((vD*5)/1023)*converting_factor)-24;

    if (comms > 3.00){
      Serial.println(vAk);
      Serial.println(vBk);
      Serial.println(vCk);
      Serial.println(vDk);
      Serial.println("DC");
    }
    //Pantalla 
  
    display.print("Voltimetro DC \n");
    display.print("--------------");
    display.print("VA: ");
    display.print(vAk);
    display.print(" V");
    display.print("\n");
    display.print("VB: ");
    display.print(vBk);
    display.print(" V");
    display.print("\n");
    display.print("VC: ");
    display.print(vCk);
    display.print(" V");
    display.print("\n");
    display.print("VD: ");
    display.print(vDk);
    display.print(" V");
    display.print("\n");
    display.display();
    display.clearDisplay();

    precaucion_DC(vAk, vBk, vCk, vDk);
  }
}
