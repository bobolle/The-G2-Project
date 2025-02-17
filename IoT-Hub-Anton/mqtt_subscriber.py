import paho.mqtt.client as mqtt
import psycopg2
import json
import os
import time
import requests
from dotenv import load_dotenv
from datetime import datetime

# config
load_dotenv("config.env")
DB_NAME = os.getenv("DB_NAME")
DB_USER = os.getenv("DB_USER")
DB_PASSWORD = os.getenv("DB_PASSWORD")
DB_HOST = os.getenv("DB_HOST")
MQTT_BROKER = os.getenv("MQTT_BROKER")
MQTT_TOPIC = os.getenv("MQTT_TOPIC")
MQTT_USER = os.getenv("MQTT_USER")
MQTT_PASSWORD = os.getenv("MQTT_PASSWORD")
SERVER_URL = os.getenv("SERVER_URL")

# Connect to PostgreSQL
conn = psycopg2.connect(
    dbname=DB_NAME, user=DB_USER,password=DB_PASSWORD,host=DB_HOST
)
cursor = conn.cursor()

# MQTT-callback function at message
def on_message(client, userdata,msg):
    try:
        data = json.loads(msg.payload.decode())

        device_id = data.get("device_id", "unknown")
        lightlevel = data.get("lightlevel", 0.0)
        moisture = data.get("moisture", 0.0)
        error_code = data.get("error_code", 0)

        # Write in PostgreSQL
        cursor.execute(
            "INSERT INTO sensor_log (device_id, lightlevel, moisture, error_code) VALUES (%s, %s, %s, %s)",
            (device_id, lightlevel, moisture, error_code),
        )
        conn.commit()
        print(f"Saved data: {device_id}: {lightlevel}, {moisture}, error code: {error_code}")

    except Exception as e:
        print(f"Error with adding to database: {e}"
        )

# Change to only delete diversing from average and with no error code
def delete_old_data():
    try:
        cursor.execute("DELETE FROM sensor_log WHERE timestamp < NOW() - INTERVAL '30 days'")
        conn.commit()
        print("Deleted old data.")
    except Exception as e:
        print(f"Error when deleting: {e}")

def send_to_cloud():
    try:
        cursor.execute("""
                       SELECT device_id, AVG(lightlevel), AVG(moisture) 
                       FROM sensor_log 
                       WHERE timestamp >= NOW() - INTERVAL '10 minutes'
                       GROUP BY device_id
                       """)
        results = cursor.fetchall()

        for row in results:
            data = {
                "timestamp": datetime.utcnow().isoformat(),
                "deviceId":  row[0],
                "lightLevel":round(row[1]),
                "moistureLevel":round(row[2]),
                "deviceName:": "GardenHub1"
            }
            headers ={'Content-Type': 'application/json'}
            response = requests.post(SERVER_URL, json=data, headers=headers)
            if response.status_code == 201:
                print(f"Success, data sent to cloud: {response.json()}")
            else:
                print(f"Failed, status code: {response.status_code} Tried to send: {response.json()} , Response: {response.text}")


    except Exception as e:
        print(f"Error at aggregation/cloud-post: {e}")



# Mqtt connect
client = mqtt.Client()
client.on_message = on_message
client.username_pw_set(MQTT_USER, MQTT_PASSWORD)
client.connect(MQTT_BROKER, 1883, 60)
client.subscribe(MQTT_TOPIC)
client.loop_start()

# Main loop

try:
    while True:
        time.sleep(600)
        delete_old_data()
        send_to_cloud()
except KeyboardInterrupt:
    print("\nClosing script...")
    client.loop_stop()
    cursor.close()
    conn.close()
