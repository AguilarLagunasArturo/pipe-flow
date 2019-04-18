/*
  Arduino Mega 2560
  Sensor de flujo YF-S201
*/
// Constantes
#define sensores 6
#define relevadores 6

// Pines
int sensor_flujo[sensores] = { 2,  3, 18, 19, 20, 21};
int relevador[relevadores] = { 4,  5,  6,  7,  8,  9};

// Limites de cada sensor
float gasto_minimo[sensores] = {
  -1, -1, -1, -1, -1, -1  
};

// Variables
volatile int frecuencia[sensores];       // Pulsos obtenidos del sensor
float gasto[sensores];           // Litros por minuto
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
  // Se inicializan los pines que utilizaran los sensores de gasto.
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
  //sei(); // Permite "Interrupts"
  
  // Se inicializan los pines que controlaran los relevadores
  for (int i = 0; i < relevadores; i++){
    pinMode(relevador[i], OUTPUT);
  }
  
  tiempo_actual = millis();
  tiempo_auxiliar = tiempo_actual;

  // Se esperan 5s para que el agua fluya por la tuberia.
  Serial.println("Esperando ...");
  delay(10000);
}

void loop() {
  tiempo_actual = millis();
  
  // Calcula el flujo (L/min) cada segundo
  if (tiempo_actual >= (tiempo_auxiliar + 1000)){
    tiempo_auxiliar = tiempo_actual;

    // Calcula el gasto por cada sensor
    for(int i = 0; i < sensores; i++){
      gasto[i] = float(frecuencia[i]) / 7.5;
      
      // Si necesario, cerrar electrovalvulas
      if (gasto[i] <= gasto_minimo[i] || gasto[i] >= 30){
        cerrar_valvula(i, true);
        Serial.println("Valvula " + String(i+1) + " cerrada.");
      }else {
        // Imprime con formato: Seccion | Hz | L/min
        Serial.println("Sensor: " + String(i+1)+ " | Hz: " + frecuencia[i]+ " | " + String(gasto[i]) + " L/min");
        data += i == sensores-1 ? String(gasto[i]) : String(gasto[i]) + ",";
        frecuencia[i] = 0;
      }
    }
    
    // Serial.println(data);
    data = "";
  }
}

void cerrar_valvula(int id, boolean cerrar){
  // Si se quiere abrir enviar LOW sino HIGH
  cerrar ? digitalWrite(relevador[id], HIGH) : digitalWrite(relevador[id], LOW);
}
