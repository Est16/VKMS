#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>

int status = WL_IDLE_STATUS;
char ssid[] = "MiFibra-2F43"; //  El nombre de tu red wifi
char pass[] = "qz4x3a7p";    // la clave de tu red wifi

unsigned int localPort = 2390; // numero del puerto de la placa para recibir mensajes

IPAddress ipremoto(192, 168, 1, 131);    //direccion IP de placa remota
unsigned int puertoRemoto = 1500; //numero de puerto de la placa remota

char  msg[20];  //Matriz de caracteres 

WiFiUDP Udp;  //nombre de la instancia

void setup() {
  //Inicia el puerto serial y espera hasta que se complete:
  Serial.begin(9600);
  while (!Serial) {
    ; // espera la conexion del puerto serie. Solo es necesario para placas con puerto USB nativo
      // Remover este while si la placa no estara conectada al ordenador
  }

  // revision de la presencia del shield de comunicación:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("El shield WiFi no esta presente");
    // no continua:
    while (true);
  }
  
  // intenta conectar a la red wifi:
  while ( status != WL_CONNECTED) {
    Serial.print("Conectando a SSID: ");
    Serial.println(ssid);
    // Conecta a una red WPA/WPA2:
    status = WiFi.begin(ssid, pass);
    // espera 10 segundos por la coneccion:
    delay(10000);
  }
  Serial.println("Conectado a wifi");
  printWiFiStatus();
  
  
  Udp.begin(localPort); // Inicia el socket WiFiUDP, y escucha el puerto local
}

void loop() {

int valorPot = analogRead(A1); //leo el valor del potenciometro
    sprintf(msg, "ValPot:%i", valorPot); // Crea una cadena de texto con el valor del potenciometro
    Serial.println(msg); //Imprimo la cadena de texto al monitor serie
    
    Udp.beginPacket(ipremoto, puertoRemoto); //configura el envio del paquete de datos
    Udp.write(msg); //coloca en el buffer los datos de salida
    Udp.endPacket(); //envia el paquete de datos

   delay(500);
}

  void printWiFiStatus() {
  // imprime el nombre de la red a la cual estas conectado:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // imprime la direcion IP del shield o placa con la cual estas conectado:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  
  // imprime la intensidad de señal recibida:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


  
