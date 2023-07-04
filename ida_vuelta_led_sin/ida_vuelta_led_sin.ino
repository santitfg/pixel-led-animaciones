#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN     6
int cantPixeles  =7;
long marcaTiempo, tiempoActual;
long marcaTiempoFrec, muestreo = 25;
int espera = 500;
int indice = 0;
float frecSeno = 0.001;
bool direccion = true;
bool rampa = false;

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



  // contador muestreo de brillo y led
  if (tiempoActual > marcaTiempoFrec + muestreo) {
    for (int i = 0 ; i < cantPixeles; i++) {
      float proporcion =float(i)/float(cantPixeles-1);
      float brillo = (1.-(sin(float(tiempoActual)* frecSeno + proporcion * PI*2. ) * 0.5 + .5) )* 255.;
   //  float degrade = 1.- abs(proporcion -.5); //guarda que esto no funciona bien
      strip.setPixelColor(i, strip.Color(  brillo,  brillo,  brillo));         //  Set pixel's color (in RAM)
      strip.show();
      //Serial.println(brillo*degrade);
    }
    marcaTiempoFrec = tiempoActual;
  }
}
