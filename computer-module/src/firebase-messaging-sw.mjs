import { initializeApp } from "https://www.gstatic.com/firebasejs/9.1.1/firebase-app.js";
import { getMessaging, onBackgroundMessage } from "https://www.gstatic.com/firebasejs/9.1.1/firebase-messaging-sw.js";

// Initialize Firebase
const firebaseApp = initializeApp({
  apiKey: "AIzaSyBnuw6yTr4iEWVdFFeKUMYLnfxQ1drRbyE",
  authDomain: "prueba-meridian.firebaseapp.com",
  projectId: "prueba-meridian",
  storageBucket: "prueba-meridian.appspot.com",
  messagingSenderId: "1020610532216",
  appId: "1:1020610532216:web:fe793296247430442cee8d"
});

// Initialize Firebase Cloud Messaging and get a reference to the service
const messaging = getMessaging(firebaseApp);
