
bool state = false;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(D0,INPUT);
  pinMode(D6,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  int sum = 0;
  for(int i=0; i<100; i++) {
    sum += digitalRead(D0);  
  }

  sum = sum/100;
  
  
  if(sum > 0){
      if(state)
        state = false;
      else
        state = true;
      delay(400);
      digitalWrite(D6, state);
      
  }

  Serial.println(sum);

}
