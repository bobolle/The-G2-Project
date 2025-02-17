# server/app.py
from flask import Flask
from routes import configure_routes
from models import db
import os
from seed import seed_data
from time import sleep
from flask_cors import CORS

def create_app():
    app = Flask(__name__)
    CORS(app)
    app.config['SQLALCHEMY_DATABASE_URI'] = os.environ.get('DATABASE_URL')
    app.config['SQLAlCHEMY_TRACK_MODIFICATIONS'] = False
    db.init_app(app)
    configure_routes(app)
    return app

def init_db(app):
    with app.app_context():
        try:
            db.drop_all()
        except:
            pass
        db.create_all()

def connect_to_db_with_retry(app):
    max_retries = 10
    retry_delay = 5
    for attempt in range(max_retries):
        try:
            with app.app_context():
                db.session.execute("SELECT 1")
            print("Database connection successful.")
            break
        except Exception as e:
            print(f"Attempt {attempt + 1} failed: {e}")
            if attempt < max_retries - 1:
                print(f"Retrying in {retry_delay} seconds...")
                sleep(retry_delay)
            else:
                print("Failed to connect to the database after multiple attempts.")
                raise

if __name__== '__main__':
    app = create_app()
    init_db(app)
    connect_to_db_with_retry(app)
    #seed_data(app)
    app.run(debug=True, host='0.0.0.0', port=5000)