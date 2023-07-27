#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define TAU 2.0*PI

#define SUMA_INI .75
#define CANT_LEDS 30

int LED_PIN = D6 ;

float f = 0.95;

//frecuencia general del clock
float frecSeno = .00005;

float brilloGeneral = 1.1;
float offGeneral = 0.0; //agregarle T?
float amplitudDesfase =.25;

int espera = 2000;
int  muestreo = 20;
int indice = 0;
bool distancia = true;
bool distLuz = false; //no funca bien
bool rampa = true;
float potencia = 2.; // para la ecuacion de inversa de la distancia
//fecuencia
float frecR = PI * .5;
float frecG = PI * .1;
float frecB = .05;
float desfaceR = 0.50 * TAU;
float desfaceG = 0.33 * TAU;
float desfaceB = 0.0 * TAU;
//en rangos nomalizados es decir pensndolos de 0a 1.
//amplitud //reduciendo la onda a la mitad de -0.5 a 0.5
float ampR = .5;
float ampG = .275;
float ampB = .25;
//offset //para compensar o desplazar la onda hacia arriba
float offR = 0.5 ;
float offG = 0.25;
float offB = 0.3;
float thresh = .0; //en num normalizados

//VARIABLES OPERATIVAS
int cantPixeles =  CANT_LEDS;

long marcaTiempo, tiempoActual;
long marcaTiempoFrec;
float  sumatoriasR[CANT_LEDS];
float  sumatoriasG[CANT_LEDS];
float  sumatoriasB[CANT_LEDS];
bool direccion = true;

Adafruit_NeoPixel strip(cantPixeles, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  tiempoActual = millis();
  marcaTiempo = tiempoActual;
  marcaTiempoFrec = tiempoActual;
  Serial.begin(9600);

  for (int i = 0 ; i < cantPixeles; i++) {
    sumatoriasR[i] = SUMA_INI;
    sumatoriasG[i] = SUMA_INI;
    sumatoriasB[i] = SUMA_INI;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  tiempoActual = millis();


  if (tiempoActual > marcaTiempoFrec + muestreo) {

    for (int i = 0 ; i < cantPixeles; i++) {

      float proporcion = float(i) / float(cantPixeles - 1);
      float desfaseLed =  proporcion * TAU*amplitudDesfase; // dibuja un seno completo en la tira
      float clockGeneral = float(tiempoActual) * frecSeno;

      // float d = pow(( abs(i - indice)) / float(cantPixeles - 1), potencia);

      float r = sin(clockGeneral * frecR + desfaceR + desfaseLed) * ampR + offR ;
      float g = sin(clockGeneral * frecG + desfaceG + desfaseLed) * ampG + offG ;
      float b = sin(clockGeneral * frecB + desfaceB + desfaseLed) * ampB + offB ;
            
      if (r <= thresh) r = 0.0;
      if (g <= thresh) g = 0.0;
      if (b <= thresh) b = 0.0;
      if (distancia) {
        float d = ( abs(i - indice)) / float(cantPixeles - 1);

        if (distLuz) {
          d = 1. - pow(d, potencia);
        }
        r *= d;
        g *= d;
        b *= d;
        Serial.println("-----D----");
        Serial.println(pow(d, potencia));
      }
      sumatoriasR[i] =  sumatoriasR[i] * f +  (1. - f) * r ;
      sumatoriasG[i] =  sumatoriasG[i] * f +  (1. - f) * g;
      sumatoriasB[i] =  sumatoriasB[i] * f +  (1. - f) * b;

      r = constrain( sumatoriasR[i] * 255. * brilloGeneral + offGeneral, 0., 255.0) ;
      g = constrain( sumatoriasG[i] * 255. * brilloGeneral + offGeneral , 0., 255.0) ;
      b = constrain(sumatoriasB[i] * 255. * brilloGeneral + offGeneral, 0., 255.0) ;

      strip.setPixelColor(i, strip.Color( r, g, b));        //  Set pixel's color (in RAM)
      //Serial.println(brillo*degrade);
    /*    
     Serial.println("-----SUMAS----");
    Serial.println( sumatoriasR[i]);
    Serial.println(sumatoriasG[i] );
    Serial.println(  sumatoriasB[i]);
    */
    }
    strip.show();

    marcaTiempoFrec =  millis();
  
    /*    

    Serial.println("-----RGB----");
    Serial.println(r);
    Serial.println(g);
    Serial.println( b);
    Serial.println("---------------");
    Serial.println("---------------");
    */
  }


  if (tiempoActual > marcaTiempo + espera) {

    if (rampa) {
      indice++;
      if (indice == cantPixeles - 1)indice = 0;
    } else {
      if (indice == cantPixeles - 1)direccion = !direccion;
      if (indice == 0 && !direccion)direccion = !direccion;

      if (direccion)indice ++;
      else indice--;
    }
    marcaTiempo = millis();

  }
}
