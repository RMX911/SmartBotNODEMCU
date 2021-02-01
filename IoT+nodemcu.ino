
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h> //  get it from https://github.com/Links2004/arduinoWebSockets/releases 
#include <ArduinoJson.h> // get it from https://arduinojson.org/ or install via Arduino library manager
#include<Hash.h>
ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;

#define MyApiKey "" // TODO: Change to your sinric API Key. Your API Key is displayed on sinric.com dashboard
#define MySSID "ajax1" // TODO: Change to your Wifi network SSID
#define MyWifiPassword "ajax123456" // TODO: Change to your Wifi network password

#define HEARTBEAT_INTERVAL 300000 // 5 Minutes 

#define option_a D4
#define option_b D5
#define option_c D6

#define Switch_1 D3
#define Switch_2 D2

#define Switch_3 D1
//#define AC D0

uint64_t heartbeatTimestamp = 0;
bool isConnected = false;

void turnOn(String deviceId) {
   
  if (deviceId == "5ba6303e9fb4046ba64a1254") // Device ID of first device
  {  
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    digitalWrite(Switch_1, LOW);
    delay(500);  
    digitalWrite(Switch_1, HIGH);
    delay(500);  
  
  } 
  else if (deviceId == "5ba6d3e89fb4046ba64a12bc") // Device ID of second device
  { 
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    digitalWrite(Switch_2, LOW);
  }
  else if (deviceId == "5ba6db6b9fb4046ba64a12bf") // Device ID of third device
  { 
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    digitalWrite(Switch_3, LOW);
  }
   else if (deviceId == "5ba6e27e9fb4046ba64a12c3") // Device ID of fourth device
  { 
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    digitalWrite(option_b, LOW);
  }
   else if (deviceId == "5ba6e0de9fb4046ba64a12c2") // Device ID of fifth device
  { 
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    digitalWrite(option_a,LOW);
  }
  else if (deviceId == "5ba6e3be9fb4046ba64a12c4") // Device ID of fifth device
  { 
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    digitalWrite(option_c,LOW);
  }
  else {
    Serial.print("Turn on for unknown device id: ");
    Serial.println(deviceId);    
  }     
}

void turnOff(String deviceId) {
   if (deviceId == "5ba6303e9fb4046ba64a1254") // Device ID of first device
   {  
     Serial.print("Turn off Device ID: ");
     Serial.println(deviceId);
     digitalWrite(Switch_1, LOW);
     delay(500);
   }
   else if (deviceId == "5ba6d3e89fb4046ba64a12bc") // Device ID of second device
   { 
     Serial.print("Turn off Device ID: ");
     Serial.println(deviceId);
     digitalWrite(Switch_2, HIGH);
  }
  else if (deviceId == "5ba6db6b9fb4046ba64a12bf") // Device ID of third device
  { 
     Serial.print("Turn off Device ID: ");
     Serial.println(deviceId);
     digitalWrite(Switch_3, HIGH);
  }
  else if (deviceId == "5ba6e27e9fb4046ba64a12c3") // Device ID of fourth device
  { 
     Serial.print("Turn off Device ID: ");
     Serial.println(deviceId);
     digitalWrite(option_b, HIGH);
  }
  else if (deviceId == "5ba6e0de9fb4046ba64a12c2") // Device ID of fifth device
  { 
     Serial.print("Turn off Device ID: ");
     Serial.println(deviceId);
     digitalWrite(option_a, HIGH);
  }
  else if (deviceId == "5ba6e3be9fb4046ba64a12c4") // Device ID of fifth device
  { 
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    digitalWrite(option_c,HIGH);
  }
  else {
     Serial.print("Turn off for unknown device id: ");
     Serial.println(deviceId);    
  }
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      isConnected = false;    
      Serial.printf("[WSc] Webservice disconnected from sinric.com!\n");
      break;
    case WStype_CONNECTED: {
      isConnected = true;
      Serial.printf("[WSc] Service connected to sinric.com at url: %s\n", payload);
      Serial.printf("Waiting for commands from sinric.com ...\n");        
      }
      break;
    case WStype_TEXT: {
        Serial.printf("[WSc] get text: %s\n", payload);
        // Example payloads

        // For Switch or Light device types
        // {"deviceId": xxxx, "action": "setPowerState", value: "ON"} // https://developer.amazon.com/docs/device-apis/alexa-powercontroller.html

        // For Light device type
        // {"deviceId": xxxx, "action": "AdjustBrightness", value: 3} // https://developer.amazon.com/docs/device-apis/alexa-brightnesscontroller.html
        // {"deviceId": xxxx, "action": "setBrightness", value: 42} // https://developer.amazon.com/docs/device-apis/alexa-brightnesscontroller.html
        // {"deviceId": xxxx, "action": "SetColor", value: {"hue": 350.5,  "saturation": 0.7138, "brightness": 0.6501}} // https://developer.amazon.com/docs/device-apis/alexa-colorcontroller.html
        // {"deviceId": xxxx, "action": "DecreaseColorTemperature", value: 0} // https://developer.amazon.com/docs/device-apis/alexa-colortemperaturecontroller.html
        // {"deviceId": xxxx, "action": "IncreaseColorTemperature", value: 0} // https://developer.amazon.com/docs/device-apis/alexa-colortemperaturecontroller.html
        // {"deviceId": xxxx, "action": "SetColorTemperature", value: 2200} // https://developer.amazon.com/docs/device-apis/alexa-colortemperaturecontroller.html
        
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject((char*)payload); 
        String deviceId = json ["deviceId"];     
        String action = json ["action"];
        
        if(action == "setPowerState") { // Switch or Light
            String value = json ["value"];
            if(value == "ON") {
                turnOn(deviceId);
            } else {
                turnOff(deviceId);
            }
        }
        else if(action == "setBrightness") {
          
        }
        else if(action == "AdjustBrightness") {
          
        }
        else if (action == "test") {
            Serial.println("[WSc] received test command from sinric.com");
        }
      }
      break;
    case WStype_BIN:
      Serial.printf("[WSc] get binary length: %u\n", length);
      break;
  }
}

void setup() {
  Serial.begin(115200);
 pinMode(option_a, OUTPUT);
  pinMode(option_b, OUTPUT);
  pinMode(option_c,OUTPUT);
  
  pinMode(Switch_2, OUTPUT);
  pinMode(Switch_3, OUTPUT);
  pinMode(Switch_1, OUTPUT);
  
  
  digitalWrite(Switch_2, HIGH);  
  delay(500);
  digitalWrite(Switch_3, HIGH);   
   delay(500);
   //digitalWrite(Switch_1, HIGH);   
   delay(500);
   
   digitalWrite(option_a, HIGH);   
   delay(500);
   digitalWrite(option_b, HIGH);   
   delay(500);
   digitalWrite(option_c, HIGH);   
   delay(500);
  WiFiMulti.addAP(MySSID, MyWifiPassword);
  Serial.println();
  Serial.print("Connecting to Wifi: ");
  Serial.println(MySSID);  

  // Waiting for Wifi connect
  while(WiFiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  if(WiFiMulti.run() == WL_CONNECTED) {
    Serial.println("");
    Serial.print("WiFi connected. ");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }

  // server address, port and URL
  webSocket.begin("iot.sinric.com", 80, "/");

  // event handler
  webSocket.onEvent(webSocketEvent);
  webSocket.setAuthorization("apikey", MyApiKey);
  
  // try again every 5000ms if connection has failed
  webSocket.setReconnectInterval(5000);   // If you see 'class WebSocketsClient' has no member named 'setReconnectInterval' error update arduinoWebSockets
}

void loop() {
  webSocket.loop();
  
  if(isConnected) {
      uint64_t now = millis();
      
      // Send heartbeat in order to avoid disconnections during ISP resetting IPs over night. Thanks @MacSass
      if((now - heartbeatTimestamp) > HEARTBEAT_INTERVAL) {
          heartbeatTimestamp = now;
          webSocket.sendTXT("H");          
      }
  }   
}
