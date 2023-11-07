

#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include <Servo.h>

Servo servo;




//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "HUAWEI nova 2i"
#define WIFI_PASSWORD "19971103"

// Insert Firebase project API Key
#define API_KEY "AIzaSyDKsV8VgDb_pDAWwm5z6fMWizeLPjY0BfU"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://iotcarparck-default-rtdb.firebaseio.com" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

//unsigned long sendDataPrevMillis = 0;
//int count = 0;
bool signupOK = false;


int inputpinD1 = 5;
int inputpinD2 = 4;
int inputpinD3 = 0;
int inputpinD4 = 2;
int servoPin = 14;


int d1 = 0;
int d2 = 0;
int d3 = 0;
int d4 = 0;

int count = 0;








void setup(){
  Serial.begin(115200);

  pinMode(inputpinD1,INPUT);
  pinMode(inputpinD2,INPUT);
  pinMode(inputpinD3,INPUT);
  pinMode(inputpinD4,INPUT);

  servo.attach(servoPin);


  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);



}


void loop(){
  
  if(digitalRead(inputpinD1)==LOW){
        Serial.println("D1 Detected object");
        d1 = 1;
  }else{
     Serial.println("D1 Does not Detected object");
     d1 = 0;
  }


  if(digitalRead(inputpinD2)==LOW){
        Serial.println("D2 Detected object");
        d2 = 1;
  }else{
     Serial.println("D2 Does not Detected object");
        d2 = 0;
  }
 
  if(digitalRead(inputpinD3)==LOW){
        Serial.println("D3 Detected object");
        d3 = 1;
  }else{
     Serial.println("D3 Does not Detected object");
     d3 = 0;
  }

  if(digitalRead(inputpinD4)==LOW){
        Serial.println("D4 Detected object");
        d4 = 1;
  }else{
     Serial.println("D4 Does not Detected object");
     d4 = 0;
  }

    count = d1+d2+d3+d4;

    if(count == 4){
      Serial.println("Car park is Full");
      servo.write(150);
    }else{
      servo.write(0);
      Serial.print("You have ");
      Serial.print(4-count);
      Serial.print(" Spaces");
    }


    if (Firebase.ready() && signupOK ) {
    
            if (Firebase.RTDB.setInt(&fbdo, "CarParck/Values/Spaces",4-count)){
        //      Serial.println("PASSED");
                Serial.print("Data base Spaces: ");
                Serial.print(4-count);
                
              
            }
            else {
              Serial.println("FAILED");
              Serial.println("REASON: " + fbdo.errorReason());
            }
            
            
            
     }
     


     delay(1000);

}

 
