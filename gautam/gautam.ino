#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "nodemcu-30211.firebaseio.com"
#define FIREBASE_AUTH "ktD8GtlUI3Gu7Go7X0Of5hezwPUn3Sj2SOb2V5Xf"




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

  
Serial.println("connect?");
Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
Firebase.stream("/buttons"); 
Serial.println("firebase connected?");

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
         FirebaseObject yes = Firebase.readEvent();
         String token = yes.getString("/buttons");
         Serial.println("direct not ");
          if(token){
            
            FirebaseObject obj = Firebase.get("/buttons");
            Serial.println("firebaseee ");
            
      
            
            int bell = obj.getInt("switch1");
             digitalWrite(D1, !bell);
            int bell1 = obj.getInt("switch2");
              digitalWrite(D2, !bell1);
            int bell2 = obj.getInt("switch3");
              digitalWrite(D3,!bell2);
                Serial.println(bell);
             digitalWrite(D10,LOW);
        
          }
            
       

     

     

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
