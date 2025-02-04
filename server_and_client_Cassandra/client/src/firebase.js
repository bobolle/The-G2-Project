// filepath: /c:/Users/cassa/Documents/Projects/IOT/Data Archictecture/plant_test/src/firebase.js
import { initializeApp } from 'firebase/app';
import { getFirestore } from 'firebase/firestore';

const firebaseConfig = {
  
    apiKey: "AIzaSyA_W8EnWPLiROMOQWrUB5qTZyt-odm8WXI",

    authDomain: "planttest-fe3a1.firebaseapp.com",
  
    projectId: "planttest-fe3a1",
  
    storageBucket: "planttest-fe3a1.firebasestorage.app",
  
    messagingSenderId: "868497883550",
  
    appId: "1:868497883550:web:1ed6243969d9c1c3af968f"
  
};

const app = initializeApp(firebaseConfig);
const db = getFirestore(app);

export { db };