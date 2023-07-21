import { initializeApp } from "https://www.gstatic.com/firebasejs/9.1.1/firebase-app.js";
import { getMessaging, getToken, onMessage } from "https://www.gstatic.com/firebasejs/9.1.1/firebase-messaging.js";

// Firebase configuration data
const firebaseConfig = {
  apiKey: "AIzaSyBnuw6yTr4iEWVdFFeKUMYLnfxQ1drRbyE",
  authDomain: "prueba-meridian.firebaseapp.com",
  projectId: "prueba-meridian",
  storageBucket: "prueba-meridian.appspot.com",
  messagingSenderId: "1020610532216",
  appId: "1:1020610532216:web:fe793296247430442cee8d"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);

// Initialize Firebase Cloud Messaging and get a reference to the service
const messaging = getMessaging(app);

// Global variables
let deviceIp = "";
let token = "";

// Configure service worker for cloud messaging
await navigator.serviceWorker.register('firebase-messaging-sw.mjs', { type: "module" })
  .then((register) => {
    // Get push token
    getToken(messaging, { vapidKey: 'BLK91AI8IVzKMOQPJYBg5AmTpHVrhnEZ1KGCUp15zlS0CrymMdxf_ca-4XxUuHy2O-wmbj1S5XSSZ27Hv5ohGjU' })
      .then((currentToken) => {
        console.log("Current Token", currentToken)
        token = currentToken;
      }).catch((err) => {
        console.log('An error occurred while retrieving token. ', err);
      });
  });

// Reveive message on first plane
onMessage(messaging, (payload) => {
  console.log("Message received");
  window.alert("Notification received\n" + payload.notification.title + ":\n" + payload.notification.body);
})

// Get ESP Ip from input and send push token
function setIp() {
  const ipInput = document.getElementById("ipInput");
  deviceIp = "http://" + ipInput.value;
  console.log("Device IP set to: " + deviceIp);

  let data = {
    token: token
  }
  // Send push token to the ESP
  postService("/token", data);
  document.querySelector("#confirmText").innerHTML = "Device Ip configured to " + deviceIp + " and Token send to ESP";
}

// Function to make GET request
function getService(url) {
  let xhr = new XMLHttpRequest();
  xhr.open("GET", deviceIp + url, true);
  xhr.onload = function () {
    if (xhr.status == 200) {
      console.log("GET Request succesful", xhr.responseText);
    } else {
      console.error("Error:", xhr.status);
    }
  }
  xhr.onerror = function () {
    console.error("Network Error in GET request");
  };
  xhr.send();
}

// Function to make POST request
function postService(url, data) {
  fetch(deviceIp + url, {
    method: 'POST',
    mode: 'no-cors',
    headers: {
      'Content-Type': 'application/json',
      'Access-Control-Allow-Origin': '*'
    },
    body: JSON.stringify(data)
  })
    .then(response => {
      console.log("POST request executed");
      // console.log(response);
    })
    .catch(error => {
      console.error('Error during fetch:', error.message);
      alert('Error during fetch.');
    });

}

function getDoorStatus() {
  getService("/door");
}

function closeDoor() {
  let data = {
    command: "close"
  }
  postService("/door", data);
}

function openDoor() {
  let data = {
    command: "open"
  }
  postService("/door", data);
}

// Define button actions
document.querySelector("#setIpButton").addEventListener("click", setIp);
document.querySelector("#getStatusButton").addEventListener("click", getDoorStatus);
document.querySelector("#openButton").addEventListener("click", openDoor);
document.querySelector("#closeButton").addEventListener("click", closeDoor);