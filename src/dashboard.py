#Librerias
import csv
import time,json
import time
import paho.mqtt.client as mqtt

#Variable global
counter = 0

#Funciones de MQTT para realizar la conexion entre el archivo .csv y el dashboard 

def on_log(client, userdata, level, buf):
   print(buf) 

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        client.connected_flag=True 
        print("Connection established. OK") 

    else:
        print("Connection failed, could not establish link.",rc) 
        client.loop_stop()  

def on_disconnect(client, userdata, rc): 
   print("Disconnection successful.") 

def on_publish(client, userdata, mid):
    print("In on_pub callback mid= "  ,mid)

#Funcionalidades de MQTT

mqtt.Client.connected_flag=False 
mqtt.Client.suppress_puback_flag=False

client = mqtt.Client("python1")              
client.on_connect = on_connect 
client.on_disconnect = on_disconnect 

#Publicacion de la informacion
client.on_publish = on_publish 
port =1883
broker="iot.eie.ucr.ac.cr"
topic="v1/devices/me/telemetry"

#Token de usuario (se consigue al crear un 'device' en ThingsBoard)
usuario="rC7brimfZMespuYUn0eO"
password=""

if usuario !="":
   pass
client.username_pw_set(usuario, password)
client.connect(broker,port)

while not client.connected_flag: 
    client.loop()
    time.sleep(1)

time.sleep(2)
data = dict()


#Creacion de listas 
X = list()
Y = list()
Z = list()
V = list()


#Se manda a abrir el archivo Datos.csv
with open('Datos.csv') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    line_counter = 0
    for row in csv_reader:
        X.append(row[0])
        Y.append(row[1])
        Z.append(row[2])
        V.append(row[3])
        
        line_counter += 1

fileRead = False
counter2 = 0

#Se comienzan a dar los datos
while(not fileRead):

    data["X"] = X[counter2]
    data["Y"] = Y[counter2]
    data["Z"] = Z[counter2]
    data["V"] = V[counter2]
    


    data_out = json.dumps(data) 
    print("publish topic",topic, "data out= ", data_out)
    pub = client.publish(topic, data_out, 0)
    counter2 += 1
    time.sleep(1)

client.disconnect()