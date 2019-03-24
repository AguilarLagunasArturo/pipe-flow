/*
Arduino sensor de flujo YF-S201
*/
// Constantes
#define sensor_flujo 2
// Variables
volatile int frecuencia;       // Pulsos obtenidos del sensor
unsigned long gasto;           // Litros por minuto
unsigned long tiempo_actual;
unsigned long tiempo_auxiliar;

// Funcion "Interrupt"
void flujo(){
  frecuencia ++;
}

void setup() {
  Serial.begin(9600);
  
  pinMode(sensor_flujo, INPUT);
  digitalWrite(sensor_flujo, HIGH);
  attachInterrupt(0, flujo, RISING);
  sei(); // Permite "Interrupts"
  tiempo_actual = millis();
  tiempo_auxiliar = tiempo_actual;
}

void loop() {
  tiempo_actual = millis();
  // Calcula el flujo (L/min) cada segundo
  if (tiempo_actual >= (tiempo_auxiliar +1000)){
    tiempo_auxiliar = tiempo_actual;
    gasto = frecuencia / 7.5;
    Serial.println(String(frecuencia) + "Hz\t" + String(gasto) + "L/m");
    frecuencia = 0;
  }
}
