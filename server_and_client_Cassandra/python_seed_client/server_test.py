import requests
import random
import time
from datetime import datetime

def send_test_data(device_id="test1", num_requests=5, interval=2):
    """
    Send test data to the API endpoint
    :param device_id: ID of the device to send data for
    :param num_requests: Number of requests to send
    :param interval: Time between requests in seconds
    """
    url = 'http://localhost:5000/api/data'
    headers = {'Content-Type': 'application/json'}

    for i in range(num_requests):
        # Generate random sensor data
        test_data = {
            'deviceId': device_id,
            'timestamp': datetime.utcnow().isoformat(),
            'lightLevel': round(random.uniform(0, 100), 2),
            'moistureLevel': round(random.uniform(0, 100), 2),
            'deviceName': f'Test Device {device_id}'
        }
        
        try:
            response = requests.post(url, json=test_data, headers=headers)
            if response.status_code == 201:
                print(f"Request {i+1}: Success")
                print(f"Data sent: {test_data}")
                print(f"Response: {response.json()}\n")
            else:
                print(f"Request {i+1}: Failed - Status Code: {response.status_code}")
                print(f"Response: {response.text}\n")
        except requests.exceptions.RequestException as e:
            print(f"Request {i+1}: Error - {e}\n")
        
        time.sleep(interval)

if __name__ == '__main__':
    send_test_data()