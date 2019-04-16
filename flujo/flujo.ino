/*
  Arduino sensor de flujo YF-S201
*/
// Constantes
#define sensores_total 2

// Variables
int sensor_flujo[sensores_total] = {2, 3};
volatile int frecuencia[sensores_total];       // Pulsos obtenidos del sensor
unsigned long gasto[sensores_total];           // Litros por minuto
unsigned long tiempo_actual;
unsigned long tiempo_auxiliar;

// Funcion "Interrupt"
void flujo_0(){
  frecuencia[0] ++;
}
void flujo_1(){
  frecuencia[1] ++;
}

void setup() {
  Serial.begin(9600);
  
  pinMode(sensor_flujo[0], INPUT);
  //digitalWrite(sensor_flujo, HIGH);
  attachInterrupt(digitalPinToInterrupt(sensor_flujo[0]), flujo_0, RISING);
  pinMode(sensor_flujo[1], INPUT);
  //digitalWrite(sensor_flujo, HIGH);
  attachInterrupt(digitalPinToInterrupt(sensor_flujo[1]), flujo_1, RISING);
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
      // Seccion,Hz,L/min
      Serial.println("Sensor: " + String(i+1)+ "\t|\t" + String(gasto[i]) + " L/min");
      frecuencia[i] = 0;
    }
  }
}
