#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <Servo.h>
#include <FirebaseESP8266.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"



//------------ Servo Configuration Data
#define SERVO_PIN_OUT 0

//------------ Firebase Configuration Data
#define API_KEY "AIzaSyBnuw6yTr4iEWVdFFeKUMYLnfxQ1drRbyE"
#define FIREBASE_FCM_SERVER_KEY "AAAA7aEg03g:APA91bGO9CQGvPoymDhABBfC_eaAKSidUlvxCOghjIG2Hc8la3NqFsgCmc78K83Lul1k1N9uiWUlPIhw9yiqAW83vxWGD0EzaA299kBAmE29dlmI-5dLZfCgQhd4wIZ94gsHf-xQ4I9I"
#define DATABASE_URL "https://prueba-meridian-default-rtdb.firebaseio.com/" 
#define FIREBASE_USER "esp@gmail.com"
#define FIREBASE_PASSWORD "ESP8266"

//------------ Wifi Configuration Data
const char *ssid = "OSORIO FONSECA ";
const char *password = "91454342";



// Create Firebase Data objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Create server object
ESP8266WebServer server(80);

// Create servo object
Servo servo;

// Process variables
bool opened = false;
String firebaseFcmDeviceToken = "";



//-------------- WiFi Configuration
void initializeWiFi() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  int time = 0;
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    time++;
    Serial.print(time);
    Serial.println("s... ");
  }
  Serial.println("Connection established");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}



//------------- API Configuration
// Request Functions
void homeRequest() {
  server.send(200, "text/plain", "Hello");
}

void notFound() {
  server.send(404, "text/plain", "Not found");
}

void getRequest() {
  String message;
  if(opened){
    message = "The door is open";
  }else{
    message = "The door is close";
  }
  sendNotification("Status", message, "data");
  server.send(200, "text/plain", message);
}

void postRequestDoor() { 
  StaticJsonDocument<200> doc;
  String post_body = server.arg("plain");

  DeserializationError error = deserializeJson(doc, post_body);
  if (error) { server.send(400); return;}

  String string_data = doc["command"];
  String message = executeCommand(string_data);
  Serial.println(message);
  sendNotification("Action executed", message, "data"); 

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", message);
}

void postRequestToken() { 
  StaticJsonDocument<200> doc;
  String post_body = server.arg("plain");

  DeserializationError error = deserializeJson(doc, post_body);
  if (error) { server.send(400); return;}

  String string_data = doc["token"];
  firebaseFcmDeviceToken = string_data;
  Serial.print("Token recieved: ");
  Serial.println(firebaseFcmDeviceToken);

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "Token recieved");
}

// Initialization
void initializeServer(){  
  server.enableCORS(true);
  server.on("/", homeRequest);
  server.on("/door", HTTP_GET, getRequest);
  server.on("/door", HTTP_POST, postRequestDoor);
  server.on("/token", HTTP_POST, postRequestToken);
  server.onNotFound(notFound);

  server.begin();
  Serial.println("HTTP Server started");
}


// Execute servo movement command function
String executeCommand(String command){
  if (command=="open"){
    servo.write(90);
    opened = true;
    setStatusToDb(opened);
    return "The door was opened";
  }
  if (command=="close"){
    servo.write(0);
    opened = false;
    setStatusToDb(opened);
    return "The door was closed";
  }
  return "Not valid command";
}



//----------- Send Push Notification 
void sendNotification(String title, String body, String data){
  fbdo.fcm.addDeviceToken(firebaseFcmDeviceToken);
  fbdo.fcm.setNotifyMessage(title, body);

  FirebaseJson msg;
  msg.add("Data", data);
  fbdo.fcm.setDataMessage(msg.raw());

  Serial.println("Sending messsage...");
  Serial.printf("Send message... %s\n", Firebase.sendMessage(fbdo, 0) ? "ok" : fbdo.errorReason().c_str());

  if (fbdo.httpCode() == FIREBASE_ERROR_HTTP_CODE_OK)
  Serial.println(fbdo.fcm.getSendResult());
  Serial.println();
}


// --------- Get and Set in Firebase Database
void getStatusFromDb(){
  if(Firebase.RTDB.getBool(&fbdo, "door/status/opened")){
    if (fbdo.dataType() == "bool") {
      Serial.print("Opened: ");
      opened = fbdo.boolData();
      Serial.println(opened);
    }
  }else{
    Serial.println("Get status from DB failed");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}

void setStatusToDb(boolean opened){
  if(Firebase.RTDB.setBool(&fbdo, "door/status/opened", opened)){
    Serial.print("Status opened set");
  }else{
    Serial.println("Set status to DB failed");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}


void setup() {
  Serial.begin(115200);
  initializeWiFi();
  initializeServer();
  servo.attach(SERVO_PIN_OUT);

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  
  // Firebase Configuration
  config.api_key = API_KEY;
  
  auth.user.email = FIREBASE_USER;
  auth.user.password = FIREBASE_PASSWORD;

  config.database_url = DATABASE_URL;

  // Assign the callback function for the long running token generation task
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  fbdo.fcm.begin(FIREBASE_FCM_SERVER_KEY);

  fbdo.fcm.setPriority("high");

  fbdo.fcm.setTimeToLive(1000);

  getStatusFromDb();
}

void loop() {
  server.handleClient();
}
