#define relevadores 7
int relevador[relevadores] = { 5,  6,  7,  9, 10, 11};
boolean high = true;
int i = 0;

void setup() {
  Serial.begin(9600);
  for(int i = 0; i < relevadores; i++){
    pinMode(relevador[i], OUTPUT);
  }
}

void loop() {
  delay(2000);
  if (high){
    digitalWrite(relevador[i], LOW);
    Serial.println("open");
  }else{
    digitalWrite(relevador[i], HIGH);
    Serial.println("close");
  }
  i++;
  if (i ==7) {
    i = 0;
    high = !high;
  }
}
