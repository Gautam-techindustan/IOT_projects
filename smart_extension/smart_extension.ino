#define BLYNK_PRINT Serial
// Libraries
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp8266.h>

// WiFi network
ESP8266WebServer server ( 80 );


int mod = 0;
int state = 0;


int blystate=0;
int bl=1;
int resetsmarthome=0;

const char *ssid = "Smarthome";
const char *password = "Smarthome";

char auth[] = "2f61d44d154c4e4ba56f50c3e1a05a92";

String wifi;
String pass;
String wifiarr;
String passarr;

char htmlResponse[3000];




void handleRoot()
  {
     server.send(200, "text/html", "<html><body><form  name='frm'  method='post' action='/save'><input type='text' name='userwifi'   >Wifi<input type='text' name='password'   >Password<input type='submit' value='Submit'></form></body></html>");
  }


void handleSave()
  {
      if (server.arg("userwifi")!= ""){
          Serial.println("Wifi Name " + server.arg("userwifi"));
      }
    
      if (server.arg("password")!= ""){
          Serial.println("Wifi Password: " + server.arg("password"));
      }
    
        if (server.arg("password")!= "" && server.arg("userwifi")!= ""){
              wifi=server.arg("userwifi");
              pass = server.arg("password");
              int k = wifi.length()+3;
              EEPROM.write(1,wifi.length());
              EEPROM.write(2,pass.length());
         
        for(int i=0 ; i<wifi.length() ; i++){
           EEPROM.write(i+3,wifi[i]); 
        }
       
        for(int j=0 ; j<pass.length() ; j++){
            EEPROM.write(k+j,pass[j]);
        }
    
        EEPROM.commit();
      }
      handleRoot();
  }

void newWifiConnect()
    {
       if(wifiarr != server.arg("userwifi") && server.arg("userwifi").length())
        {
                char wifiarray[EEPROM.read(1)];
                char passarray[EEPROM.read(2)];
                for(int i=0 ; i<EEPROM.read(1) ; i++)
                   {
                     wifiarray[i]=EEPROM.read(i+3);
                   }
                wifiarray[EEPROM.read(1)] = '\0';
                Serial.println("New Wifi");
                Serial.println(wifiarray);
           
                for(int i=0 ; i<EEPROM.read(2) ; i++)
                   {
                     passarray[i]=EEPROM.read((i+EEPROM.read(1))+3);
                   }
                     
                     
                passarray[EEPROM.read(2)] = '\0'; 
                      
                 Serial.println( passarray);
                 wifiarr=wifiarray;
                 passarr=passarray;
                 WiFi.disconnect();
                       
                 WiFi.begin(wifiarray,passarray);
                 while (WiFi.status() != WL_CONNECTED) 
                      {
                        delay(500);
                        Serial.print(".");
                      }
            
                 Serial.println("");
                 Serial.println("New WiFi connected");  
                 Serial.println("IP address: ");
                 Serial.println(WiFi.localIP());
                 Blynk.begin(auth,wifiarray,passarray);
        }
    } 

void resetpassword()
        {
          
//        snprintf ( htmlResponse, 3000,
//                    "<!DOCTYPE html>\
//                    <html lang=\"en\">\
//                      <head>\
//                        <meta charset=\"utf-8\">\
//                        <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
//                      </head>\
//                      <body>\
//                              <h1>Time</h1>\
//                              <input type='text' name='date_hh' id='date_hh'  autofocus> Wifi \
//                              <input type='text' name='date_mm' id='date_mm'  autofocus> password \
//                              <div>\
//                              <br><button id=\"save_button\">Save</button>\
//                              </div>\
//                        <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js\"></script>\    
//                        <script>\
//                          var userwifi;\
//                          var password;\
//                          $('#save_button').click(function(e){\
//                            e.preventDefault();\
//                            userwifi = $('#date_hh').val();\
//                            password = $('#date_mm').val();\        
//                            $.get('/save?hh=' + userwifi + '&mm=' + password , function(data){\
//                              Serial.println(data);\
//                            });\
//                          });\      
//                        </script>\
//                      </body>\
//                    </html>"); 
//      
//         server.send ( 200, "text/html", htmlResponse );   

         server.send(200, "text/html", "<html><body><form  name='frm'  method='post' action='/save'><h1>Reset password</h1><input type='text' name='userwifi'   >Wifi<input type='text' name='password'   >Password<input type='submit' value='Submit'></form></body></html>");

        }


void clearmemory(){
//  
//      Serial.println("D8 input are");
     
//     
     
    if(digitalRead(D8)==1)
          {
            
            resetsmarthome++;
            delay(200);
            Serial.print("resetsmarthome are :");
            Serial.println(resetsmarthome);
            if(resetsmarthome == 20)
              {
                for (int i = 0 ; i < EEPROM.length() ; i++)
                    {
                      EEPROM.write(i, 0);
                      
                    }
                    EEPROM.commit();
                Serial.println("cleared all memory");
              }
          }
          else
          {
            resetsmarthome = 0;
          }
  
}



void runapp(){
//  clearmemory();
      mod = digitalRead(D8);
      if(mod == 1 ) {
        if(state == 0)
          state = 1;
        else
          state = 0;
    
      EEPROM.write(0, state);
      EEPROM.commit();
      delay(250);
      } 
//   Serial.println("runapp wifi");
//   Serial.println(wifi);
//    Serial.println("runapp wifi1111111111");
//    Serial.println(wifiarr);
      if(WiFi.status() ==  WL_CONNECTED && wifiarr.length() )
        {
          digitalWrite(D4,HIGH);
           
           if(state==1)
            {
                //Serial.print("D8=1");
                digitalWrite(D1,LOW);
                digitalWrite(D2,LOW);
                digitalWrite(D3,LOW);
                digitalWrite(D0,LOW);
                digitalWrite(D5,LOW);
                digitalWrite(D6,LOW);
                digitalWrite(D7,LOW);
                digitalWrite(D10,HIGH);
                blystate=0;
            }
    
            else
            {
              if(blystate==0 && state==0)
               {
                Serial.println("HElllo can i change");
                digitalWrite(D1,HIGH);
                digitalWrite(D2,HIGH);
                digitalWrite(D3,HIGH);
                digitalWrite(D0,HIGH);
                digitalWrite(D5,HIGH);
                digitalWrite(D6,HIGH);
                digitalWrite(D7,HIGH);
                digitalWrite(D10,LOW);
                  char blynkwifi[wifiarr.length()];
                  char blynkpass[passarr.length()];
                  Serial.print("Blynk chla");
                  
                   for (int i=0;i<wifiarr.length() ;i++)
                     {
                        blynkwifi[i] = wifiarr[i]; 
                     }
                    blynkwifi[EEPROM.read(1)] = '\0';
                   
                   for (int i=0;i<passarr.length() ;i++)
                     {
                        blynkpass[i] = passarr[i]; 
                     }
                    blynkpass[EEPROM.read(2)] = '\0';
                    Serial.println("Going for connect to blink connected");
                    Serial.println(blynkwifi);
                    Serial.println(blynkpass);
                    Serial.println(" ");
                   Blynk.begin(auth, blynkwifi, blynkpass);
                   Serial.println("Blink connected");
                  blystate=1;
                }
               Blynk.syncAll();
               Serial.println("Blink running");
               Blynk.run();
    //            clearmemory();
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


  void setup() {
    
    // Start serial
    Serial.begin(9600);
    EEPROM.begin(512);
    Serial.println(".");
//    WiFi.disconnect();
//    
//    Serial.println("Why i am not running");
//      for (int i = 0 ; i < EEPROM.length() ; i++)
//                    {
//                      EEPROM.write(i, 0);
//                      EEPROM.commit();   
//                    }

     Serial.println("I have cleared All Memory" );
     Serial.println("Wifi Length If Exist");
     Serial.println(EEPROM.read(1));

     
     Serial.println("Password Length If Exist");
     Serial.println(EEPROM.read(2));
     
                   

    delay(10); 
    
  
    // Connecting to a WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
   
  

   char wifiarray[EEPROM.read(1)];
   char passarray[EEPROM.read(2)];
       for(int i=0 ; i<EEPROM.read(1) ; i++)
           {
            wifiarray[i]=EEPROM.read(i+3);
           }
       wifiarray[EEPROM.read(1)] = '\0';
        
     Serial.println(wifiarray);

       for(int i=0 ; i<EEPROM.read(2) ; i++)
           {
            passarray[i]=EEPROM.read((i+EEPROM.read(1))+3);
           }
     
      passarray[EEPROM.read(2)] = '\0'; 
            
     Serial.println( passarray);
     Serial.println("Chla rha h?");


     if(EEPROM.read(1) && EEPROM.read(2))
          {
            wifiarr=wifiarray;
            passarr=passarray;
            Serial.println("wifi connect hua?");
            WiFi.begin(wifiarray,passarray);
            Serial.println("Yes hua");
            Serial.println(wifiarray);
//            Serial.println("blynk connect hua?");
//            Blynk.begin(auth, wifiarray, passarray);
//            Serial.println("Yes hua blynk ");
            server.on("/reset",resetpassword);
            server.on ("/save", handleSave);
          }
     else
          {
    //        WiFi.begin(ssid, NULL);
              WiFi.softAP(ssid, password);        
              IPAddress myIP = WiFi.softAPIP();
              Serial.print("AP IP address: ");
              Serial.println(myIP);
              server.on ( "/", handleRoot );
              server.on ("/save", handleSave);
              
          }
        
      server.begin();
      Serial.println ( "HTTP server started" );

      pinMode(D1,OUTPUT);
      pinMode(D2,OUTPUT);
      pinMode(D3,OUTPUT);
      pinMode(D4,OUTPUT);
      pinMode(D0,OUTPUT);
      pinMode(D5,OUTPUT);
      pinMode(D6,OUTPUT);
      pinMode(D7,OUTPUT);
//      pinMode(D10, OUTPUT);
      digitalWrite(D1,HIGH);
      digitalWrite(D2,HIGH);
      digitalWrite(D3,HIGH);
      digitalWrite(D4,LOW);
      digitalWrite(D0,HIGH);
      digitalWrite(D5,HIGH);
      digitalWrite(D6,HIGH);
      digitalWrite(D7,HIGH);
      digitalWrite(D10, LOW);
//      digitalWrite(10,LOW);
      pinMode(D8,INPUT);
        
        
     Serial.println("Pin Mode Chle");
      }


void loop() 
    {
        newWifiConnect();

        server.handleClient();
        runapp();

    }
