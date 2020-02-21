#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SoftwareSerial.h>
 
const int pinCE = 9;
const int pinCSN = 10;
RF24 radio(pinCE, pinCSN);
SoftwareSerial SerialBLE(2,3); //puerto de counicacion BLE
const int ledPIN = 7; // pon de salida del LED indicador
 
// Single radio pipe address for the 2 nodes to communicate.
const uint64_t connect1 = 0xE8E8F0F0E1LL;
 
float data[4];  // Arreglo de datos que son recibidos del emisor
uint8_t dataBLE[20]; // Arreglo de datos que seran enviados al Movil
 
void setup()
{
   radio.begin();
   Serial.begin(115200); 
   SerialBLE.begin(115200);  /// Velocidad de los Baudios en comunicacion BLE
   radio.openReadingPipe(1, connect1);
   radio.startListening();
   pinMode(ledPIN , OUTPUT);  //definir pin como salida
}
 
void loop()
{
   if (radio.available())
   {    
      radio.read(data, sizeof data); // Recibo los datos del modulo Emisor y los alaceno el el arreglo "data"
      
      if(data[0]>5 || data[1]>6 ){// en caso de que las variables Co2 o NH4 sean elevadas se enciende el LED
        digitalWrite(ledPIN , HIGH);   // poner el Pin en HIGH
        //delay(1000);  // esperar un segundo
        }   
      else{  
        digitalWrite(ledPIN , LOW);    // poner el Pin en LOW
        //delay(1000);                   // esperar un segundo
        }

       for(int i=0; i<20; i=i+4){
        dataBLE[i] = data[0]; // colocamos el valor de Co2 que se encuentra en data[0],  y lo alamacenamos en dataBLE
        dataBLE[i+1] = data[1];// lo mismo con este y el resto
        dataBLE[i+2] = data[2];
        dataBLE[i+3] = data[3];
        }    
        
      Serial.print("Co2 = " );
      Serial.print(data[0]);
      Serial.print("NH4 = " );
      Serial.print(data[1]);
      Serial.print("--Temp = " );
      Serial.print(data[2]);
      Serial.print("--Hum = " );
      Serial.println(data[3]);
      
      SerialBLE.write(dataBLE, 20); //Envia el arreglo  al movil
      
   }
   delay(1000);
   }
  
