//Jacobo De Bruyn Monge

#include <SPI.h>
#include <RH_RF69.h>

/************ Radio Setup ***************/

// Change to 434.0 or other frequency, must match RX's freq!
#define RF69_FREQ 915.0

#if defined (__AVR_ATmega32U4__) // Feather 32u4 w/Radio
  #define RFM69_CS      8
  #define RFM69_INT     7
  #define RFM69_RST     4
  #define LED           13
#endif




// Singleton instance of the radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);

int16_t packetnum = 0;  // packet counter, we increment per xmission


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
int DatoRecibido[22];


void setup() 
{
  Serial.begin(115200);
 

  pinMode(LED, OUTPUT);     
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

  //Limpiando las variables que almacenan los datos
for(int i=0;i<5;i++){
    temperatura[i]=48;
    humedad[i]=48;
    iluminacion[i]=48;
    PC_temperatura[i]=48;
    PC_humedad[i]=48;
    PC_iluminacion[i]=48;
  
  }

  // manual reset
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);
  
  if (!rf69.init()) {
    Serial.println("RFM69 radio init failed");
    while (1);
  }
  Serial.println("RFM69 radio init OK!");
  
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
  // No encryption
  if (!rf69.setFrequency(RF69_FREQ)) {
    Serial.println("setFrequency failed");
  }

  // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
  // ishighpowermodule flag set like this:
  rf69.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW

  // The encryption key has to be the same as the one in the server
  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  rf69.setEncryptionKey(key);
  
  pinMode(LED, OUTPUT);

  Serial.print("RFM69 radio @");  Serial.print((int)RF69_FREQ);  Serial.println(" MHz");
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

 
delay(100);
//Leer Emisor
LeerEmisor(DatoRecibido);

//Convertir dato a formato requerido
CDFR(temperatura,humedad, iluminacion);

//Logica para Actuar sobre las Salidas
LAS();


// Imprimir informacion de los sensores en pantalla
Serial.println();
Serial.println();
Serial.println("//////////////////////////////////////////////////////");
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
     else if(Dato==84){ // 84 Corresponde a la letra T en codigo ASCII, Si empieza con T el dato es de Temperatura
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

     else if(Dato==73){ // 73 Corresponde a la letra I en codigo ASCII, Si empieza con I el dato es de Iluminacion
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
  
  }
  
  
  
  }





 //---------------------------------------------------------------------------------------------------------------------------------------------

void LeerEmisor(int DatoRecibido[19]){// Esta funcion lee la informacion proveniente del Emisor
   int len =6;  //Tamaño del Dato a Leer en bytes
   uint8_t buf [len]; //Variable donde se va a almacenar el dato
 if (rf69.available()) {
    // Should be a message for us now   
    uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf69.recv(buf, &len)) {
      if (!len) return;
      buf[len] = 0;


      for(int k=0;k<22;k++){
        if(k<=len){       
        DatoRecibido[k]=buf[k];
       
        }

        
        }
    

    } else {
      Serial.println("Receive failed");
    }
  }
}




//---------------------------------------------------------------------------------------------------------------------------------------------

//Logica para Actuar sobre las Salidas
  void LAS(){ // Esta funcion compara las entradas y actua sobre las salidas

    //Comparando Temperaturas
  

    if(comparador(temperatura,PC_temperatura)==true){
      //Encender Ventilacion
      digitalWrite(ventilacion,HIGH);
      Serial.println("La ventilacion esta encendida");
      
      delay(100);
      }
      else{
        //Ventilacion Apagada
        digitalWrite(ventilacion,LOW);
        Serial.println("La ventilacion esta Apagada");
        delay(100);
        }


    //Comparando Humedades
   
    if(comparador(humedad,PC_humedad)==false){
      //Desactivar Riego
      digitalWrite(riego,HIGH);
      }
      else{
        //Activar sistema de Riego
        digitalWrite(riego,LOW);
        }

    
     //Comparado Iluminacion
    
       if(comparador(iluminacion , PC_iluminacion)==true){
      //Apagar Iluminacion
      digitalWrite(lamparas,LOW);
      }
      else{
        //Encender Iluminacion
        digitalWrite(lamparas,HIGH);
        }

    
    }
  
  
//---------------------------------------------------------------------------------------------------------------------------------------------

boolean comparador (int num[5], int ref[5]){//Esta funcion convierte el char recibido en un numero flotante
int Dato1;
int Dato2;

Dato1=((num[0]-48)*1000)+((num[1]-48)*100)+((num[3]-48)*10)+(num[4]-48);
Dato2=((ref[0]-48)*1000)+((ref[1]-48)*100)+((ref[3]-48)*10)+(ref[4]-48);  

if(Dato1>Dato2){return true;}
else{return false;}
  }




//-------------------------------------------------------------------------------------------------------------------------------------------------
void CDFR(int t[6], int h[6], int ilu[6]){ // codigo que ordena el dato recibido y lo coloca en la variable correspondiente
  for(int j=0;j<19;j++){
    if (DatoRecibido[j]==84){// Si el dato Recibido es un 84, equivale a un T en ASCII lo que significa que es de temperatura
      j++;
      t[0]=DatoRecibido[j];
      j++;
      t[1]=DatoRecibido[j];
      j++;
      t[2]=DatoRecibido[j];
      j++;
      t[3]=DatoRecibido[j];
      j++;
      t[4]=DatoRecibido[j];
      
      }
    else if (DatoRecibido[j]==72){// Si el dato Recibido es un 72, equivale a un H en ASCII lo que significa que es de Humedad
      j++;
      h[0]=DatoRecibido[j];
      j++;
      h[1]=DatoRecibido[j];
      j++;
      h[2]=DatoRecibido[j];
      j++;
      h[3]=DatoRecibido[j];
      j++;
      h[4]=DatoRecibido[j];
      
      
      }
    else if (DatoRecibido[j]==73){// Si el dato Recibido es un 73, equivale a un I en ASCII lo que significa que es de Iluminacion
      j++;
      ilu[0]=DatoRecibido[j];
      j++;
      ilu[1]=DatoRecibido[j];
      j++;
      ilu[2]=DatoRecibido[j];
      j++;
      ilu[3]=DatoRecibido[j];
      j++;
      ilu[4]=DatoRecibido[j];
      
      
      }
   else{}
    
    
    }
 
  
  
  }



