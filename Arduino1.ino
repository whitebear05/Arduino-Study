int i;
void setup() {
  pinMode(3, OUTPUT);
}

void loop() {
  for(i = 0; i < 256; i++){
    analogWrite(3, i);
    delay(5);
  }
  for(i = 255; i >= 0; i--){
    analogWrite(3, i);
    delay(5);
  }
}
