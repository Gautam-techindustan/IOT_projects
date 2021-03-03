#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>


// Set these to run example.

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
//char auth[] = "2f61d44d154c4e4ba56f50c3e1a05a92";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "SmartBell";
char pass[] = "1234567890";

const char* mqttServer = "m15.cloudmqtt.com";
const int mqttPort =14616;
const char* mqttUser = "hefekvox";
const char* mqttPassword = "mSKLQCwN_6Xm";

bool restartvalue = true;


WiFiClient espClient;
PubSubClient client(espClient);
LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setup()
{
  Serial.begin(9600);
  Wire.begin(D2,D1);
  lcd.begin(16,2);
  lcd.init();   // initializing the LCD
  lcd.backlight();

  pinMode(D0, OUTPUT);
  digitalWrite(D0, HIGH);
  WiFi.softAPdisconnect(true);
  WiFi.enableAP(false);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
    lcd.setCursor(1, 0);
    lcd.print("Connecting");
  }

  
  Serial.println("Connected to the WiFi network");
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  
  
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    String clientId = "ESP8146228067";
    clientId += String(random(0xffff),HEX);
    //clientId.c_str() 
    if(client.connect(clientId.c_str() , mqttUser, mqttPassword )) 
    {
     Serial.println("connected");
     lcd.clear();
     lcd.setCursor(1, 0);
     lcd.print("Connected");
//     client.publish("abc","GAUTAM");
    }else{
      Serial.print("failed state ");
      
      Serial.print(client.state());
      delay(2000);
    }
}

  lcd.clear();
  client.subscribe("callbell");
  Serial.println(client.subscribe("callbell")); 
  
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
    a[i] = payload[i];
  }
  String mystring(topic);
   a[length]= '\0';
   
  Serial.println("callback");
  
  if(mystring == "callbell")
  {   
    JsonObject& object = jsonBuffer.parseObject(a);
      lcd.clear();  
       String Sname = object["name"];
       Serial.println("name aya");
       Serial.println(Sname);
      digitalWrite(D0,LOW);
      delay(200);
      digitalWrite(D0,HIGH);
      
      lcd.setCursor(0, 1);
      lcd.print(Sname);
//      lcd.print("gautam");  
  } 
   
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("connecting");
     String clientId = "ESP8146228067";
    clientId += String(random(0xffff),HEX);
    //clientId.c_str() 
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqttUser, mqttPassword)) {
      Serial.println("connected");
      lcd.clear();
      client.subscribe("callbell");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void loop()
{ 

    if(WiFi.status() !=  WL_CONNECTED  ){
      Serial.println("wifi not connected");
      if(restartvalue == true){
        restartvalue == false;
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("esp restart ");
        Serial.println("esp restart");
        ESP.restart();  
      }
      
      
    } else {
      if (!client.connected()) {
          reconnect();
        }
        restartvalue = true;
        Serial.println("wifi connected");
        client.loop();
        client.subscribe("callbell");
        lcd.setCursor(1, 0);
        lcd.print("Last call from ");
        client.publish("bellstatus" ,"true");
        delay(500);
    }
}
