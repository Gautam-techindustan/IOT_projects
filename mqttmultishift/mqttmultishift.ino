#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <MultiShiftRegister.h>

int latchPin  = D1;
int clockPin = D0;
int dataPin = D2;

char ssid[] = "Yo";
char pass[] = "1234567890";

const char* mqttServer = "m15.cloudmqtt.com";
const int mqttPort =10785;
const char* mqttUser = "twqzbjfa";
const char* mqttPassword = "fxfAsRAdIH9U";

int i = 0;
const char* Sname;
int numberOfRegisters = 1;


WiFiClient espClient;
PubSubClient client(espClient);
MultiShiftRegister msr (numberOfRegisters , latchPin, clockPin, dataPin); 



void setup(){
  Serial.begin(9600);

    pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  WiFi.begin(ssid,pass);

  for(int i=0 ;i<numberOfRegisters * 8 ;i++ ){
    msr.set_shift(i);
  }
  
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if(client.connect("ESP8214752095", mqttUser, mqttPassword )) 
    {
     Serial.println("connected");
    }else{
      Serial.print("failed state ");
      Serial.print(client.state());
      delay(2000);
    }
  }

   

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
      onoff(i,object["status"]);
    }else if(strlen(object["name"]) == 8 && object["key"]=="1234567890"){
      i = ((Sname[6] - 48)*10) + ((Sname[7] - 49)*1);
      onoff(i,object["status"]);
    }
  }
  else if (mystring == "nodeswitches" ){
    for(int j=1 ; j<=numberOfRegisters * 8 ;j++ ){
      onoff(j-1,a[j]-48);
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


void reconnect(){
    while (!client.connected()) {
    String clientId ;
    
    if(client.connect("ESP8214752095", mqttUser, mqttPassword )) 
    {
     Serial.println("connected");
    }else{
      Serial.print("failed state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  
}


void loop(){

    client.loop();
    client.subscribe("buttons");
  
}
