
bool state1 = false;
bool state2 = false;
bool state3 = false;
bool state4 = false;
bool state5 = false;
bool state6 = false;
bool state7 = false;
bool state8 = false;
bool state9 = false;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  //pinMode(D4,INPUT);
  pinMode(23, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(5,OUTPUT);
//  pinMode(18,OUTPUT);
//  pinMode(19,OUTPUT);
//  pinMode(21,OUTPUT);
//  pinMode(26,OUTPUT);
//  pinMode(25,OUTPUT);
//  pinMode(35,OUTPUT);
//  pinMode(34,OUTPUT);
  
}

void passCurrentToSwitch(int input , bool state , int pinnum){
  Serial.begin(9600);
//  Serial.println("input are :=" + input);
  if (input < 20) {
    if (state)
      state = false;
    else
      state = true;
    delay(400);
    digitalWrite(23, state);

  }
}


void loop() {
  // put your main code here, to run repeatedly:

  int input1 = 0;
  int input2 = 0;
  int input3 = 0; 
  int input4 = 0;
  int input5 = 0;
  int input6 = 0;
  int input7 = 0;
  int input8 = 0;
  int input9 = 0;
 
  for (int i = 0; i < 10; i++) {
    input1 += touchRead(T0);
    input2 += touchRead(T2);
    input3 += touchRead(T3);
    input4 += touchRead(T4);
    input5 += touchRead(T5);
    input6 += touchRead(T6);
    input7 += touchRead(T7);
    input8 += touchRead(T8);
    input9 += touchRead(T9);
  }

  input1 = input1 / 10;
  input2 = input2 / 10;
  input3 = input3 / 10;
  input4 = input4 / 10;
  input5 = input5 / 10;
  input6 = input6 / 10;
  input7 = input7 / 10;
  input8 = input8 / 10;
  input9 = input9 / 10;

//  passCurrentToSwitch(input1,state1,23);
//  passCurrentToSwitch(input2,state2,22);
//  passCurrentToSwitch(input3,state3,5);
//  passCurrentToSwitch(input4,state4,18);
//  passCurrentToSwitch(input5,state5,19);
//  passCurrentToSwitch(input6,state6,21);
//  passCurrentToSwitch(input7,state7,26);
//  passCurrentToSwitch(input8,state8,25);
//  passCurrentToSwitch(input9,state9,35);

  if (input1 < 20) {
    state1 = !state1;
    delay(400);
    digitalWrite(23, state1);

  }
//  if (input2 < 20) {
//    if (state2)
//      state2 = false;
//    else
//      state2 = true;
//    digitalWrite(22, state2);
//  }
  if (input3 < 20) {
   
    state3 = !state3;
    Serial.println("Here"+state3); 
     delay(400);
    digitalWrite(5, state3);
  }
 
 
  

  Serial.println(input3);

}
