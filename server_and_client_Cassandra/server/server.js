const express = require('express');
const cors = require('cors');
const admin = require('firebase-admin');
const serviceAccount = require('./serviceAccountKey.json');

admin.initializeApp({
    credential: admin.credential.cert(serviceAccount)
});

const db = admin.firestore();
const app = express();
const port = 5000;

app.use(cors());
app.use(express.json());

app.post('/api/data', async (req, res) => {
    const { deviceId, data } = req.body;
    try {
      const deviceDocRef = db.collection('devicetestdatabase').doc(deviceId);
      await deviceDocRef.collection('data').add(data);
      res.status(200).send('Data received and stored successfully');
    } catch (error) {
      console.error('Error storing data:', error);
      res.status(500).send('Error storing data');
    }
  });
  

  app.get('/api/items', async (req, res) => {
    try {
      const snapshot = await db.collection('planttestdatabase').get();
      const items = snapshot.docs.map(doc => ({ id: doc.id, ...doc.data() }));
      res.json(items);
    } catch (error) {
      console.error('Error fetching items:', error);
      res.status(500).send('Error fetching items');
    }
  });

  app.get('/api/devices', async (req, res) => {
    try {
      const snapshot = await db.collection('devicetestdatabase').get();
      const devices = snapshot.docs.map(doc => ({ id: doc.id, ...doc.data() }));
      res.json(devices);
    } catch (error) {
      console.error('Error fetching devices:', error);
      res.status(500).send('Error fetching devices');
    }
  });

app.get('/api/devices/:deviceId/data', async (req, res) => {
    const { deviceId } = req.params;
    try {
      const snapshot = await db.collection('devicetestdatabase').doc(deviceId).collection('data').orderBy('timestamp', 'desc').limit(1).get();
      const latestData = snapshot.docs.map(doc => doc.data());
      res.json(latestData);
    } catch (error) {
      console.error('Error fetching latest device data:', error);
      res.status(500).send('Error fetching latest device data');
    }
  });
  
  app.listen(port, '0.0.0.0', () => {
    console.log(`Server is running on http://localhost:${port}`);
  });