
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <EEPROM.h>
#include <soc.h>
#include <rtc_cntl_reg.h>
#include <string.h>
#include <stdlib.h>

#include <BLE2902.h>


#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

const int trigPin = 14;
const int echoPin = 12;

bool deviceConnected = false;
BLECharacteristic* pCharacteristic = NULL;

long duration;
int distance;
int height;


void setHeight(){

  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);

  duration = pulseIn(echoPin,HIGH);
  distance = duration*0.034/2;

  Serial.print("Distance ");
  Serial.println(distance );

//  String str(distance);
   char buf[5];
   itoa(distance, buf, 10);

  Serial.println(buf);
   
  if(deviceConnected){
     pCharacteristic->setValue(buf); 
  }
  
}

void calculateHeight(){
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);

  duration = pulseIn(echoPin,HIGH);
  height = duration*0.034/2;

  int finalheight = distance-height;
  Serial.print("person height is ");
  Serial.println(finalheight);

     char buf[5];
   itoa(finalheight, buf, 10);
  
  if(deviceConnected){
      pCharacteristic->setValue(buf); 
  }
  
}


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      char a[value.length()];
      if (value.length() > 0) {
        Serial.println("*********");
        Serial.print("New value: ");
        for (int i = 0; i < value.length(); i++){
          Serial.print(value[i]);
          a[i] =value[i];
        }
        Serial.println(" ");
       a[value.length()] ='\0';
        
       String str(a);
       Serial.print("str");
       Serial.println(str);
       
        if(str == "set"){
          setHeight();  
        }

        if(str == "hi"){
          calculateHeight();
        }
        
                
        Serial.println();
        Serial.println("*********");
      }
    }
};

void setup() {
  

  Serial.begin(9600);
  EEPROM.begin(512);
  
  

  Serial.println("2- Scan for BLE devices in the app");
  Serial.println("3- Connect to MyESP32");

//  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG,0);


  BLEDevice::init("MyESP32");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic  =  pService->createCharacteristic(
                                 CHARACTERISTIC_UUID,
                                 BLECharacteristic::PROPERTY_READ |
                                 BLECharacteristic::PROPERTY_WRITE |
                                 BLECharacteristic::PROPERTY_NOTIFY
                               );

  pCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristic->setValue("Hello World");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();



  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);

  
}


void loop() {
  // put your main code here, to run repeatedly:

}
