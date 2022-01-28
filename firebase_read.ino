/*This program is for an ESP8266 to read the value from firebase. This code is a very simple implementation of switching on/off an LED. 
My goal was to read from and write to firebase using the ESP8266 and this was the only working method I was able to find.
*/
//FirebaseESP8266.h must be included before ESP8266WiFi.h
#include "FirebaseESP8266.h"  // Install Firebase ESP8266 library
#include <ESP8266WiFi.h>
#define FIREBASE_HOST "yourhostnamegoeshere.firebaseio.com" //Without http:// or https:// schemes
#define FIREBASE_AUTH "authkeygoeshere"
#define WIFI_SSID "enteryourssid"
#define WIFI_PASSWORD "enterpassword"

int led = D5; 

//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseData stat,param;

FirebaseJson json;


void setup()
{

  Serial.begin(115200);
  
  pinMode(led,OUTPUT);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

}
String prev;
void loop() {  
  if (Firebase.getString(stat, "/test")&&Firebase.getString(param,"/blink"))
  {
    if(stat.stringData()=="on" || stat.stringData()=="oN" || stat.stringData()=="On" || stat.stringData()=="ON")
    { 
      
        if(stat.stringData()!=prev && param.stringData()!="yes")
              {
              
                  Serial.println("Command detected\n Switching on LED, no blink.");
                  delay(500);
                  digitalWrite(led,HIGH);
                  prev = stat.stringData();
              
              }
        else if(param.stringData()=="yes")
              {
                Serial.println("Command detected\n Switching on LED, blink enabled.");
                delay(750);
                digitalWrite(led,HIGH);
                delay(750);
                digitalWrite(led,LOW);
              }
              
    }
              

    else if(stat.stringData()=="off" )
    {           
            if(stat.stringData()!=prev)
                    {
                      Serial.println("Command detected\n Switching off LED");
                      delay(500);
                      digitalWrite(led,LOW);
                      prev = stat.stringData();
                      
                    }
    }
  
    else
      {
              Serial.println("Incorrect Command: on or off only allowed\n Switching off LED");
              delay(500);
              digitalWrite(led,LOW);
      }
  }
  else
    {
    
              Serial.println("ERROR: Please enter a string value only.");

    }
  delay(1000);
}
