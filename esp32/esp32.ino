/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleWrite.cpp
    Ported to Arduino ESP32 by Evandro Copercini
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <EEPROM.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

int led[] = {12,14,27};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();


//      char a[value.length()];
      if (value.length() > 0) {
        Serial.println("*********");
        Serial.print("New value: ");
        for (int i = 0; i < value.length(); i++){
          Serial.print(value[i]);
//          a[i]=value[i];
          digitalWrite(led[i] , value[i]-48);
          EEPROM.write(i+1, value[i]-48);
        }

            
          EEPROM.commit();
           delay(190);
//        digitalWrite(led[0] , a[0]);
//        digitalWrite(led[1] , a[1]);
//        digitalWrite(led[2] , a[2]);
        
//        a[value.length()]= '\0';
//        String str(a);

//        if(str == "on"){
//          digitalWrite(led , HIGH);
//        }else if(str == "off"){
//          digitalWrite(led , LOW);
//        }
        
        Serial.println();
        Serial.println("*********");
      }
    }
};

void setup() {
  Serial.begin(9600);
  EEPROM.begin(512);

     for(int i = 0; i<3 ; i++){
         EEPROM.write(i+1, 0);
         pinMode( led[i],OUTPUT); 
      }
 
    pinMode( 2,INPUT);
  Serial.println("1- Download and install an BLE scanner app in your phone");
  Serial.println("2- Scan for BLE devices in the app");
  Serial.println("3- Connect to MyESP32");
  Serial.println("4- Go to CUSTOM CHARACTERISTIC in CUSTOM SERVICE and write something");
  Serial.println("5- See the magic =)");

  BLEDevice::init("MyESP32");
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristic->setValue("Hello World");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

int state = 0;

void setInputPin(){
int val = digitalRead(2);
  if(val == 1 ) {
    if(state == 0){
      state = 1;
      for(int i = 0; i<3 ; i++){
        digitalWrite(led[i],HIGH);
      }
    }
      
    else{
      state = 0;
      for(int i = 0; i<3; i++){
//        EEPROM.read(i+1);
        digitalWrite(led[i],EEPROM.read(i+1));
      }
    }
      

    EEPROM.write(0, state);
    EEPROM.commit();
   delay(190);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  setInputPin();

}



//  keyyy
//#include <BLEDevice.h>
//#include <BLEUtils.h>
//#include <BLEServer.h>
//#include <EEPROM.h>
//
//// See the following for generating UUIDs:
//// https://www.uuidgenerator.net/
//
//#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
//#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
//
//int led[] = {12,14,27};
//
//class MyCallbacks: public BLECharacteristicCallbacks {
//    void onWrite(BLECharacteristic *pCharacteristic) {
//      std::string value = pCharacteristic->getValue();
//
//
//      char a[value.length()];
//      char b[value.length()-3];
//      if (value.length() > 0) {
//        Serial.println("*********");
//        Serial.print("New value: ");
//        for (int i = 0; i < value.length(); i++){
//          Serial.print(value[i]);
//          a[i]=value[i];
//        }
//        a[value.length()] = "/0";
//        b[value.length()-3] = "/0";
//        
//        for(int i = 3 ;i < value.length(); i++){
//            b[i-3] = a[i];
//        }
//        String str(b);
//        if(str ==""){
//          for(int i = 0; i < 3; i++){
//            digitalWrite(led[i] , a[i]-48);
//            EEPROM.write(i+1, a[i]-48);
//          }
//        }
//
//          EEPROM.commit();
//           delay(190);
////        digitalWrite(led[0] , a[0]);
////        digitalWrite(led[1] , a[1]);
////        digitalWrite(led[2] , a[2]);
//        
////        a[value.length()]= '\0';
////        String str(a);
//
////        if(str == "on"){
////          digitalWrite(led , HIGH);
////        }else if(str == "off"){
////          digitalWrite(led , LOW);
////        }
//        
//        Serial.println();
//        Serial.println("*********");
//      }
//    }
//};
//
//void setup() {
//  Serial.begin(9600);
//  EEPROM.begin(512);
//
//     for(int i = 0; i<3 ; i++){
//         EEPROM.write(i+1, 0);
//         pinMode( led[i],OUTPUT); 
//      }
// 
//    pinMode( 2,INPUT);
//  Serial.println("1- Download and install an BLE scanner app in your phone");
//  Serial.println("2- Scan for BLE devices in the app");
//  Serial.println("3- Connect to MyESP32");
//  Serial.println("4- Go to CUSTOM CHARACTERISTIC in CUSTOM SERVICE and write something");
//  Serial.println("5- See the magic =)");
//
//  BLEDevice::init("MyESP32");
//  BLEServer *pServer = BLEDevice::createServer();
//
//  BLEService *pService = pServer->createService(SERVICE_UUID);
//
//  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
//                                         CHARACTERISTIC_UUID,
//                                         BLECharacteristic::PROPERTY_READ |
//                                         BLECharacteristic::PROPERTY_WRITE
//                                       );
//
//  pCharacteristic->setCallbacks(new MyCallbacks());
//
//  pCharacteristic->setValue("Hello World");
//  pService->start();
//
//  BLEAdvertising *pAdvertising = pServer->getAdvertising();
//  pAdvertising->start();
//}
//
//int state = 0;
//
//void setInputPin(){
//int val = digitalRead(2);
//  if(val == 1 ) {
//    if(state == 0){
//      state = 1;
//      for(int i = 0; i<3 ; i++){
//        digitalWrite(led[i],HIGH);
//      }
//    }
//      
//    else{
//      state = 0;
//      for(int i = 0; i<3; i++){
////        EEPROM.read(i+1);
//        digitalWrite(led[i],EEPROM.read(i+1));
//      }
//    }
//      
//
//    EEPROM.write(0, state);
//    EEPROM.commit();
//   delay(190);
//  }
//}
//
//void loop() {
//  // put your main code here, to run repeatedly:
//
//  setInputPin();
//
//}
