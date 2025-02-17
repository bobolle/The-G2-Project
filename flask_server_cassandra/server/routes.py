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
        data = DeviceData.query.filter_by(device_id=deviceId).all()  # Changed from deviceId
        return jsonify([{'id': d.id, 'deviceId': d.device_id, 'value': d.value} for d in data])
    
    @app.route('/api/data', methods=['POST'])
    def post_data():
        data = request.get_json()
        device_id = data.get('deviceId')
        device_name = data.get('deviceName', f'Device {device_id}')  # Optional device name
        
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
            value=data.get('value')
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
                    'deviceId': new_data.device_id,
                    'value': new_data.value
                }
            }), 201
        except Exception as e:
            db.session.rollback()
            return jsonify({
                'error': 'Failed to save data',
                'details': str(e)
            }), 500