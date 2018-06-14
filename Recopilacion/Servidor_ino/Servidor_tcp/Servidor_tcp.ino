#include <SPI.h>
#include <Ethernet.h>
//#include <Keyboard.h>
//#include <Mouse.h>

byte MiMac[]= {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

byte MiIp[]= {192,168,1,45};

EthernetServer ArduinoTCP = EthernetServer (1000);

void setup() {

Serial.begin(9600);
//pinMode(10,OUTUPUT)
Serial.println("Initializing ethernet connection");
Ethernet.begin(MiMac, MiIp);
ArduinoTCP.begin();
// Keyboard.begin();  
}


unsigned char buf[8]={0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};

#define sensitivity 200

typedef struct eventos {  
    byte x_plus[7]= {0x3, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0};
    byte x_less[7]= {0x3, 0x0, 0xFF, 0xF, 0x0, 0x0, 0x0}; //    byte x_less[7]= {3, 0, -1, 15, 0, 0, 0};
    byte y_plus[13]= {0x3, 0x0, 0x0, 0xF0, 0xFF, 0x0, 0x0}; //    byte y_plus[7]= {3, 0, 0, -16, -1, 0, 0};
    byte y_less[7]= {0x3, 0x0, 0x0, 0x10, 0x0, 0x0, 0x0}; //    byte y_less[7]= {3, 0, 0, 16, 0, 0, 0};
    byte ckd[7]= {0x3, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0};
    byte cki[7]= {0x3, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0};
};

//raton* bytes;

void loop() {
    eventos raton;
    EthernetClient cliente = ArduinoTCP.available();
    
    //char c;
    int i,p;
    int cont1=0;
    int cont2=0;
    int cont3=0;
    int cont4=0;
    int cont5=0;
    int cont6=0;
    double A=0;
    double B=0;
      

    Serial.print("Contenido de buf[0]: ");
    Serial.println(buf[0], HEX); 
    
    buf[0]= cliente.read(); 
    
    //////////////////////////////////////////////
    //        Leer el contenido del bufer       //
    //////////////////////////////////////////////
    
    if (buf[7]=='T') {
        //Keyboard.write(buf[6]);
        Serial.print("Caracter: ");
        Serial.println(buf[6], HEX);
    }

    if  (buf[7]=='R') {
        for (i=6; i>=0; i--) {
            if (buf[i]==raton.x_plus[p]) {
                cont1++;
                if (cont1==7) {
                    A= (510/sensitivity);
                    B=0;
                    Serial.println("A");
                }
            }
            if (buf[i]==raton.x_less[p]) {
                cont2++;;
                if (cont2==7) {
                    A= (510/sensitivity)*(-1);
                    B=0;
                    Serial.println("B");
                }
            }
            if (buf[i]==raton.y_plus[p]) {
                cont3++;
                if (cont3==7) {
                    A= 0;
                    B= (510/sensitivity);
                    Serial.println("C");

                }
            }
            if (buf[i]==raton.y_less[p]) {
                cont4++;
                if (cont4==7) {
                    A= 0;
                    B= (510/sensitivity)*(-1);
                    Serial.println("D");

                }
            }
            if (buf[i]==raton.ckd[p]) {
                cont5++;
                if (cont5==7) {
                    //Mouse.press(MOUSE_RIGHT);  
                    //delay(200);
                    //Mouse.release(MOUSE_RIGHT);
                    Serial.println("E");
                }
            }
            if (buf[i]==raton.cki[p]) {
                cont6++;
                if (cont6==7) {
                    //Mouse.press(MOUSE_LEFT);  
                    //delay(200);
                    //Mouse.release(MOUSE_LEFT);
                    Serial.println("F");

                }
            }
            p++;
       }
       //mouse.move(A,B,0);
    }
    
    
    //////////////////////////////////////////////
    //              Rellenar el bufer           //
    //////////////////////////////////////////////
    
    for (int i=7; i>0; i--) {
      //Serial.println("///////////////");
      Serial.print("Contenido de la posición ");  
      Serial.print(i); 
      Serial.print(" de buf[]: "); 
      Serial.println(buf[i], HEX);    
      buf[i]= buf[(i-1)];      
      delay(10);
    }
    Serial.println("///////////////");
      //Serial.println("buf[0]");
      //Serial.println(buf[0]);  
      //buf[0]= cliente.read();
      
    delay (2000);

}
