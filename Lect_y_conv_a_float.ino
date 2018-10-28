const int temp = A0;      //Definicion de las entradas a utilizar 
const int lum = A1;
const int hum = A2;

float lect_temp;            //Variables de cada una de las lecturas
float lect_lum;
float lect_hum;
float temp_escala;

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
float fmap (float valor_sensor, float min_sensor, float max_sensor, float min_salida, float max_salida)
    {
  return (valor_sensor - min_sensor) * (max_salida - min_salida) / (max_sensor - min_sensor) + min_salida;
}


void loop() {
  lect_temp=analogRead(temp);                     //Lectura de temperatura
  temp_escala=fmap(lect_temp, 0,445,0.0,2.14);    //Conversion a escala de punto flotante, max_salida debe ser preferiblemente el valor maximo exacto medido de forma experimental
  Serial.println(temp_escala);
  delay(1000);
}
