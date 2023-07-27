#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define TAU 2.0*PI

int LED_PIN = D6 ;



int cantPixeles  = 7;


long marcaTiempo, tiempoActual;
long marcaTiempoFrec, muestreo = 25;


float brilloGeneral = 1.0;
//frecuencia general del clock
float frecSeno = .0001;

//fecuencia
float frecR = PI;
float frecG = PI*.5;
float frecB = 1.;

float desfaceR = 0.;
float desfaceG = 0.33* TAU;
float desfaceB = 0.66* TAU;

//en rangos nomalizados es decir pensndolos de 0a 1.
//amplitud //reduciendo la onda a la mitad de -0.5 a 0.5
float ampR = .5;
float ampG = .5;
float ampB = .5;

//offset //para compensar o desplazar la onda hacia arriba
float offR = 0.5 ;
float offG = 0.5;
float offB = 0.5;




Adafruit_NeoPixel strip(cantPixeles, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  tiempoActual = millis();
  marcaTiempo = tiempoActual;
  marcaTiempoFrec = tiempoActual;
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  tiempoActual = millis();

  if (tiempoActual > marcaTiempoFrec + muestreo) {

    for (int i = 0 ; i < cantPixeles; i++) {

      float proporcion = float(i) / float(cantPixeles - 1);
      float desfaseLed =  proporcion * TAU; // dibuja un seno completo en la tira
      float clockGeneral = float(tiempoActual) * frecSeno;


      float r = sin(clockGeneral * frecR + desfaceR + desfaseLed) * ampR + offR ;
      float g = sin(clockGeneral * frecG + desfaceG + desfaseLed) * ampG + offG ;
      float b = sin(clockGeneral * frecB + desfaceB + desfaseLed) * ampB + offB ;

      r = constrain(r * 255. * brilloGeneral, 0., 255.0) ;
      g = constrain(g * 255. * brilloGeneral, 0., 255.0) ;
      b = constrain(b * 255. * brilloGeneral, 0., 255.0) ;

      strip.setPixelColor(i, strip.Color(  r,  g,  b));         //  Set pixel's color (in RAM)
      strip.show();
      //Serial.println(brillo*degrade);
    }
    marcaTiempoFrec = tiempoActual;
  }
}
