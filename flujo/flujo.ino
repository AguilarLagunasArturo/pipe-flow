/*
  Arduino sensor de flujo YF-S201
*/
// Constantes
#define sensores_total 6

// Variables
int sensor_flujo[sensores_total] = {2, 3, 18, 19, 20, 21};
volatile int frecuencia[sensores_total];       // Pulsos obtenidos del sensor
unsigned long gasto[sensores_total];           // Litros por minuto
unsigned long tiempo_actual;
unsigned long tiempo_auxiliar;

String data = "";                              // Datos a graficar

// Funciones "Interrupt"
void flujo_0(){
  frecuencia[0] ++;
}
void flujo_1(){
  frecuencia[1] ++;
}
void flujo_2(){
  frecuencia[2] ++;
}
void flujo_3(){
  frecuencia[3] ++;
}
void flujo_4(){
  frecuencia[4] ++;
}
void flujo_5(){
  frecuencia[5] ++;
}

void setup() {
  Serial.begin(9600);
  
  pinMode(sensor_flujo[0], INPUT);
  //digitalWrite(sensor_flujo, HIGH);
  attachInterrupt(digitalPinToInterrupt(sensor_flujo[0]), flujo_0, RISING);
  pinMode(sensor_flujo[1], INPUT);
  //digitalWrite(sensor_flujo, HIGH);
  attachInterrupt(digitalPinToInterrupt(sensor_flujo[1]), flujo_1, RISING);
  pinMode(sensor_flujo[2], INPUT);
  //digitalWrite(sensor_flujo, HIGH);
  attachInterrupt(digitalPinToInterrupt(sensor_flujo[2]), flujo_2, RISING);
  pinMode(sensor_flujo[3], INPUT);
  //digitalWrite(sensor_flujo, HIGH);
  attachInterrupt(digitalPinToInterrupt(sensor_flujo[3]), flujo_3, RISING);
  pinMode(sensor_flujo[4], INPUT);
  //digitalWrite(sensor_flujo, HIGH);
  attachInterrupt(digitalPinToInterrupt(sensor_flujo[4]), flujo_4, RISING);
  pinMode(sensor_flujo[5], INPUT);
  //digitalWrite(sensor_flujo, HIGH);
  attachInterrupt(digitalPinToInterrupt(sensor_flujo[5]), flujo_5, RISING);
  
  sei(); // Permite "Interrupts"
  tiempo_actual = millis();
  tiempo_auxiliar = tiempo_actual;
}

void loop() {
  tiempo_actual = millis();
  // Calcula el flujo (L/min) cada segundo
  if (tiempo_actual >= (tiempo_auxiliar +1000)){
    tiempo_auxiliar = tiempo_actual;
    for(int i = 0; i < sensores_total; i++){
      gasto[i] = frecuencia[i] / 7.5;
      // Seccion | Hz | L/min
      Serial.println("Sensor: " + String(i+1)+ " | Hz: " + frecuencia[i]+ " | " + String(gasto[i]) + " L/min");
      data += i == sensores_total-1 ? String(gasto[i]) : String(gasto[i]) + ",";
      frecuencia[i] = 0;
    }
    // Serial.println(data);
    data = "";
  }
}
