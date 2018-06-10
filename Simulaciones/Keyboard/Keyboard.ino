/*
 * Arduino Keyboard Emulation
 * learnelectronics
 * 13 FEB 2017
 * 
 * www.youtube.com/c/learnelectronics
 */

#include <Keyboard.h>    // This is a "built-in" library no need to install
#include <Mouse.h>

//---------------------------------------------------------
//                           Setup
//---------------------------------------------------------


void bucle2 (char tipo2, char *v) {

                         if (tipo2== 'R') {
                             for (int i=0; i<7; i++) {
                             v[i]= Serial.read();
                             Serial.println(v[i]);
                             }
                         }

}


void bucle1 (char opcion) {
const int sensitivity = 200;  
char v[7]= {'0', '0', '0', '0', '0', '0', '0'};
char tipo2 = 'C';
  
  for(;;) {

      if (opcion=='A') {
          Mouse.move((510/sensitivity)*(1), 0, 0);  
      }
      if (opcion=='B') {
          Mouse.move((510/sensitivity)*(-1), 0, 0);
      }
      if (opcion=='C') {
          Mouse.move(0, (510/sensitivity)*(1), 0);
      }
      if (opcion=='D') {
          Mouse.move(0, (510/sensitivity)*(-1), 0);
      }
    Serial.println("Moviendo");
    tipo2= Serial.read();
    bucle2(tipo2, v); // Paso por referencia
                        /*
                        Serial.println("Imprimir2");
                        Serial.println(v[0]);
                        Serial.println(v[1]);
                        Serial.println(v[2]);
                        Serial.println(v[3]);
                        Serial.println(v[4]);
                        Serial.println(v[5]);
                        Serial.println(v[6]);
                        Serial.println(opcion);
                      */
                      
    if (opcion=='A') {
        if ((v[0] != '3') && (v[1] != '0') && ((v[2] != '1') || (v[2] != '2') || (v[2] != '3')) && (v[3] != '0') && (v[4] != '0') && (v[5] != '0') && (v[6] != '0')) {
          break; 
        }
    }
    else if (opcion=='B') {
        if ((v[0] != '3') && (v[1] != '0') && (v[2] != '1') && (v[3] != '5') && (v[4] != '0') && (v[5] != '0') && (v[6] != '0')) {
          break; 
        }
    }
    else if (opcion=='C') {
        if ((v[0] != '3') && (v[1] != '0') && (v[2] != '0') && (v[3] != '6') && (v[4] != '0') && (v[5] != '0') && (v[6] != '0')) {
          break; 
        }
    }
    else if (opcion=='D') {
        if ((v[0] != '3') && (v[1] != '0') && (v[2] != '0') && (v[3] != '6') && (v[4] != '1') && (v[5] != '0') && (v[6] != '0')) {
          break; 
        }
    }
        delay(5000);
  }
    Serial.println("Terminado");

}







void setup() {
Serial.begin(9600);// begin serial comms for debugging  
}

//---------------------------------------------------------
//                           Loop
//---------------------------------------------------------

void loop() {

char caracter;
const int sensitivity = 200;  
char v[7];
//char p[7];
char tipo= Serial.read();
//char b;
char opcion;



  
 Keyboard.begin();         //begin keyboard 

/////////////////////////////////////////////////////////////
//                      Eventos teclado                    //
/////////////////////////////////////////////////////////////

  if (tipo== 'T') {
      caracter= Serial.read();
      Serial.println(caracter);
      Keyboard.write(caracter);
  }

/////////////////////////////////////////////////////////////
//                       Eventos raton                     //
/////////////////////////////////////////////////////////////

  else if (tipo== 'R') {

        bucle2 (tipo, v);
    /*
        Serial.println(v[0]);
        Serial.println(v[1]);
        Serial.println(v[2]);
        Serial.println(v[3]);
        Serial.println(v[4]);
        Serial.println(v[5]);
        Serial.println(v[6]);
        */
   
        if (v[1]=='1') {
        //************ click izq ***********//
        Mouse.press(MOUSE_LEFT);  
        delay(200);
        Mouse.release(MOUSE_LEFT);  
        }
        
        else if (v[1]=='2') {
        //************ click der ***********//
        Mouse.press(MOUSE_RIGHT);  
        delay(200);
        Mouse.release(MOUSE_RIGHT);  
        }
        
        else if (v[2]=='1' || v[2]=='2' || v[2]=='3') {
        
              if (v[3]=='0') {
              //************ Movimiento en x+ ***********//  
                  opcion= 'A';
                  bucle1(opcion);
              }
            
              else if (v[3]=='5'){
              //************ Movimiento en x- ***********//
                  opcion= 'B';
                  bucle1(opcion);
              }
        } 
        
        else if (v[2]=='0') {
          
              if (v[3]=='6') {
                
                  if (v[4]=='0') {
                  //************ Movimiento en y- ***********//
                  opcion= 'C';
                  bucle1(opcion);
                
                  }
        
                  else if (v[4]=='1') {
                  //************ Movimiento en y+ ***********//
                  opcion= 'D';
                  bucle1(opcion);
                
                  }
              }
        
              if (v[3]=='0') {
                   
                   if (v[5]=='-1') {
                   //************ Ruleta abajo ***********//
                
                   
                   }
                   if (v[5]=='1') {
                   //************ Ruleta arriba ***********//
                
              
                   }
                
              
              }
          
        }

}  
Keyboard.end();                 //stops keybord
  delay(5000);
}


/*
 * 
        char b;
        if ((v[2]=='1')) {  
          Serial.println("Moviendo");
        b='1';
          while (b!='0') {
             Mouse.move((510/sensitivity)*(1), 0, 0);  
             Serial.println("Moviendo");
             b= Serial.read();
             Serial.println(b);
             delay(200);
            }
         Serial.println("Terminado");
        }
    
      }
 */























