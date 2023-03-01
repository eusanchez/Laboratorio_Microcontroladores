#!/usr/bin/env python3
import serial
import time
import csv

# Se conecta Python al puerto serial /dev/ttyACM0 y este recibe los datos enviados
# por el Arduino UNO. 
ser = serial.Serial(port = '/dev/ttyACM0', baudrate=115200 , timeout=1) 
print("Connected")
header = ['Vout']
print(header)

filename = open("Datos.csv",'w') # Creamos un archivo .csv en donde se escriben las lecturas.
file_write = csv.writer(filename)

while(1):
    # Formateamos las lineas de datos para ser procesadas y agregadas al .csv 
    data = ser.readline().decode('latin-1').replace('\r', "").replace('\n', "")
    data = data.split('\t')
    

    if len(data) == 1:
        print(data)
        time.sleep(0.1)
        file_write.writerow(data)
        filename.flush()