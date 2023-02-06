#!/usr/bin/python
import serial #Paquete de comunicacion arduino / python

baud = 9600 #sample rate
filename = "datos.csv" #archivo csv
samples = 29 

file = open(filename, 'w') 
file.close()
print("Se lee el archivo correctamente")


ser = serial.Serial("/tmp/ttyS1", baud) #Abre la comunicacion con el arduino usando el puerto ttyS1
print("Se conecto correctamente")
file = open(filename, 'w')
print("Se creo el archivo csv correctamente")

contador  = 0

while (1):
    getData = str(ser.readline())
    data = getData[2:][:-5] 
    print(data)

    if contador == 4:
        file = open(filename, "a")
        file.write(data + "\n")
        contador = 0
    else:
        file.write(data + ",")
        contador+=1
