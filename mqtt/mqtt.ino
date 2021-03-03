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

WiFiClient espClient;
PubSubClient client(espClient);

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
  pinMode(D5,OUTPUT);
  pinMode(D6,OUTPUT);
  pinMode(D7,OUTPUT);
  pinMode(D10, OUTPUT);
  digitalWrite(D1,HIGH);
  digitalWrite(D2,HIGH);
  digitalWrite(D3,HIGH);
  digitalWrite(D4,LOW);
  digitalWrite(D0,HIGH);
  digitalWrite(D5,HIGH);
  digitalWrite(D6,HIGH);
  digitalWrite(D7,HIGH);
  digitalWrite(D10, LOW);
//  digitalWrite(10,LOW);
 pinMode(D8,INPUT);

 state = EEPROM.read(0);
 if(state == 97)
  state = 0;

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if(client.connect("ESP8266Client", mqttUser, mqttPassword )) 
    {
     Serial.println("connected");
    }else{
      Serial.print("failed state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  client.publish("connect","{\"username\":\"tester\",\"password\":\"admin\"}");
  client.subscribe("connected");// here is where you later add a wildcard
  
Serial.println("firebase connected?");

}


void callback(char topic, byte payload, unsigned int length)
{ 
  char a[length];
  StaticJsonBuffer<200> jsonBuffer;
  
  Serial.print("Messageved in topic: ");
  Serial.println(topic);
  Serial.print("Message"); 
   for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    a[i]=payload[i];

  }
  a[length]= '\0';

  Serial.println();
  Serial.println("payload");
  Serial.println(a);
  
  JsonObject& object = jsonBuffer.parseObject(a);
  const char* kye = object["key"];
  Serial.println(kye);

  if(object["switch"] == "switch1"){
    digitalWrite(D1,object["status"]);
    s1 = object["status"];
  } else if (object["switch"] == "switch2"){
    s2 = object["status"];
  } else if (object["switch"] == "switch3"){
    s3 = object["status"];
  }
  
  

 
}

void loop()
{

  
  mod = digitalRead(D8);
  if(mod == 1 ) {
    if(state == 0)
      state = 1;
    else
      state = 0;

     EEPROM.write(0, state);
     EEPROM.commit();
     delay(750);
  } 
   
  if(WiFi.status() ==  WL_CONNECTED  )
    {
      digitalWrite(D4,HIGH); 
      

      
      if(state==1)
        {
            Serial.println("direct  ");
            digitalWrite(D1,LOW);
            digitalWrite(D2,LOW);
            digitalWrite(D3,LOW);
            digitalWrite(D0,LOW);
            digitalWrite(D5,LOW);
            digitalWrite(D6,LOW);
            digitalWrite(D7,LOW);
            digitalWrite(D10,HIGH);
      
        }
    
     else
        { 

            client.loop();
          client.subscribe("switch");

          Serial.println(s1);
             digitalWrite(D1, s1);
          Serial.println(s2);
              digitalWrite(D2, s2);
       
              digitalWrite(D3,s3);
                
             digitalWrite(D10,LOW);
        
          }
            

       }

 else 
    {
      if(state==1) 
           {
              //Serial.print("ELSE");
              digitalWrite(D4,LOW);  
              digitalWrite(D1,LOW);
              digitalWrite(D2,LOW);
              digitalWrite(D3,LOW);
              //digitalWrite(D4,LOW);
              digitalWrite(D0,LOW);
              digitalWrite(D5,LOW);
              digitalWrite(D6,LOW);
              digitalWrite(D7,LOW);
              digitalWrite(D10,HIGH);
            }
      else
           {
              //Serial.print("ELSE2");
              digitalWrite(D1,HIGH);
              digitalWrite(D2,HIGH);
              digitalWrite(D3,HIGH);
              digitalWrite(D4,LOW);
              digitalWrite(D0,HIGH);
              digitalWrite(D5,HIGH);
              digitalWrite(D6,HIGH);
              digitalWrite(D7,HIGH);
              digitalWrite(D10,LOW);
          }
    }
  
}
