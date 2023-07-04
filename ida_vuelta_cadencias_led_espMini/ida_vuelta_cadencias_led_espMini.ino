#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

int LED_PIN = D6 ;
float potencia =10;
float brilloGeneral = 1. ;
int cantPixeles =  45;
long marcaTiempo, tiempoActual;
long marcaTiempoFrec, muestreo = 25;
float thresh = 0.; //en num normalizados
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
  Serial.begin(115200);

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

  //(cantPixeles - 1) - indice
  // contador muestreo de brillo y led
  if (tiempoActual > marcaTiempoFrec + muestreo) {
    for (int i = 0 ; i < cantPixeles; i++) {

      // float brillo = (sin(float(tiempoActual) * frecSeno * PI * 2. ) * 0.5 + .5) * 255.;
      float brillo = pow((float(cantPixeles - 1) - abs(i - indice)) / float(cantPixeles - 1), potencia) ;
      if(brillo <= thresh) brillo=0.0;
      brillo*= 255.;
      brillo *= brilloGeneral;
      
      Serial.println(brillo);

      strip.setPixelColor(i, strip.Color(  brillo, brillo, brillo));         //  Set pixel's color (in RAM)
    }
    strip.show();

    marcaTiempoFrec = tiempoActual;
  }
}
