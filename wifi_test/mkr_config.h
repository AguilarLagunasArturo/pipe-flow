#include <SPI.h>
#include <WiFiNINA.h>

#define ssid "INFINITUM2372_2.4"
#define pass "uH0L0Ybioa"
int keyIndex  = 0;

int status = WL_IDLE_STATUS;
WiFiServer server(80);

void wifi_setup() {
  // Busca el modulo WiFi
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("Modulo WiFi no existe.");
    while (true);
  }

  // Intenta conectar a WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Conectando a: ");
    Serial.println(ssid);

    // Conecta a WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
    // Espera 10 s por conexion.
    delay(10000);
  }
  server.begin();     // Servidor web en port 80
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");

  // print where to go in a browser:
  Serial.print("To see this page: http://");
  Serial.println(ip);
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
          client.println("<body><center><h1>Pipe Flow.</h1></center></body>");
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
