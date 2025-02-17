import requests
import random
import time

def send_test_data(device_id=1, num_requests=5, interval=2):
    """
    Send test data to the API endpoint
    :param device_id: ID of the device to send data for
    :param num_requests: Number of requests to send
    :param interval: Time between requests in seconds
    """
    url = 'http://localhost:5000/api/data'
    headers = {'Content-Type': 'application/json'}

    for i in range(num_requests):
        # Generate random value between 0 and 100
        test_data = {
            'deviceId': device_id,
            'value': random.randint(0, 100)
        }
        
        try:
            response = requests.post(url, json=test_data, headers=headers)
            if response.status_code == 201:
                print(f"Request {i+1}: Success - {response.json()}")
            else:
                print(f"Request {i+1}: Failed - Status Code: {response.status_code}")
        except requests.exceptions.RequestException as e:
            print(f"Request {i+1}: Error - {e}")
        
        time.sleep(interval)

if __name__ == '__main__':
    send_test_data()