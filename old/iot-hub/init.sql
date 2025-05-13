CREATE TABLE IF NOT EXISTS device (id SERIAL PRIMARY KEY, name VARCHAR(80) NOT NULL);

INSERT INTO device (name) VALUES ('pico_test_device');
