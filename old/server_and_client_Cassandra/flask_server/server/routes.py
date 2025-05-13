# server/routes.py
from flask import request, jsonify
from models import db, Item, Device, DeviceData

def configure_routes(app):
    @app.route('/', methods=['GET'])
    def home():
        return jsonify({
            'message': 'Welcome to the IoT API',
            'available_endpoints': {
                'GET /api/items': 'Get all items',
                'GET /api/devices': 'Get all devices',
                'GET /api/devices/<deviceId>/data': 'Get data for a specific device',
                'POST /api/data': 'Post new device data'
            }
        })

    @app.route('/api/items', methods=['GET'])
    def get_items():
        items = Item.query.all()
        return jsonify([{'id': item.id, 'name': item.name} for item in items])
    
    @app.route('/api/devices', methods=['GET'])
    def get_devices():
        devices = Device.query.all()
        return jsonify([{'id': device.id, 'name': device.name} for device in devices])
    
    @app.route('/api/devices/<deviceId>/data', methods=['GET'])
    def get_device_data(deviceId):
        data = DeviceData.query.filter_by(device_id=deviceId).all()
        return jsonify([{
            'id': d.id,
            'deviceId': d.device_id,
            'timestamp': d.timestamp.isoformat(),
            'lightLevel': d.light_level,
            'moistureLevel': d.moisture_level
        } for d in data])

    @app.route('/api/data', methods=['POST'])
    def post_data():
        data = request.get_json()
        
        # Extract required fields
        device_id = data.get('deviceId')
        timestamp = data.get('timestamp')
        light_level = data.get('lightLevel')
        moisture_level = data.get('moistureLevel')
        device_name = data.get('deviceName', f'Device {device_id}')

        # Validate required fields
        if not all([device_id, timestamp, light_level, moisture_level]):
            return jsonify({
                'error': 'Missing required fields',
                'required': ['deviceId', 'timestamp', 'lightLevel', 'moistureLevel']
            }), 400

        # Check if device exists, if not create it
        device = Device.query.get(device_id)
        if not device:
            try:
                device = Device(id=device_id, name=device_name)
                db.session.add(device)
                db.session.commit()
            except Exception as e:
                db.session.rollback()
                return jsonify({
                    'error': 'Failed to create device',
                    'details': str(e)
                }), 500

        # Create new device data
        new_data = DeviceData(
            device_id=device_id,
            timestamp=timestamp,
            light_level=light_level,
            moisture_level=moisture_level
        )

        try:
            db.session.add(new_data)
            db.session.commit()
            return jsonify({
                'message': 'Data received and saved',
                'device': {
                    'id': device.id,
                    'name': device.name
                },
                'data': {
                    'id': new_data.id,
                    'timestamp': new_data.timestamp.isoformat(),
                    'deviceId': new_data.device_id,
                    'lightLevel': new_data.light_level,
                    'moistureLevel': new_data.moisture_level
                }
            }), 201
        except Exception as e:
            db.session.rollback()
            return jsonify({
                'error': 'Failed to save data',
                'details': str(e)
            }), 500