#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Set these to run example.





// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
//char auth[] = "2f61d44d154c4e4ba56f50c3e1a05a92";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Yo";
char pass[] = "1234567890";
int mod = 0;
int state = 0;
int blystate=0;
int bl=1;
const char* mqttServer = "demos.ths.agency";
const int mqttPort =1883;
const char* mqttUser = "";
const char* mqttPassword = "";
int s1 ,s2 , s3;
int syncpub = 0;


WiFiClient espClient;
PubSubClient client(espClient);
void callback(char* topic, byte* payload, unsigned int length);

void setup()
{
  // Debug console  
  Serial.begin(9600);
  EEPROM.begin(512);
  
  pinMode(D1,OUTPUT);
  pinMode(D2,OUTPUT);
  pinMode(D3,OUTPUT);
  pinMode(D4,OUTPUT);
  pinMode(D0,OUTPUT);
  //pinMode(D10, OUTPUT);
  digitalWrite(D1,HIGH);
  digitalWrite(D2,HIGH);
  digitalWrite(D3,HIGH);
  digitalWrite(D4,LOW);
  digitalWrite(D0, LOW);
//  digitalWrite(10,LOW);
 pinMode(D8,INPUT);

 Serial.begin(9600);

 state = EEPROM.read(0);
 if(state == 97)
  state = 0;

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  client.publish("sync" , "{\"key\":\"1234567890\"}");
  while (!client.connected()) {
        Serial.println("Connecting to MQTT...");
        String clientId = "ESP8266365276-";
        clientId += String(random(0xffff), HEX);
    if(client.connect(clientId.c_str(), mqttUser, mqttPassword )) 
    {
        Serial.println("connected");
        client.publish("sync" , "{\"key\":\"1234567890\"}");
            client.subscribe("nodeswitches");
            client.subscribe("switch");
        client.publish("sync" , "{\"key\":\"1234567890\"}");
    }else{
        Serial.print("failed state ");
        Serial.print(client.state());
         delay(2000);
    }
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266365276-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
   if(client.connect(clientId.c_str(), mqttUser, mqttPassword ))  {
      Serial.println("connected");
      // Once connected, publish an announcement...
         client.publish("sync" , "{\"key\":\"1234567890\"}");
            client.subscribe("nodeswitches");
            client.subscribe("switch");
        client.publish("sync" , "{\"key\":\"1234567890\"}");
    } else {
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      delay(2000);
    }
  }
}


void callback(char* topic, byte* payload, unsigned int length)
{ 

  Serial.begin(9600);
  char a[length];
  
  StaticJsonBuffer<2000> jsonBuffer;
 
   for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    a[i]=payload[i];
    }
    Serial.println();
  a[length]= '\0';
  
  String mystring(topic);

  JsonObject& object = jsonBuffer.parseObject(a);
  if(mystring == "switch"){
    if(object["name"] == "switch1"){
      s1 = object["status"];
    } else if (object["name"] == "switch2"){
      s2 = object["status"];
    } else if (object["name"] == "switch3"){
      s3 = object["status"];
    } 
  } else if(mystring == "nodeswitches"){
    s1 = a[1] - 48;
    s2 = a[2] - 48;
    s3 = a[3] - 48;  
  }
}


void loop()
{

//  if(client.connected()){
//    Serial.println("connected");
//  }else{
//    Serial.println("not connected");
//  }
   if(!client.connected()){
      reconnect();
   }  
    client.loop();
    client.subscribe("switch");
  

  mod = digitalRead(D8);
  if(mod == 1 ) {
    if(state == 0) {
      state = 1;
    }
    else {
      state = 0;
    }

     EEPROM.write(0, state);
     EEPROM.commit();
     delay(500);
  } 
  
  if(WiFi.status() ==  WL_CONNECTED  )
    { 
      digitalWrite(D4,HIGH);    
      if(state==1)
        {
            digitalWrite(D1,LOW);
            digitalWrite(D2,LOW);
            digitalWrite(D3,LOW);
            digitalWrite(D0,HIGH);
      
        }
    
     else
        {               
            digitalWrite(D1, !s1);
            digitalWrite(D2, !s2);
            digitalWrite(D3, !s3);
            digitalWrite(D0,LOW);           
        }
    }
  else 
    {
      digitalWrite(D4,LOW);
      if(state==1) 
           {
              //Serial.print("ELSE");
              digitalWrite(D4,LOW);  
              digitalWrite(D1,LOW);
              digitalWrite(D2,LOW);
              digitalWrite(D3,LOW);
              digitalWrite(D0,HIGH);
            }
      else
           {
              //Serial.print("ELSE2");
              digitalWrite(D1,HIGH);
              digitalWrite(D2,HIGH);
              digitalWrite(D3,HIGH);
              digitalWrite(D0,LOW);
          }
    }

}
