# Plant Test Project

This project consists of a React frontend and an Express backend. The frontend is built using Create React App, and the backend uses Firebase Admin SDK for database operations.

## Project Structure

### Client

The client folder contains the React frontend.

#### Available Scripts

In the `client` directory, you can run:

- `npm start`: Runs the app in development mode. Open [http://localhost:3000](http://localhost:3000) to view it in your browser.
- `npm test`: Launches the test runner in the interactive watch mode.
- `npm run build`: Builds the app for production to the `build` folder.
- `npm run eject`: Ejects the Create React App configuration.

#### Environment Variables

- `REACT_APP_API_URL`: The URL of the backend API (default is `http://localhost:5000/api`).

### Server

The server folder contains the Express backend.

#### Available Scripts

In the `server` directory, you can run:

- `node server.js`: Runs the server in development mode. The server will start on [http://localhost:5000](http://localhost:5000).

#### API Endpoints

- `POST /api/data`: Stores data for a specific device.
- `GET /api/items`: Fetches all items from the database.
- `GET /api/devices`: Fetches all devices from the database.
- `GET /api/devices/:deviceId/data`: Fetches the latest data for a specific device.

#### Environment Variables

- `PORT`: The port on which the server will run (default is 5000).

## Learn More

To learn more about Create React App, check out the [Create React App documentation](https://facebook.github.io/create-react-app/docs/getting-started).

To learn more about Express, check out the [Express documentation](https://expressjs.com/).

To learn more about Firebase Admin SDK, check out the [Firebase Admin SDK documentation](https://firebase.google.com/docs/admin/setup).