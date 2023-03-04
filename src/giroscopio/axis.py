import serial
import csv
import os

#Se define donde esta conectado el arduino
puerto = "/dev/ttyACM0"
ser = serial.Serial(port = puerto,baudrate=9600,parity=serial.PARITY_NONE,stopbits=serial.STOPBITS_ONE,bytesize=serial.EIGHTBITS) 

current_directory = os.getcwd()

#Se denomina el nombre del archivo, como queremos que se llame
csv_path = os.path.join(current_directory, "izq_der.csv")


with open(csv_path, mode ='w') as csv_file:
  writer = csv.writer(csv_file)
  writer.writerow(['gX', 'gY', 'gZ']) #Para poder identificar los datos
  muestras = 100 #suficientes para 10 segundos, si no sigue en un bucle.
  cuenta = 0
  while True:
    line = ser.readline().decode().strip()
    data = line.split('\t')
    aX = float(data[0])
    aY = float(data[1])
    aZ = float(data[2])
    gX = float(data[3])
    gY = float(data[4])
    gZ = float(data[5])
    writer.writerow([gX, gY, gZ])
    cuenta += 1
    if cuenta >= muestras:
      break
