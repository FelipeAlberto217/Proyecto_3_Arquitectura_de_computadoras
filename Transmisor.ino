#include <SPI.h>
#include <RH_RF69.h>

#define RF69_FREQ 915.0

#if defined (__AVR_ATmega32U4__) // Feather 32u4 w/Radio
  #define RFM69_CS      8
  #define RFM69_INT     7
  #define RFM69_RST     4
  #define LED           13
#endif

RH_RF69 rf69(RFM69_CS, RFM69_INT);

int16_t packetnum = 0;  // packet counter, we increment per xmission


const int temp = A0;      //Definicion de las entradas a utilizar 
const int lum = A1;
const int hum = A2;

float lect_temp;            //Variables de cada una de las lecturas
float lect_lum;
float lect_hum;

float temp_escala;         //Variables para guardar la lectura luego de aplicar la escala de 0 a 2V
float lum_escala;
float hum_escala;

float temp_real;           //Variables para guardar la lectura luego de aplicar la escala a valores reales
float lum_real;
float hum_real;


float temp_max=40.00;      //Definicion de los rangos a medir
float temp_min=25.00;
float lum_max=40.00;
float lum_min=25.00;
float hum_max=40.00;
float hum_min=25.00;

String t="T";             //Strings a concatenar al inicio de las lecturas para identificar de que tipo son
String i="I";
String h="H";
String Dato;



void setup() {           //Comunicacion serial y modo de los pines a utilizar
  Serial.begin(115200);
  //Serial.begin(9600);
  pinMode(temp,INPUT);
  pinMode(lum,INPUT);
  pinMode(hum,INPUT);
  
  //while (!Serial) { delay(1); } // wait until serial console is open, remove if not tethered to computer

  pinMode(LED, OUTPUT);     //utilizacion del LED
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

  Serial.println("Feather RFM69 TX Test!");
  Serial.println();

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

  // si se usa alto poder RF69 eg RFM69HW, you *must* set a Tx power with the
  // ishighpowermodule flag set like this:
  rf69.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW

  // The encryption key has to be the same as the one in the server
  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  rf69.setEncryptionKey(key);
  
  pinMode(LED, OUTPUT);

  Serial.print("RFM69 radio @");  Serial.print((int)RF69_FREQ);  Serial.println(" MHz");
}

/*Definicion de la funcion fmap 
          Se define esta funcion debido a que se debe mapear el valor leido de 
          10 bits por el ADC a un valor del tipo flotante que vaya de 0 al valor
          máximo posible a medir, en este caso un valor aproximado a 2V. No se 
          utiliza la funcion map debido a que esta solo trabaja con enteros y
          trunca los decimales de valores flotantes.*/
float fmap (float valor_original, float min_original, float max_original, float min_salida, float max_salida)
    {
  return (valor_original - min_original) * (max_salida - min_salida) / (max_original - min_original) + min_salida;
}


void loop() {
  delay(1000);
  t="T";        //Se reinicializan las variables en cada iteracion debido a que se concatena el valor a la letra con el tipo de dato
  i="I";
  h="H";
  
  lect_temp=analogRead(temp);                     //Lectura de temperatura
  temp_escala=fmap(lect_temp, 0,445,0.0,2.14);    //Conversion a escala de punto flotante, max_salida debe ser preferiblemente el valor maximo exacto medido de forma experimental
  temp_real=fmap(temp_escala, 0.0,2.14,temp_min,temp_max);//Conversión a valores reales
  t.concat(temp_real);
  Serial.print("Temperatura: ");
  Serial.println(t);
  
  
  lect_lum=analogRead(lum);                     
  lum_escala=fmap(lect_lum, 0,445,0.0,2.14);
  lum_real=fmap(lum_escala, 0.0,2.14,lum_min,lum_max);
  i.concat(lum_real);
  Serial.print("Luz: ");
  Serial.println(i);
  

  lect_hum=analogRead(hum);                     
  hum_escala=fmap(lect_hum, 0,445,0.0,2.14);
  hum_real=fmap(hum_escala, 0.0,2.14,hum_min,hum_max);
  h.concat(hum_real);
  Serial.print("Humedad: ");
  Serial.println(h);
  Serial.println();
  Dato = t+i+h;
  
  char datoRF[19]; // definicion del arreglo a mandar por RF
  Dato.toCharArray(datoRF,19); //Guardar los datos de los sensores en la variable anterior

  Serial.print("Sending "); Serial.println(Dato); //Para visualizar el dato emitido
  
  // Send a message!
  rf69.send((uint8_t *)datoRF, strlen(datoRF)); //Mandar el dato por RF
  rf69.waitPacketSent();


/*Esta parte del codigo seria opcional si se quiere encender el LED si el receptor responde*/
  /* Now wait for a reply
  uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf69.waitAvailableTimeout(500))  { 
    // Should be a reply message for us now   
    if (rf69.recv(buf, &len)) {
      Serial.print("Got a reply: ");
      Serial.println((char*)buf);
      Blink(LED, 50, 3); //blink LED 3 times, 50ms between blinks
    } else {
      Serial.println("Receive failed");
    }
  } else {
    Serial.println("No reply, is another RFM69 listening?");
  }*/
}




void Blink(byte PIN, byte DELAY_MS, byte loops) {
  for (byte i=0; i<loops; i++)  {
    digitalWrite(PIN,HIGH);
    delay(DELAY_MS);
    digitalWrite(PIN,LOW);
    delay(DELAY_MS);
  }
}
