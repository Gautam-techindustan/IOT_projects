int latchPin = 8;
int clockPin = 12;
int dataPin = 11;
byte data = 0b00000000;

void setup() {
  // put your setup code here, to run once:
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, data);
  digitalWrite(latchPin, HIGH);

  delay(250);

  bitSet(data, 1);
  bitSet(data, 2);
  bitSet(data, 3);

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, data);
  digitalWrite(latchPin, HIGH);

  delay(250);

  bitClear(data, 1);
  bitClear(data, 2);
  bitClear(data, 3);
}
