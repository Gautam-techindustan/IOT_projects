#include "BluetoothSerial.h"
#include <soc.h>
#include <rtc_cntl_reg.h>
 
BluetoothSerial SerialBT;
 
void setup() {
  Serial.begin(9600);
 
  if(!SerialBT.begin("ESP3")){
    Serial.println("An error occurred initializing Bluetooth");
  }
}
 
void loop() {
 
  while(SerialBT.available()){
    Serial.write(SerialBT.read());
    Serial.println(SerialBT.read());

  }
 
  delay(50);
}
