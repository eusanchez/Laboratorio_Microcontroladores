#include <LSM9DS1.h>


void setup() {
  // Comenzamos comunicaciones seriales en 9600 baudios
  Serial.begin(9600);
  // Esperamos hasta que el 9-axis IMU inicialice y una vez ocurra, pasamos al loop()
  while (!Serial);
  if (!IMU.begin()) {
		while (1);
  }

}

void loop() {
	float aX, aY, aZ, gX, gY, gZ;
	unsigned long timestamp = millis();
 
  // Leemos los 3 ejes del giroscopio y acelerómetro y los mandamos por el puerto serial
  // para ser capturados por el script 'axis.py'.  
	if (IMU.gyroscopeAvailable() && IMU.accelerationAvailable() ) {
		IMU.readAcceleration(aX, aY, aZ);
    IMU.readGyroscope(gX, gY, gZ);
      
    Serial.print('\t');    
		Serial.print(aX);
		Serial.print('\t');
		Serial.print(aY);
		Serial.print('\t');
		Serial.print(aZ);
		Serial.print('\t');    
		Serial.print(gX);
		Serial.print('\t');
		Serial.print(gY);
		Serial.print('\t');
		Serial.println(gZ);
	}
}
