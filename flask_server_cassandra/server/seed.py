from models import db, Item, Device, DeviceData

def seed_data(app):
    with app.app_context():
        db.create_all()
        
        try:
            if not Item.query.first():
                print("Seeding Items...")
                item1 = Item(name='Test Item 1')
                item2 = Item(name='Test Item 2')
                db.session.add_all([item1, item2])
                db.session.commit()

            if not Device.query.first():
                print("Seeding Devices...")
                device1 = Device(name='Test Device 1')
                db.session.add(device1)
                db.session.commit()

            if not DeviceData.query.first():
                print("Seeding Device Data...")
                device = Device.query.first()
                if device:
                    data1 = DeviceData(deviceId=device.id, value=10)
                    data2 = DeviceData(deviceId=device.id, value=20)
                    db.session.add_all([data1, data2])
                    db.session.commit()

            print("Database seeded successfully!")
            
        except Exception as e:
            print(f"Error seeding database: {e}")
            db.session.rollback()
            raise