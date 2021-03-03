
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <MultiShiftRegister.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).


// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Yo";
char pass[] = "1234567890";
int mod = 0;
int state = 0;
int blystate=0;
int bl=1;

int latchPin  = D1;
int clockPin = D2;
int dataPin = D3;

const char* mqttServer = "demos.ths.agency";
const int mqttPort =1883;
const char* mqttUser = "";
const char* mqttPassword = "";


int i = 0;
const char* Sname;
int numberOfRegisters = 5;
int val[40];



WiFiClient espClient;
PubSubClient client(espClient);
MultiShiftRegister msr (numberOfRegisters , latchPin, clockPin, dataPin); 



void setup()
{
  // Debug console  
  Serial.begin(9600);
  EEPROM.begin(512);
  
    pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  pinMode(D4,OUTPUT);
  pinMode(D0,OUTPUT);

//  pinMode(D10, OUTPUT);

  for(int i=0 ;i<40 ;i++ ){
    msr.set_shift(i);
  }
  digitalWrite(D4,LOW);
  digitalWrite(D0, LOW);
//  digitalWrite(10,LOW);
 pinMode(D8,INPUT);

 WiFi.begin(ssid,pass);

 client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
   String clientId = "ESP8269517535-";
   clientId += String(random(0xffff),HEX);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    String clientId = "ESP8269517535-";
    clientId += String(random(0xffff),HEX);
    if(client.connect(clientId.c_str(), mqttUser, mqttPassword )) 
    {
     Serial.println("connected");
     client.subscribe("nodeswitches");
     client.publish("sync","{\"key\":\"1234567890\"}");

    }else{
      Serial.print("failed state ");
      Serial.print(client.state());
      delay(2000);
    }
  }

 state = EEPROM.read(0);
 if(state == 97)
  state = 0;

}

void callback(char* topic, byte* payload, unsigned int length)
{ 

  Serial.begin(9600);
    Serial.print("Messageved in topic: ");
  Serial.println(topic);
  char a[length]; 
  StaticJsonBuffer<2000> jsonBuffer;
  Serial.print("Message"); 
   for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    a[i]=payload[i];
  }
  a[length]= '\0';

  String mystring(topic);

  if(mystring == "buttons")
  {
    JsonObject& object = jsonBuffer.parseObject(a);
    Sname = object["name"];
    Serial.println();
  
    if (strlen(object["name"]) == 7 && object["key"] == "1234567890"){
      i = Sname[6] - 49;
      val[i] = object["status"];
      Serial.println(val[i]);
      onoff(i,object["status"]);
    }else if(strlen(object["name"]) == 8 && object["key"] == "1234567890"){
      i = ((Sname[6] - 48)*10) + ((Sname[7] - 49)*1);
      val[i] = object["status"];
      onoff(i,object["status"]);
    }
  }
  else if (mystring == "nodeswitches"){
    for(int j=1 ; j<=40 ;j++ ){
      onoff(j-1,a[j]-48);
      Serial.println(a[j]);
      val[j-1] = a[j]-48;
    }
    
  }
}

void onoff(int i,int Sstatus){
  if(Sstatus == 1){
    msr.clear_shift(i);
    
  }
  else if(Sstatus == 0){
    msr.set_shift(i);
    
  }
}
void reconnect() {
  // Loop until we're reconnected
  int time = 0;
   
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    String clientId = "ESP8269517535-";
    clientId += String(random(0xffff),HEX); 
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("nodeswitches");
      client.publish("sync","{\"key\":\"1234567890\"}");
      // Once connected, publish an announcement...
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 3 seconds");
      delay(1000);
      time++;
      if(time==3)
      break;
    }
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
     delay(500);
  } 
   
  if(WiFi.status() ==  WL_CONNECTED )
    {

      digitalWrite(D4,HIGH); 
      
      if(state==1)
        {
            
            digitalWrite(D0,HIGH);
            for(int i=0 ; i<40 ;i++){
              msr.clear_shift(i);
            }
        }
      else
        {    
          digitalWrite(D0,LOW);
          for(int i=0 ;i<40 ;i++){
            int k = val[i];

            if(k==0){
             msr.set_shift(i); 
            }
            else{
              msr.clear_shift(i);
            }
              
          }
        }
    
    }
  else 
    {
          digitalWrite(D4,LOW);
      if(state==1) 
           {
            
              digitalWrite(D0,HIGH);
              for(int i=0 ; i<40 ;i++){
                msr.clear_shift(i);
              }
            }
      else
           {
        
              digitalWrite(D0,LOW);
              for(int i=0 ; i<40 ;i++){
                msr.set_shift(i);
              }
          }
    }

   if(WiFi.status()==WL_CONNECTED){
    if(!client.connected()){
      reconnect();
    }
    client.loop();
    client.subscribe("buttons");
   }

  
}
