
#include <DHT11.h> // Incluye libreria para nanejo DHT
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SoftwareSerial.h>

int pin=7;
DHT11 dht11(pin);

const int pinCE = 9;
const int pinCSN = 10;
RF24 radio(pinCE, pinCSN);

// Single radio pipe address for the 2 nodes to communicate.
const uint64_t connect1 = 0xE8E8F0F0E1LL;
float data[4];

float temp, hum;
float monOxide, nh4, Rs;


void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(connect1);
  
}

void loop() {

  //------- Llenamos el arreglo con los datos de los dos sensores MQ y DHT ----------------- 

    dht11.read(hum, temp); // Hacemos uso del metodo read para leer y asignar valor a las variables hum y temp 

    Rs = functReadMQSensor(A4, 5, 10000);
    monOxide = 100.52 * pow(Rs/5463, -1.277); // Esta ecuacion se obtiene por medio de una regresion lineal, exponencial segun sea el caso, 
                                              //para el sensro Mq7 es esta, pero varia dependiendo del sensor (revisar DataSheet del sensor)  
                                              //calculamos la concentración  de monoxido de carbono con la ecuación obtenida.
    
    Rs = functReadMQSensor(A2, 5, 20000);
    nh4 = (30.593 * pow(Rs/5463, 2)) - (167.14 * (Rs/5463)) + 231.55; // Esta ecuacion se obtiene por medio de una regresion lineal, exponencial segun sea el caso,  
                                                                      //para el sensro Mq135 es esta, pero varia dependiendo del sensor (revisar DataSheet del sensor) 
                                                                      //calculamos la concentración  de nh4 con la ecuación obtenida.
   
   data[0] = monOxide; 
   data[1] = nh4;
   data[2] = temp;
   data[3] = hum;

   radio.write(data, sizeof data);
   delay(5000);   
}

float functReadMQSensor(int analogPin, float powerVolt, float rL){  // analogPin pin conexion sensor, inVoltage voltaje de alimentacion Sensor (5v o 3.3v) 
   int adc_MQ = analogRead(analogPin); //Leemos el puerto analogico para determinar la salida analógica  del sensor MQ  
   float readVolt = adc_MQ * ( powerVolt / 1023.0); //Convertimos la lectura en un valor de voltaje
   float rS = rL *((5-readVolt)/ readVolt);  //Ecuacion del divisor de voltaje, formada por el sensor y resitencia rL, permite calcualr  rS 
   return rS;                                 // para el sensor MQ7 rL = 10K, para MQ135 rL  = 20K (Segun datasheet)
}
