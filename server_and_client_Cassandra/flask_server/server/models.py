# server/models.py
from flask_sqlalchemy import SQLAlchemy

db = SQLAlchemy()

class Item(db.Model):
    __tablename__ = 'item'
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(80), nullable=False)

    def __repr__(self):
        return f'<Item {self.name}>'
    
class Device(db.Model):
    __tablename__ = 'device'
    id = db.Column(db.String(80), primary_key=True)  # Changed to String
    name = db.Column(db.String(80), nullable=False)

    def __repr__(self):
        return f'<Device {self.name}>'

class DeviceData(db.Model):
    __tablename__ = 'device_data'
    id = db.Column(db.Integer, primary_key=True)
    device_id = db.Column(db.String(80), db.ForeignKey('device.id'), nullable=False)
    timestamp = db.Column(db.DateTime, nullable=False)
    light_level = db.Column(db.Float, nullable=False)
    moisture_level = db.Column(db.Float, nullable=False)

    def __repr__(self):
        return f'<DeviceData {self.device_id}: Light={self.light_level}, Moisture={self.moisture_level}>'