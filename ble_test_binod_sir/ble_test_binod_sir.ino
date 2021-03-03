#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <EEPROM.h>
#include <soc.h>
#include <rtc_cntl_reg.h>
#include <string.h>
#include <stdlib.h>

#include <BLE2902.h>


#define SERVICE_UUID        "497cb1f5-5703-4ca6-8dd8-5cd7230af887"
#define CHARACTERISTIC_UUID_LENGTH "b8b5fe4f-0228-4250-a56c-f36d73397960"
#define CHARACTERISTIC_UUID_WEIGHT "2bc693c9-b68b-4e4d-8a65-80c980fa4c23"
#define CHARACTERISTIC_UUID_WAIST "9907b924-1686-49c0-afe9-ab809949ff05"
const int trigPin = 14;
const int echoPin = 12;

bool deviceConnected = false;
BLECharacteristic* pCharacteristic_length = NULL;
BLECharacteristic* pCharacteristic_weight = NULL;
BLECharacteristic* pCharacteristic_waist = NULL;
long duration;
int distance;
int height;
int weight,waist,length1;

void setHeight(){

  Serial.print("Distance ");
  Serial.println(distance );

//  String str(distance);
   
  
}

//void calculateHeight(){
//  
//  String finalheight = "Stoped";
//  Serial.print("person height is ");
//  Serial.println(finalheight);
//
//     char buf[5];
//   itoa(finalheight, buf, 10);
//  
//  if(deviceConnected){
//      pCharacteristic->setValue(buf); 
//  }
//  
//}


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

  pCharacteristic_length  =  pService->createCharacteristic(
                                 CHARACTERISTIC_UUID_LENGTH,
                                 BLECharacteristic::PROPERTY_READ
                               );

 pCharacteristic_weight  =  pService->createCharacteristic(
                                 CHARACTERISTIC_UUID_WEIGHT,
                                 BLECharacteristic::PROPERTY_READ
                               );

 pCharacteristic_waist  =  pService->createCharacteristic(
                                 CHARACTERISTIC_UUID_WAIST,
                                 BLECharacteristic::PROPERTY_READ
                               );

  pCharacteristic_length->setCallbacks(new MyCallbacks());
  pCharacteristic_waist->setCallbacks(new MyCallbacks());
  pCharacteristic_weight->setCallbacks(new MyCallbacks());
  
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();



  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);

  
}


void loop() {
  // put your main code here, to run repeatedly:

    for(int i = 0; i < 10 ;i++){
    distance = i;
    
    weight = (i+5);
    length1 = (i+100);
    waist = i;
    delay(1000);
//
//    hex_length = String(length,HEX);
//    Serial.println(hex_length+"length");
//    hex_weight = String(weight,HEX);
//    Serial.println(hex_weight+"Weight");
//    hex_waist = String(waist,HEX);
    
    char len[10];
    char wei[10];
    char wat[10];
    itoa(length1, len, 10);
    itoa(weight, wei, 10);
    itoa(waist, wat, 10);
//    Serial.println(len+"buffer kength");
   
    if(deviceConnected){
       pCharacteristic_length->setValue(len);
       pCharacteristic_weight->setValue(wei);
       pCharacteristic_waist->setValue(wat); 
    }
  }

}
