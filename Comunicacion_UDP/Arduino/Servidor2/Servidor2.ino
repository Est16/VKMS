#include <SPI.h>
#include <Ethernet.h>

//--------------------------------------------------------------------------------------------
//Declaración de la direcciones MAC e IP. También del puerto 80
byte mac[]={0xDE,0xAD,0xBE,0xEF,0xFE,0xED}; //MAC
IPAddress ip(192,168,1,117); //IP
EthernetServer servidor(80);
//--------------------------------------------------------------------------------------------

void setup()
{
 
 //Inicializamos el servidor
  Ethernet.begin(mac, ip); 
  servidor.begin();
}

void loop(){

  
  
  EthernetClient cliente = servidor.available();
        if(cliente.available())
      {
       
        cliente.print("");
        
      }
      
  
  }





/*
void loop()

{

  EthernetClient cliente = servidor.available();
 
      if(cliente.available())
      {
        cliente.print("Hola");
      }
      

  
}


*/

/*

void loop()
{
  
  
  
  EthernetClient cliente = servidor.available();
 
      if(cliente.available())
      {
      
          
          
          
//------------------------------------------------WEB--------------------------------------------------------------------------------------          
          
          
          cliente.print("<!DOCTYPE html>");
          cliente.print("<html lang='es'>");
          
          
          cliente.println("<head>");
          cliente.println("<title>TUTORIAL CREAR UNA PAGINA WEB CON ARDUINO - FROSTCODE.ES</title>");
          cliente.println("<style type='text/css'> body { color: red; background-image: url('http://frostcode.es/descargas/back%20servidor.jpg'); } </style>");//EL CSS
          cliente.println("<meta charset='utf-8'>");
          cliente.println("</head>");
          
          cliente.println("<body>");


          cliente.print("<center>");   
          cliente.print("<h1>Esta es la Página WEB</h1>");
          cliente.print("<br><hr><br>Aquí tienes el tutorial donde lo explico todo y además como publicar la página en internet<br><br><iframe class='youtube-player' type='text/html' width='640' height='385' src='http://www.youtube.com/embed/magShG1Ch50' frameborder='0' allowfullscreen='true'></iframe>");
          cliente.print("</center>");
          
          
          
     
          
          
          
          
          cliente.println("</body>");
          
          cliente.println("</html>");
          cliente.stop();//Cierro conexión con el cliente
        }
      }
    
  
*/

