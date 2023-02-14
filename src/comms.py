#!/usr/bin/env python3
import serial
import time
import csv

ser = serial.Serial(port = '/dev/ttyACM0', baudrate=115200, timeout=1) 
print("Connected")
data_rows = []
encabezado = ['x', 'y', 'z']
print(encabezado)
print(ser)
filename= open("Datos.csv",'w') 
escritura_archivo = csv.writer(filename)

while(1):
    data = ser.readline().decode('latin-1').replace('\r', "").replace('\n', "")
    data = data.split('\t')

    if len(data) == 3:
        print(data)
        time.sleep(0.5)
        escritura_archivo.writerow(data)
