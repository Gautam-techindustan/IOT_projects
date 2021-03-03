#include <MultiShiftRegister.h>

int latchPin  = D1;
int clockPin = D2;
int dataPin = D3;

// How many 8 bit shift registers we're talking to.
// Two 8 bit registers can host 16 individually
// addressable binary outputs.
int numberOfRegisters = 2;

// Setup group of shift registers and name it `msr`.
// All outputs are initialized to low/off.
MultiShiftRegister msr (numberOfRegisters , latchPin, clockPin, dataPin); 

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  for(int i=0; i<8; i++) {
    msr.set_shift(i);
  }
}

void loop() {
 

  // This set and shift business is getting repetitive.
  // Let's do it in one line instead.
  for(int i=0; i<8; i++) {
    msr.set_shift(i);
    delay(1000);
    msr.clear_shift(i);
    delay(1000);
  }

  //delay(1000);
}
