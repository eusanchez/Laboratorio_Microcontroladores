"""
El codigo realizado fue a base de la informacion obtenida de la pagina : http://www.steves-internet-guide.com/thingsboard-mqtt-dashboard/
Para comunicacion por MQTT para dashboard, en este caso ThingsBoard
"""

#Librerias
import csv
import serial
import paho.mqtt.client as mqtt
import time,json
import time

#Variable global
count=0

#FUNCIONES

def on_log(client, userdata, level, buf):
   print(buf) 

#Funcion on_connect verifica el estado de conexion si esta pudo ser realizada correctamente
# de no ser asi detiene la conexion.

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        client.connected_flag=True 
        print("conexion OK") 

    else:

        print("La conexion no fue realizada correctamente",rc) 
        client.loop_stop()  

#Funcion de desconexion del ciente, imprime si fue realizada correctamente

def on_disconnect(client, userdata, rc): 
   print("Desconexión de cliente realizada correctamente") 


def on_publish(client, userdata, mid):
    print("In on_pub callback mid= "  ,mid)

#TECNOLOGIA MQTT

#Revision si se realizada la conexion de cliente por tec MQTT correctamente

mqtt.Client.connected_flag=False 

mqtt.Client.suppress_puback_flag=False

#Creacion de instancia 

client = mqtt.Client("python1")              

#Conexion 

client.on_connect = on_connect 

#Desconexion

client.on_disconnect = on_disconnect 

#Publicacion

client.on_publish = on_publish 

#Broker de la escuela de electrica

broker="iot.eie.ucr.ac.cr"

#Puerto

port =1883

#Topic

topic="v1/devices/me/telemetry"

#Token de usuario donde se establece la conexion

usuario="H0BEo1567kp50xpIkme9"
password=""

if usuario !="":
   pass
client.username_pw_set(usuario, password)
client.connect(broker,port)           #establish connection

while not client.connected_flag: #wait in loop
   client.loop()
   time.sleep(1)

time.sleep(3)
data=dict()

X=[]
Y=[]
Z=[]
with open('Datos.csv') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    line_count = 0
    for row in csv_reader:
        X.append(row[0])
        Y.append(row[1])
        Z.append(row[2])
        line_count += 1
print(X)
print(Y)
print(Z)
cont=0
fileRead = False
while(not fileRead): #Hacemos un loop

    data["X"] = X[cont]
    data["Y"] = Y[cont]
    data["Z"] = Z[cont]
    data_out=json.dumps(data) #Creamos un objeto tipo JSON
    print("publish topic",topic, "data out= ",data_out)
    ret=client.publish(topic,data_out,0) #Publicamos
    cont+=1
    time.sleep(2)

client.disconnect()
