#include <SPI.h>
#include <Ethernet.h>

byte MiMac[]= {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

byte MiIp[]= {192,168,1,38};

EthernetServer ArduinoTCP = EthernetServer (1000);

void setup() {

Serial.begin(9600);
Serial.println("Initializing ethernet connection");
Ethernet.begin(MiMac, MiIp);
ArduinoTCP.begin();



}

int cont=0;

void loop() {
  //ArduinoTCP.

EthernetClient cliente = ArduinoTCP.available();


char c;


//c= cliente.read();
//Serial.println(sizeof(char));
/*
if (c=='T') {

   char cadena[2]={'0','0'};
   cadena[0]=c;
   cadena[1]= cliente.read();
   
   Serial.println("Contents:");
   Serial.print("Primer elemento: ");
   Serial.println(cadena[0]); //BIEN
   Serial.print("Segundo elemento: ");
   Serial.println(cadena[1]);

  
}

if (c=='R') {

char cadena[8]={'0','0','0','0','0','0','0','0'};
cadena[0]=c;
  Serial.println("Contents:");
  Serial.println("Elemento 1: ");
  Serial.println(cadena[0]); //BIEN

int i;
for (i=1; i<8; i++){
  cadena[i]= cliente.read();
  Serial.println("Elemento: ");
  Serial.println(cadena[i], HEX);


  }
  
}
*/


char prueba[7];
//char c;

int i;

c= cliente.read();
//char vect[1025];


if(c=='T'||c=='R'){
  
Serial.println("///////////////");
Serial.println(c);

} else {

Serial.println(c, HEX);
//Serial.println(c);
  
}


/*


if (c=='T') {
vect[cont]=c;
Serial.print("///////////////");
Serial.print("TIPO: ");
Serial.println(vect[0]);
Serial.print("BUFER: ");

cont= cont + 1;

}

if (c=='R') {
Serial.print("///////////////");
char vect[8];
vect[0]=c;
Serial.print("TIPO: ");
Serial.println(c);
Serial.print("BUFER: ");
}
*/

/*
Serial.print("COMPROBACION: ");
Serial.println(cont);
Serial.print("C:");
Serial.println(c);

*/

//Serial.print("C:");
//Serial.println(c, HEX);

/*
if (c=='A') {
  prueba[0]=c;
Serial.print("Elemento: ");
Serial.println(0);
Serial.println(prueba[0]);
for (i=1; i<7; i++){

  prueba[i]= cliente.read();
 
  Serial.print("Elemento: ");
  Serial.println(i);
  Serial.println(prueba[i]);
}
  }
  
Serial.println("//////////////fin del bucle//////////////");

prueba[0]= '0';
prueba[1]= '0';
prueba[2]= '0';
prueba[3]= '0';
prueba[4]= '0';
prueba[5]= '0';
prueba[6]= '0';
Serial.println("//////////////datos borrados//////////////");




*/
 /*
if (cont==7) {

  cont=0;
  
}
*/

/*

Serial.print("Elemento: ");
Serial.println(cont);
Serial.println(prueba[cont]=cliente.read());

*/

/*

char cadena[8]={'0','0','0','0','0','0','0','0'};

int i=0;
for (i=0; i<8; i++){
  cadena[i]= cliente.read();
  
  }

//c= cliente.read();

   Serial.println("Contents:");
   // Serial.println(c);
   Serial.print("Primer elemento: ");
   Serial.println(cadena[0]); //BIEN
   Serial.print("Segundo elemento: ");
   Serial.println(cadena[1], HEX);
   Serial.print("Tercer elemento: ");
   Serial.println(cadena[2], HEX);
   Serial.print("Cuarto elemento: ");
   Serial.println(cadena[3], HEX);
   Serial.print("Quinto elemento: ");
   Serial.println(cadena[4], HEX);
   Serial.print("Sexto elemento: ");
   Serial.println(cadena[5], HEX);
   Serial.print("Séptimo elemento: ");
   Serial.println(cadena[6], HEX);
   Serial.print("Octavo elemento: ");
   Serial.println(cadena[7], HEX);

   */
    delay (1000);
    //exit(0);

}
