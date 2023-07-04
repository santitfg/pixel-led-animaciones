#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN     6
int cantPixeles  7
long marcaTiempo, tiempoActual;
long marcaTiempoFrec, muestreo = 25;
int espera = 500;
int indice = 0;
float frecSeno = 0.0005;
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

}

void loop() {
  // put your main code here, to run repeatedly:
  tiempoActual = millis();

  //contador desplazamiento led
  if (tiempoActual > marcaTiempo + espera) {
    strip.setPixelColor(indice, strip.Color(  0, 0, 0));       //  Set pixel's color (in RAM)

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

  // contador muestreo de brillo y led
  if (tiempoActual > marcaTiempoFrec + muestreo) {
      float brillo = (sin(float(tiempoActual)* frecSeno * PI*2. ) * 0.5 + .5) * 255.;

      strip.setPixelColor(indice-1, strip.Color(  brillo*0.5, brillo*0.5, brillo*0.5));         //  Set pixel's color (in RAM)
      strip.setPixelColor(indice, strip.Color(  brillo, brillo, brillo));         //  Set pixel's color (in RAM)
      strip.setPixelColor(indice+1, strip.Color(  brillo*0.5, brillo*0.5, brillo*0.5));         //  Set pixel's color (in RAM)
      strip.show();
    
    marcaTiempoFrec = tiempoActual;
  }
}
