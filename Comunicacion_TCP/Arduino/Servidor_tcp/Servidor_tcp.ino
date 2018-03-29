#include <SPI.h>
#include <Ethernet.h>

byte MiMac[]= {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

byte MiIp[]= {192,168,1,45};

EthernetServer ArduinoTCP = EthernetServer (1000);

void setup() {

Serial.begin(9600);
Serial.println("Initializing ethernet connection");
Ethernet.begin(MiMac, MiIp);
ArduinoTCP.begin();


}

void loop() {
  //ArduinoTCP.
EthernetClient cliente = ArduinoTCP.available();


char c;

char cadena[4]={'0','0','0','0'};

int i=0;
for (i=0; i<4; i++){
  cadena[i]= cliente.read();
  
  }

//c= cliente.read();

   Serial.println("Contents:");
   // Serial.println(c);
   Serial.print("Primer elemento: ");
   Serial.println(cadena[0]);
   Serial.print("Segundo elemento: ");
   Serial.println(cadena[1]);
   Serial.print("Tercer elemento: ");
   Serial.println(cadena[2]);
   Serial.print("Cuarto elemento: ");
   Serial.println(cadena[3]);
    delay (5000);

}
