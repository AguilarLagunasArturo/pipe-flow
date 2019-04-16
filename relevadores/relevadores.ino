#define relevadores 2
int relevador[relevadores] = {4, 5};
boolean high = false;

void setup() {
  Serial.begin(9600);
  for(int i = 0; i < relevadores; i++){
    pinMode(relevador[i], OUTPUT);
  }
}

void loop() {
  delay(2000);
  if (high){
    digitalWrite(relevador[0], LOW);
    digitalWrite(relevador[1], LOW);
    Serial.println("open");
    high = false;
  }else{
    digitalWrite(relevador[0], HIGH);
    digitalWrite(relevador[1], HIGH);
    Serial.println("close");
    high = true;
  }
}
