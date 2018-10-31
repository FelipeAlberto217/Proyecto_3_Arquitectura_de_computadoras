//Tercer Proyecto Arquitectura de Computadores
//Instituto Tecnologico de Costa Rica
// Jacobo De Bruyn Monge  2014079654
//Jordan
//Felipe Solano Sanchez 2015116656
// Este documento coresponde al receptor

// Importando libreria para el trasmisor y receptor de datos por RF
#include <SPI.h>
#include <RH_RF69.h>


#define RF69_FREQ 915.0
// Identifica el tipo de dispositivo
#if defined (__AVR_ATmega32U4__) // Feather 32u4 w/Radio
  #define RFM69_CS      8
  #define RFM69_INT     7
  #define RFM69_RST     4
  #define LED           13
#endif

// Instanciando Controlador del RF
RH_RF69 rf69(RFM69_CS, RFM69_INT);


//Definiendo variables para los pines de salida
#define ventilacion 10   //Este pin corresponde al LED amarillo
#define riego  11   //Este pin corresponde al LED verde
#define lamparas 12    //Este pin corresponde al LED rojo

//variables para almacenar los datos de los sensores
char temperatura[5];
char humedad[5];
char iluminacion[5];

// variables para almacenar datos provenientes de la pc
char PC_temperatura[5];
char PC_humedad[5];
char PC_iluminacion[5];

byte Dato; //Byte utilizado para leer datos en la comunicacion serial

//Variables para medir el tiempo de ejecucion del programa
int Time1; // Tiempo de inicio
int Time2;  //Tiempo Final
int Time;    // Tiempo trancurrido  


void setup() {
  //Definiendo pines de salida
  pinMode(ventilacion, OUTPUT);
  pinMode(riego, OUTPUT);
  pinMode(lamparas, OUTPUT);


  //Estableciendo velocidad de comunicacion serial
  Serial.begin(9600);
  //Esperar hasta que se establezca la comunicacion serial con la pc
  while(!Serial){};
  
}


void loop() {
//Leer tiempo de ejecucion del programa 
//Inicio del conteo
Time1=millis();
while(true){
    //Leyendo informacion proveniente de la PC
    LeerPC();
    //Fin del Conteo
    Time2=millis();
    Time=Time2-Time1; //Tiempo transcurrido

     //Si el Tiempo Transcurido Supera los 5 segundos, se debe hacer una interrupcion para leer la informacion proveniente del emisor
     if(Time>=5000){
        break;
      }
      else{}
  
}

//Leer Emisor
LeerEmisor(temperatura[5],humedad[5],iluminacion[5]);


//Logica para Actuar sobre las Salidas
LAS();


// Imprimir informacion de los sensores en pantalla
Serial.print("La temperatura Actual es de ");
Serial.print(temperatura);
Serial.print(" °C");
Serial.print("/n");

Serial.print("La Humedad Actual es de ");
Serial.print(humedad);
Serial.print(" %");
Serial.print("/n");


Serial.print("La Iluminacion Actual es de ");
Serial.print(iluminacion);
Serial.print(" %");
Serial.print("/n");

//Imprimir informacion de los valores limites de PC registrados

Serial.print("La temperatura limite para activar la ventilacion es de ");
Serial.print(PC_temperatura);
Serial.print(" °C");
Serial.print("/n");

Serial.print("La Humedad limite para activar el sistema de riego es de ");
Serial.print(PC_humedad);
Serial.print(" %");
Serial.print("/n");

Serial.print("La Iluminacion limite para activar el sistema de iluminacion es de ");
Serial.print(PC_iluminacion);
Serial.print(" %");
Serial.print("/n");   

          
}



//---------------------------------------------------------------------------------------------------------------------------------------------



//Funcion Leer para Leer los dato provenientes desde la PC
void LeerPC(){// Esta funcion lee la informacion proveniente de la PC
  //Leyendo informacion proveniente de la PC
if(Serial.available()>5){

  //Leer el primer byte
    Dato=Serial.read(); // Leyendo el registro del puerto serial

    //Clasificar que tipo de dato es
    if(Dato=="H"){ // es un dato de Humedad
        Dato=Serial.read();  // Leyendo el registro del puerto serial
        PC_humedad[0]=Dato;   //Pasando el dato a la variable correspondiente
        Dato=Serial.read();
        PC_humedad[1]=Dato;
        Dato=Serial.read();
        PC_humedad[2]=Dato;
        Dato=Serial.read();
        PC_humedad[3]=Dato;
        Dato=Serial.read();
        PC_humedad[4]=Dato;
      
                 
      }
      else if(Dato=="T"){ // Es un dato de Temperatura
        Dato=Serial.read();
        PC_temperatura[0]=Dato;
        Dato=Serial.read();
        PC_temperatura[1]=Dato;
        Dato=Serial.read();
        PC_temperatura[2]=Dato;
        Dato=Serial.read();
        PC_temperatura[3]=Dato;
        Dato=Serial.read();
        PC_temperatura[4]=Dato;    
        }
        else if(Dato=="I"){ // Es un dato de Iluminacion          
        Dato=Serial.read();
        PC_iluminacion[0]=Dato;
        Dato=Serial.read();
        PC_iluminacion[1]=Dato;
        Dato=Serial.read();
        PC_iluminacion[2]=Dato;
        Dato=Serial.read();
        PC_iluminacion[3]=Dato;
        Dato=Serial.read();
        PC_iluminacion[4]=Dato;          
          } 
         else{ // El dato es un Error
          Serial.println("Sea ha recibido un error");
          }
  
  
  
  
  }
  
  } 




//---------------------------------------------------------------------------------------------------------------------------------------------

void LeerEmisor(char DataT[5],char DataH[5],char DataI[5]){// Esta funcion lee la informacion proveniente del Emisor
   int len =6;  //Tamaño del Dato a Leer en bytes
   uint8_t buf [len]; //Variable donde se va a almacenar el dato
   while (true){
   if (rf69.available()>5) {
    rf69.recv(buf, len);
      //Comparando que tipo de dato es
      if(buf[0]="H"){ // El dato es de Humedad
        buf[1]=DataH[0];
        buf[2]=DataH[1];
        buf[3]=DataH[2];
        buf[4]=DataH[3];
        buf[5]=DataH[4];
        }
        else if(buf[0]="T"){ // El dato es de Temperatura
        buf[1]=DataT[0];
        buf[2]=DataT[1];
        buf[3]=DataT[2];
        buf[4]=DataT[3];
        buf[5]=DataT[4];
        }
        else if(buf[0]="I"){ // El dato es de Iluminacion
        buf[1]=DataI[0];
        buf[2]=DataI[1];
        buf[3]=DataI[2];
        buf[4]=DataI[3];
        buf[5]=DataI[4];
        }
        else{
           // El dato es un Error
          Serial.println("Sea ha recibido un error");
        }



      
    
    } 
    else{break;}
   }
}



//---------------------------------------------------------------------------------------------------------------------------------------------





void CharToFloat (char Recibido[5], float Numero){//Esta funcion convierte el char recibido en un numero flotante
  int num[4];
  int numeros[10]={'0','1','2','3','4','5','6','7','8','9'};
  for(int j=0;j<5;j++){
    if(j=2){continue;}
    else{
        for(int k=0;k<=9;k++){
          if(numeros[k]=Recibido[j]){ 
            num[j]=k;
            }
          }
      }
    }
Numero=(num[0]*10)+num[1]+(num[3]/10)+(num[4]/100);
  }


//---------------------------------------------------------------------------------------------------------------------------------------------
  

//Logica para Actuar sobre las Salidas
  void LAS(){ // Esta funcion compara las entradas y actua sobre las salidas
    float Dato1;
    float Dato2;
    //Comparando Temperaturas
    CharToFloat(temperatura,Dato1);
    CharToFloat(PC_temperatura,Dato2);
    if(Dato1>=Dato2){
      //Encender Ventilacion
      digitalWrite(ventilacion,HIGH);
      }
      else{
        //Ventilacion Apagada
        digitalWrite(ventilacion,LOW);
        }


    //Comparando Humedades
    CharToFloat(humedad,Dato1);
    CharToFloat(PC_humedad,Dato2);
    if(Dato1>=Dato2){
      //Desactivar Riego
      digitalWrite(riego,LOW);
      }
      else{
        //Activar sistema de Riego
        digitalWrite(riego,HIGH);
        }

    
     //Comparado Iluminacion
    CharToFloat(iluminacion,Dato1);
    CharToFloat(PC_iluminacion,Dato2);
       if(Dato1>=Dato2){
      //Apagar Iluminacion
      digitalWrite(iluminacion,LOW);
      }
      else{
        //Encender Iluminacion
        digitalWrite(ventilacion,HIGH);
        }

    
    }
  
  











