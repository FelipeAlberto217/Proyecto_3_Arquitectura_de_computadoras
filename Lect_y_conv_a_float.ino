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



void setup() {           //Comunicacion serial y modo de los pines a utilizar
  Serial.begin(9600);
  pinMode(temp,INPUT);
  pinMode(lum,INPUT);
  pinMode(hum,INPUT);
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
  t="T";        //Se reinicializan las variables en cada iteracion debido a que se concatena el valor a la letra con el tipo de dato
  i="I";
  h="H";
  
  lect_temp=analogRead(temp);                     //Lectura de temperatura
  temp_escala=fmap(lect_temp, 0,445,0.0,2.14);    //Conversion a escala de punto flotante, max_salida debe ser preferiblemente el valor maximo exacto medido de forma experimental
  temp_real=fmap(temp_escala, 0.0,2.14,temp_min,temp_max);
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
  delay(1000);
}
