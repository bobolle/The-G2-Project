-- Initialize database schema
CREATE TABLE IF NOT EXISTS item (
    id SERIAL PRIMARY KEY,
    name VARCHAR(80) NOT NULL
);

CREATE TABLE IF NOT EXISTS device (
    id VARCHAR(80) PRIMARY KEY,  -- Changed from SERIAL to VARCHAR
    name VARCHAR(80) NOT NULL
);

CREATE TABLE IF NOT EXISTS device_data (
    id SERIAL PRIMARY KEY,
    device_id VARCHAR(80) REFERENCES device(id),
    timestamp TIMESTAMP NOT NULL,
    light_level FLOAT NOT NULL,
    moisture_level FLOAT NOT NULL
);