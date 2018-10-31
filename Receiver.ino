//Tercer Proyecto Arquitectura de Computadores
//Instituto Tecnologico de Costa Rica
// Jacobo De Bruyn Monge  2014079654
//Jordan
//Felipe
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
  
#endif

// Instanciando Controlador del RF
RH_RF69 rf69(RFM69_CS, RFM69_INT);


//Definiendo variables para los pines de salida
#define ventilacion 10   //Este pin corresponde al LED amarillo
#define riego  11   //Este pin corresponde al LED verde
#define lamparas 12    //Este pin corresponde al LED rojo


//variables para almacenar los datos de los sensores
int temperatura[5];
int humedad[5];
int iluminacion[5];

// variables para almacenar datos provenientes de la pc
int PC_temperatura[5];
int PC_humedad[5];
int PC_iluminacion[5];


int Dato; //Byte utilizado para leer datos en la comunicacion serial

//Variables para medir el tiempo de ejecucion del programa
int Time1; // Tiempo de inicio
int Time2;  //Tiempo Final
int Time;    // Tiempo trancurrido  


void setup() {
  //Limpiando las variables
  temperatura[0]=48;
  temperatura[1]=48;
  temperatura[2]=48;
  temperatura[3]=48;
  temperatura[4]=48;
  humedad[0]=48;
  humedad[1]=48;
  humedad[2]=48;
  humedad[3]=48;
  humedad[4]=48;
  humedad[5]=48;
  iluminacion[0]=48;
  iluminacion[1]=48;
  iluminacion[2]=48;
  iluminacion[3]=48;
  iluminacion[4]=48;
  iluminacion[5]=48;

  
  //Definiendo pines de salida
  pinMode(ventilacion, OUTPUT);
  pinMode(riego, OUTPUT);
  pinMode(lamparas, OUTPUT);

  //Estableciendo velocidad de comunicacion serial
  Serial.begin(9600);
  Serial.setTimeout(50);
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
LeerEmisor(temperatura,humedad,iluminacion);


//Logica para Actuar sobre las Salidas
//LAS();


// Imprimir informacion de los sensores en pantalla
Serial.print("La temperatura Actual es de ");
Serial.print(temperatura[0]-48);
Serial.print(temperatura[1]-48);
Serial.print(".");
Serial.print(temperatura[3]-48);
Serial.print(temperatura[4]-48);
Serial.print(" °C");
Serial.println();

Serial.print("La Humedad Actual es de ");
Serial.print(humedad[0]-48);
Serial.print(humedad[1]-48);
Serial.print(".");
Serial.print(humedad[3]-48);
Serial.print(humedad[4]-48);
Serial.print(" %");
Serial.println();


Serial.print("La Iluminacion Actual es de ");
Serial.print(iluminacion[0]-48);
Serial.print(iluminacion[1]-48);
Serial.print(".");
Serial.print(iluminacion[3]-48);
Serial.print(iluminacion[4]-48);
Serial.print(" %");
Serial.println();

//Imprimir informacion de los valores limites de PC registrados

Serial.print("Temperatura limite ");
Serial.print(PC_temperatura[0]-48);
Serial.print(PC_temperatura[1]-48);
Serial.print(".");
Serial.print(PC_temperatura[3]-48);
Serial.print(PC_temperatura[4]-48);
Serial.print("°C");
Serial.println();

Serial.print("Limite humedad ");
Serial.print(PC_humedad[0]-48);
Serial.print(PC_humedad[1]-48);
Serial.print(".");
Serial.print(PC_humedad[3]-48);
Serial.print(PC_humedad[4]-48);
Serial.print(" %");
Serial.println();

Serial.print("Iluminacion limite ");
Serial.print(PC_iluminacion[0]-48);
Serial.print(PC_iluminacion[1]-48);
Serial.print(".");
Serial.print(PC_iluminacion[3]-48);
Serial.print(PC_iluminacion[4]-48);
Serial.print(" %");
Serial.println();  





}



//---------------------------------------------------------------------------------------------------------------------------------------------



//Funcion Leer para Leer los dato provenientes desde la PC
void LeerPC(){// Esta funcion lee la informacion proveniente de la PC
  //Leyendo informacion proveniente de la PC
if(Serial.available()){
  //Leer el primer byte
     Dato=Serial.read(); // Leyendo el registro del puerto serial
     if(Dato==72){ // 72 Corresponde a la letra H en codigo ASCII, Si empieza con H el dato es de Humedad
      //Copiando el dato en las variables correspondientes
      Dato=Serial.read();
      PC_humedad[0]=Dato;
      Dato=Serial.read();
      PC_humedad[1]=Dato;
      Dato=Serial.read();
      PC_humedad[2]=Dato;
      Dato=Serial.read();
      PC_humedad[3]=Dato;
      Dato=Serial.read();
      PC_humedad[4]=Dato;      
      }
     if(Dato==84){ // 84 Corresponde a la letra T en codigo ASCII, Si empieza con T el dato es de Temperatura
      //Copiando el dato en las variables correspondientes
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

     if(Dato==73){ // 73 Corresponde a la letra I en codigo ASCII, Si empieza con I el dato es de Iluminacion
      //Copiando el dato en las variables correspondientes
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

// Se puede encender o apagar la ventilacion desde la Consola con enviando el mensaje VN, o VF
           if(Dato==86){ // 86 Corresponde a la letra V en codigo ASCII.
              Dato=Serial.read();
              if(Dato==78){//78 Corresponde a la letra N
                digitalWrite(ventilacion,HIGH);
                
                }
               if(Dato==70){digitalWrite(ventilacion,LOW);}
              
      
         
      }



      
// Se puede encender o apagar el sistema de Riego desde la Consola con enviando el mensaje RN, o RF
           
             if(Dato==82){ // 82 Corresponde a la letra R en codigo ASCII.
              Dato=Serial.read();
              if(Dato==78){//78 Corresponde a la letra N
                digitalWrite(riego,HIGH);
                
                }
               if(Dato==70){digitalWrite(riego,LOW);}
              
      
         
      }

      
// Se puede encender o apagar las Luces desde la Consola con enviando el mensaje LN, o LF
           if(Dato==76){ // 76 Corresponde a la letra L en codigo ASCII.
             
              Dato=Serial.read();
              if(Dato==78){//78 Corresponde a la letra N
                digitalWrite(lamparas,HIGH);
                
                }
               if(Dato==70){digitalWrite(lamparas,LOW);}
              
      
         
      }

      else {Serial.println("El dato recibido es un error");}
  
  
  }
  
  
  
  }



  //---------------------------------------------------------------------------------------------------------------------------------------------

void LeerEmisor(int DataT[5],int DataH[5], int DataI[5]){// Esta funcion lee la informacion proveniente del Emisor
   int len =6;  //Tamaño del Dato a Leer en bytes
   uint8_t buf [len]; //Variable donde se va a almacenar el dato
   while (true){
   if (rf69.available()>5) {
    rf69.recv(buf, len);
      //Comparando que tipo de dato es
      if(buf[0]==72){ // El dato es de Humedad
        buf[1]=DataH[0];
        buf[2]=DataH[1];
        buf[3]=DataH[2];
        buf[4]=DataH[3];
        buf[5]=DataH[4];
        }
        else if(buf[0]==84){ // El dato es de Temperatura
        buf[1]=DataT[0];
        buf[2]=DataT[1];
        buf[3]=DataT[2];
        buf[4]=DataT[3];
        buf[5]=DataT[4];
        }
        else if(buf[0]==73){ // El dato es de Iluminacion
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

void DatoToFloat (int num[5], float Numero){//Esta funcion convierte el char recibido en un numero flotante
  Numero=(num[0]*10)+num[1]+(num[3]/10)+(num[4]/100);
  }


//---------------------------------------------------------------------------------------------------------------------------------------------



//Logica para Actuar sobre las Salidas
  void LAS(){ // Esta funcion compara las entradas y actua sobre las salidas
    float Dato1;
    float Dato2;
    //Comparando Temperaturas
    DatoToFloat(temperatura,Dato1);
    DatoToFloat(PC_temperatura,Dato2);
    if(Dato1>=Dato2){
      //Encender Ventilacion
      digitalWrite(ventilacion,HIGH);
      }
      else{
        //Ventilacion Apagada
        digitalWrite(ventilacion,LOW);
        }


    //Comparando Humedades
    DatoToFloat(humedad,Dato1);
    DatoToFloat(PC_humedad,Dato2);
    if(Dato1>=Dato2){
      //Desactivar Riego
      digitalWrite(riego,LOW);
      }
      else{
        //Activar sistema de Riego
        digitalWrite(riego,HIGH);
        }

    
     //Comparado Iluminacion
    DatoToFloat(iluminacion,Dato1);
    DatoToFloat(PC_iluminacion,Dato2);
       if(Dato1>=Dato2){
      //Apagar Iluminacion
      digitalWrite(iluminacion,LOW);
      }
      else{
        //Encender Iluminacion
        digitalWrite(ventilacion,HIGH);
        }

    
    }
  
  
