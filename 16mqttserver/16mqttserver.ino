#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <MultiShiftRegister.h>


// WiFi network
ESP8266WebServer server ( 80 );

int mod = 0;
int soft = 0;

//Maintain Direct Indirect Status
int state = 0;
int softstate = 0;

int blystate=0;
int bl=1;
int resetsmarthome=0;
bool softreturn = true;

const char *ssid = "Smarthome16";
const char *password = "Smarthome";

String wifi;
String pass;
String wifiarr;
String passarr;
const char* remote_host = "www.google.com";


//demos.ths.agency
//const char* mqtt_server = "demos.ths.agency";
const char* mqtt_server = "m15.cloudmqtt.com";
const int mqtt_port = 10785;
const char* mqttUser = "twqzbjfa";
const char* mqttPassword = "fxfAsRAdIH9U";

int latchPin  = D1;
int clockPin = D2;
int dataPin = D3;

int i = 0;
const char* Sname;
int numberOfRegisters = 2;
int val[16];


WiFiClient espClient;
PubSubClient client(espClient);
MultiShiftRegister msr (numberOfRegisters , latchPin, clockPin, dataPin); 


char htmlResponse[3000];

String getData() {
  String user = server.arg("userwifi");
  String pass = server.arg("password");

  char usr[user.length()];
  char pwd[pass.length()];

  user.toCharArray(usr, user.length()+1);
  pass.toCharArray(pwd, pass.length()+1);
  server.send(200, "text","Data Accepted");

  saveWifiDetails(user, pass);
  return "HEllo";
}


void saveWifiDetails(String user, String pass){
  Serial.println("User in saveWifiDetails are:");
  Serial.println(user);
  Serial.println(pass);
  if (!user && !pass)
    return;

      Serial.println("are returning yeas or no:");
  
  int k=EEPROM.read(1)+EEPROM.read(2);
  for (int i=0;i<k;i++){
    EEPROM.write(i+1,0);
  }

  k = user.length()+3;
  EEPROM.write(1, user.length());
  EEPROM.write(2, pass.length());
         
  for(int i=0 ; i<user.length() ; i++){
      EEPROM.write(i+3, user[i]); 
  }
       
  for(int j=0 ; j<pass.length() ; j++){
      EEPROM.write(k+j, pass[j]);
  }
    
  EEPROM.commit();
  
  newWifiConnect();
}

void handleWifi(){
  server.send(200, "text",getData());
     
}

void newWifiConnect(){ 
    Serial.println("into the if condition of newWifiConnect()");
    char wifiarray[EEPROM.read(1)];
    char passarray[EEPROM.read(2)];
    
    Serial.println(server.arg("userwifi"));
    Serial.println(server.arg("password"));
    Serial.println("Server are printed");
    for(int i=0 ; i<EEPROM.read(1) ; i++){
      wifiarray[i]=EEPROM.read(i+3);
    }
    wifiarray[EEPROM.read(1)] = '\0';
    
    for(int i=0 ; i<EEPROM.read(2) ; i++) {
      passarray[i]=EEPROM.read((i+EEPROM.read(1))+3);
    }
          
    passarray[EEPROM.read(2)] = '\0'; 
          
    wifiarr=wifiarray;
    passarr=passarray;
    digitalWrite(D6,LOW);
    WiFi.begin(wifiarray,passarray);

   //Checking Connection Established Otherwise Timeout
    connectorTimeout();


    if(WiFi.status() ==  WL_CONNECTED){
        server.send(200, "text","Connected");
    }
    else{
         server.send(400, "text","Not Connected");
    }
  
    // runapp();
} 

void setInputPin() {
  mod = digitalRead(D8);
  soft = digitalRead(D5);

  if(mod == 1 ) {
    if(state == 0)
      state = 1;
    else
      state = 0;

    EEPROM.write(0, state);
    EEPROM.commit();
   delay(190);
  }
  if (soft == 1 ){
    Serial.println("Soft Value are ");
    Serial.println(soft);
    softstate++;
    delay(500);
    if (softstate == 5 && softreturn == true){
      softstate = 0;
      WiFi.mode(WIFI_OFF);
      softreturn = false;
      digitalWrite(D6 ,HIGH);
      accessPoint();
    }
    else if(softstate == 5 && softreturn == false){
      softreturn = true;
      softstate = 0;
      digitalWrite(D6 ,LOW);
      checkWifiStatus();
    }
  }
  else{
    softstate = 0;
  }
 
  
}

void runapp(){
//      setInputPin();
//      Serial.println("Loop Running"); 
      if(WiFi.status() ==  WL_CONNECTED && wifiarr.length() )
        {
//          digitalWrite(D4,HIGH); 
//           if(state==1){
//                //Write HIGH value to pins
//              digitalWrite(D0,HIGH);
//              for(int i=0 ; i<40 ;i++){
//                msr.clear_shift(i);
//              }
//                // blystate=0;
//            } 
//            else {
//                digitalWrite(D0,LOW);
                Serial.println("Run App");
         
                  int k = val[i];

                  if(k==0){
                    Serial.println("K are");
                    Serial.println(k);
                  msr.set_shift(i); 
                  }
                  else{
                    msr.clear_shift(i);
                  }
            
                    
//            }

        }  
 
//     else {
//       Serial.println("Direct Else Part");
//       digitalWrite(D4,LOW);
//       if(state==1) 
//           {
//              //Write HIGH value to pins
//            digitalWrite(D0,HIGH);
//              for(int i=0 ; i<40 ;i++){
//                msr.clear_shift(i);
//              }
//            }
//      else
//           {
//              //Write HIGH value to pins
//            digitalWrite(D0,LOW);
//              for(int i=0 ; i<40 ;i++){
//                msr.set_shift(i);
//              }
//          }
//    }
Serial.println("mqtt running");
    reConnectmqttClient();
}
void reConnectmqttClient(){
//  Serial.println("Reconnect mqtt Running"); 
      if(WiFi.status() ==  WL_CONNECTED && wifiarr.length() ){
       Serial.println("I Am In Wifi.status()");
          if (!client.connected()) {
//            Serial.println("Client.connect is running");
          reconnect();    
      } 
       if(WiFi.status() ==  WL_CONNECTED  )
      {
      client.loop();
      client.subscribe("buttons");
      client.publish("connect","{\"key\":\"1234567890\"}");
      }
        
  }   
}

void trashMemory() {
      for (int i = 0 ; i < EEPROM.length() ; i++){
        EEPROM.write(i, 0); 
      }      
      EEPROM.commit();
}

void checkWifiStatus(){
  if(!EEPROM.read(1) && !EEPROM.read(2)) {
      Serial.println("Length in memory of password and username are");
   Serial.println(EEPROM.read(1) && EEPROM.read(2));
   digitalWrite(D6 ,HIGH);
    accessPoint();
    return;
  }



  char wifiarray[EEPROM.read(1)];
  char passarray[EEPROM.read(2)];

  for(int i=0 ; i<EEPROM.read(1) ; i++){
    wifiarray[i]=EEPROM.read(i+3);
  }
  wifiarray[EEPROM.read(1)] = '\0';

  for(int i=0 ; i<EEPROM.read(2) ; i++){
    passarray[i]=EEPROM.read((i+EEPROM.read(1))+3);
  }
  passarray[EEPROM.read(2)] = '\0'; 
          
  wifiarr = wifiarray;
  passarr = passarray;
    Serial.println("wifiiiiiiiiiiiiiiiiiii");
    Serial.println(wifiarr);
     WiFi.softAPdisconnect(true);
     WiFi.begin(wifiarray, passarray);

  //Checking Connection Established Otherwise Timeout
    connectorTimeout();

  //  Blynk.begin(auth, wifiarray, passarray);
  Serial.println("wifi worked?");
  // Blynk.begin(auth, wifiarray, passarray);
}

void connectorTimeout(){
  int tries = 0 ;
      while (WiFi.status() != WL_CONNECTED) {
        
      digitalWrite(D4,HIGH);  
      delay(500);
      
      Serial.print(".");
      tries++;

      digitalWrite(D4,LOW);
      delay(500);
      if (tries > 20) {
        //offline
        // trashMemory();
        digitalWrite(D4,LOW);
        WiFi.mode(WIFI_OFF);
        break;
      }
    }
      if(WiFi.status() == WL_CONNECTED){
         client.setServer(mqtt_server, mqtt_port);
         client.setCallback(callback);
      }
}

void accessPoint(){
  WiFi.softAP(ssid, password);        
  IPAddress myIP = WiFi.softAPIP();
  Serial.println(myIP);
  server.on("/", handleWifi);
} 

// void wifiDisconnected() {
//   if(EEPROM.read(1) && EEPROM.read(2) && WiFi.status() != WL_CONNECTED) {
//     digitalWrite(D6 ,HIGH);
//     accessPoint();
//     return;
//   }
// }

void setPinMode() {
  //Output pins 1st board: 1, 2, 3 
  pinMode(D3, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  //output for softap
  pinMode(D6, OUTPUT);
  
  
  //Wifi status pin
  pinMode(D4, OUTPUT);

  //Direct Indirect Blue Light Pin 
  pinMode(D0, OUTPUT);
  
  //Input pin for direct/indirect
//  pinMode(D8, INPUT);
   //Input pin for Softap
//  pinMode(D5, INPUT);
  

  for(int i=0 ;i<16 ;i++ ){
    msr.set_shift(i);
  }
}


// This Function Run When a Anyone publish a Topic related to it
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
     
      onoff(i,object["status"]);
    }else if(strlen(object["name"]) == 8 && object["key"] == "1234567890"){
      i = ((Sname[6] - 48)*10) + ((Sname[7] - 49)*1);
      val[i] = object["status"];
      onoff(i,object["status"]);
    }
  }
  else if (mystring == "nodeswitches"){
    for(int j=1 ; j<=16 ;j++ ){
      onoff(j-1,a[j]-48);
     
      val[j-1] = a[j]-48;
    }
    
  }
}

void onoff(int i,int Sstatus){
  Serial.println(i);
  Serial.println(Sstatus);
  if(Sstatus == 1){
    Serial.println("Inside 1");
    msr.clear_shift(i);
    
  }
  else if(Sstatus == 0){
    msr.set_shift(i);
    
  }
}

//When Client is Disconnected It Reconnect client with Server
void reconnect() {
  // Loop until we're reconnected
  int time = 0;
  delay(1000);
  if(WiFi.status() ==  WL_CONNECTED){
    Serial.println("Into the Reconnect and Wifi is connected");
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    String clientId = "ESP8269544435-";
    clientId += String(random(0xffff),HEX);
    //clientId.c_str() 
    if (client.connect("ESP8266Client1", mqttUser, mqttPassword)) {
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
}

void setup() {
  
  // Start serial
  Serial.begin(9600);
  EEPROM.begin(512);
   ESP.eraseConfig();


  //Start Server
  server.begin();
//   trashMemory();

//Set Input/Output pins
  setPinMode();


  checkWifiStatus();




      
}


void loop() 
    {
      
        runapp();
        
        server.handleClient();
    }
