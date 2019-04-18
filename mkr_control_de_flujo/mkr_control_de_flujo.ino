/*
  Arduino Mega 2560
  Sensor de flujo YF-S201
*/
// Librerias
#include "mkr_wifi_config.h"

// Constantes
#define sensores 7
#define relevadores 7

// Pines
int sensor_flujo[sensores] = { 0,  1,  5,  6,  7,  8,  9};
int relevador[relevadores] = { 2,  3, 10, 11, 12, 20, 21};

// Limites de cada sensor
unsigned long gasto_minimo[sensores] = {
  0, 0, 0, 0, 0, 0, 0
};

// Variables
volatile int frecuencia[sensores];       // Pulsos obtenidos del sensor
unsigned long gasto[sensores];           // Litros por minuto
unsigned long tiempo_actual;
unsigned long tiempo_auxiliar;

String data = "";                              // Datos a graficar

// Funciones "Interrupt"
void flujo_0() {
  frecuencia[0] ++;
}
void flujo_1() {
  frecuencia[1] ++;
}
void flujo_2() {
  frecuencia[2] ++;
}
void flujo_3() {
  frecuencia[3] ++;
}
void flujo_4() {
  frecuencia[4] ++;
}
void flujo_5() {
  frecuencia[5] ++;
}
void flujo_6() {
  frecuencia[6] ++;
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
  pinMode(sensor_flujo[6], INPUT);
  //digitalWrite(sensor_flujo, HIGH);
  attachInterrupt(digitalPinToInterrupt(sensor_flujo[6]), flujo_6, RISING);
  //sei(); // Permite "Interrupts"

  // Se inicializan los pines que controlaran los relevadores
  for (int i = 0; i < relevadores; i++) {
    pinMode(relevador[i], OUTPUT);
  }

  tiempo_actual = millis();
  tiempo_auxiliar = tiempo_actual;

  // Se esperan 5s para que el agua fluya por la tuberia.
  Serial.println("Esperando ...");
  delay(5000);
  // Se inicializa el servidor web
  wifi_setup();
  print_wifi_status();
}

void loop() {
  tiempo_actual = millis();

  // Calcula el flujo (L/min) cada segundo
  if (tiempo_actual >= (tiempo_auxiliar + 1000)) {
    tiempo_auxiliar = tiempo_actual;

    // Calcula el gasto por cada sensor
    for (int i = 0; i < sensores; i++) {
      gasto[i] = frecuencia[i] / 7.5;

      // Si necesario, cerrar electrovalvulas
      if (gasto[i] < gasto_minimo[i] || gasto[i] > 30) {
        cerrar_valvula(i, true);
        Serial.println("Valvula " + String(i + 1) + " cerrada.");
      }
      // Imprime con formato: Seccion | Hz | L/min
      Serial.println("Sensor: " + String(i + 1) + " | Hz: " + frecuencia[i] + " | " + String(gasto[i]) + " L/min");
      data += i == sensores - 1 ? String(gasto[i]) : String(gasto[i]) + ",";
      frecuencia[i] = 0;
    }

    // Serial.println(data);
    data = "";
  }
  create_web_page();
}

void cerrar_valvula(int id, boolean cerrar) {
  // Si se quiere abrir enviar LOW sino HIGH
  cerrar ? digitalWrite(relevador[id], HIGH) : digitalWrite(relevador[id], LOW);
}

void create_web_page() {
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          // html code
          client.println("<body>");
          client.println("<center><h1>Pipe Flow.</h1></center>");
          for (int i = 0; i < sensores; i++) {
            client.println("<h3>Gasto en sensor "+ String(i+1) +": "+ String(gasto[i]) +"</h3>");
          }
          client.println("</body>");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);

    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}
