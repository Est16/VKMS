#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <stdio.h>
#include <Servo.h>

int status = WL_IDLE_STATUS;
char ssid[] = "MiRedWifi"; //  El nombre de tu red wifi
char pass[] = "MiClave";    // la clave de tu red wifi

char convBuffer[4];
int valServo =0;
char packetBuffer[20]; // buffer para guardar los datos entrantes
unsigned int localPort = 1500;  // numero del puerto de la placa para recibir mensajes
 
Servo myservo; //nombre de la instancia servo
WiFiUDP Udp; //nombre de la instancia UDP

void setup() {
   myservo.attach(5);  //el servo esta conectado en el pin 5
  //Inicia el puerto serial y espera hasta que se complete:
  Serial.begin(9600);
  while (!Serial) {
    ; // espera la conexión del puerto serie. Solo es necesario para placas con puerto USB nativo
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

    // Si hay datos disponibles, lee el paquete
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    Serial.print("Tamaño del paquete recibido ");
    Serial.println(packetSize);
    Serial.print("De ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", puerto ");
    Serial.println(Udp.remotePort());

    // lee el paquete y lo guarda en el buffer
    int len = Udp.read(packetBuffer, 20);  //lee el paquete y lo guarda en el buffer
    if (len > 0) packetBuffer[len] = 0;  // coloca el caracter null al final de la cadena
    Serial.println("Contents:");
    Serial.println(packetBuffer);
    
     convBuffer[0]=packetBuffer[7];
    convBuffer[1]=packetBuffer[8];
    convBuffer[2]=packetBuffer[9];
    convBuffer[3]=packetBuffer[10];
    convBuffer[4]=0;
    valServo=atoi(convBuffer); //convierte la cadena de caracteres a entero

    Serial.print("El valor extraido es:");
    Serial.println(valServo);
    
    myservo.write(valServo);    
  }
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

    
