#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define TAU 2.0*PI

#define SUMA_INI .75
#define CANT_LEDS 7

int LED_PIN = D6 ;

float brilloGeneral = .65;

//frecuencia general del clock
float velClock = .000015;
float velOscGeneral = .0005;

float f = 0.65;
float filtroSeno = .95;



float offGeneral = 0.0; //agregarle T?
float amplitudDesfase = .5;

int espera = 200;
int  muestreo = 20;


float frecBrillo=.5;
float frecR = PI * .01;
float frecG = -PI * .01;
float frecB = .01;
//desfaces
float desfaceR = 0.50 * TAU;
float desfaceG = 0.33 * TAU;
float desfaceB = 0.0 * TAU;

//en rangos nomalizados es decir pensndolos de 0a 1.
//amplitud //reduciendo la onda a la mitad de -0.5 a 0.5
float ampR = .5;
float ampG = .275;
float ampB = .3;
//offset //para compensar o desplazar la onda hacia arriba
float offR = 0.5 ;
float offG = 0.25;
float offB = 0.3;

float thresh = .0; //en num normalizados

//VARIABLES OPERATIVAS
int cantPixeles =  CANT_LEDS;

long marcaTiempo, tiempoActual;
long marcaTiempoFrec;

float  sumatoriasSeno[CANT_LEDS];
float  sumatoriasR[CANT_LEDS];
float  sumatoriasG[CANT_LEDS];
float  sumatoriasB[CANT_LEDS];

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
    sumatoriasSeno[i] = SUMA_INI;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  tiempoActual = millis();
  float tSeno = tiempoActual * velOscGeneral;

  if (tiempoActual > marcaTiempoFrec + muestreo) {

    for (int i = 0 ; i < cantPixeles; i++) {

      float proporcion = float(i) / float(cantPixeles - 1);
      float desfaseLed =  proporcion * TAU * amplitudDesfase; // dibuja un seno completo en la tira
      float clockGeneral = float(tiempoActual) * velClock;

      float seno = 1. - ( sin ( i / float(cantPixeles - 1) * PI * 2. *frecBrillo + tSeno) * .5 + .5 ) ;
      sumatoriasSeno[i] = sumatoriasSeno[i] * filtroSeno + (1. - filtroSeno) * seno;
    
      float r = sin(clockGeneral * frecR + desfaceR + desfaseLed) * ampR + offR ;
      float g = sin(clockGeneral * frecG + desfaceG + desfaseLed) * ampG + offG ;
      float b = sin(clockGeneral * frecB + desfaceB + desfaseLed) * ampB + offB ;

      if (r <= thresh) r = 0.0;
      if (g <= thresh) g = 0.0;
      if (b <= thresh) b = 0.0;

      sumatoriasR[i] =  sumatoriasR[i] * f +  (1. - f) * r;
      sumatoriasG[i] =  sumatoriasG[i] * f +  (1. - f) * g;
      sumatoriasB[i] =  sumatoriasB[i] * f +  (1. - f) * b;

      r = constrain(sumatoriasR[i] * 255. * brilloGeneral + offGeneral, 0., 255.0) *  sumatoriasSeno[i]  ;
      g = constrain(sumatoriasG[i] * 255. * brilloGeneral + offGeneral, 0., 255.0) *  sumatoriasSeno[i]  ;
      b = constrain(sumatoriasB[i] * 255. * brilloGeneral + offGeneral, 0., 255.0) *  sumatoriasSeno[i]  ;

      strip.setPixelColor(i, strip.Color( r, g, b));        //  Set pixel's color (in RAM)

    }
    strip.show();
    
    marcaTiempoFrec =  millis();

  }

/*
  if (tiempoActual > marcaTiempo + espera) {
   //   tSeno+=0.5;
    marcaTiempo = millis();
  }
  */
}
