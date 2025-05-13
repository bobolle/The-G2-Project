import paho.mqtt.client as mqtt
import psycopg2
import time
import requests

def on_connect(client, userdata, flags, reason_code, properties):
    if reason_code.is_failure:
        print(f"connection to mqtt-broker returned with result code {reason_code}")
    else:
        print(f"connected to mqtt-broker")

def on_message(client, userdata, msg):
    try:
        data = msg.payload.decode()
        print(f"message received: {data}")

        cur = conn.cursor()
        sql = f"INSERT INTO device (name) VALUES ('{data}')"
        cur.execute(sql)
        conn.commit()

        print(f"stored in db: {data}")

        send_to_cloud('/post/device', data)
    except Exception as e:
        print(f"error on_message: {e}")

def calc_avg():
    return avg;

def send_to_cloud(path, data):
    url = 'http://192.168.1.13:9090/api' + path
    headers = {'Content-Type': 'application/json'}
    json_data = {'name': data}

    r = requests.post(url, headers=headers, json=json_data)

    return 1

mqttc = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
mqttc.on_connect = on_connect
mqttc.on_message = on_message

mqttc.connect("broker", 1883, 60)
mqttc.subscribe("pico/sensor/distance")

conn = psycopg2.connect("dbname=db user=db_admin password=1234 host=db port=5432")

# from msgqueue, fetch and decode msg
# insert to db
# every 50 inserts, calc avg. , convert to json, make put req

mqttc.loop_forever()
