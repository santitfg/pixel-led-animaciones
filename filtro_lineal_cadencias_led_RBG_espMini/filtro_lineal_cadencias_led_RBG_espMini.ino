#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define CANT_LEDS 45
int LED_PIN = D6 ;
int cantPixeles =  CANT_LEDS;
float  sumatoriasR[CANT_LEDS];
float  sumatoriasG[CANT_LEDS];
float  sumatoriasB[CANT_LEDS];
float f = 0.995;
float potencia = 20; // para la ecuacion de inversa de la distancia
float thresh = .0; //en num normalizados
float brilloGeneral = 1. ;
long marcaTiempoR, marcaTiempoG, marcaTiempoB;
long tiempoActual;
long marcaTiempoFrec, muestreo = 25;
int esperaR = 500, esperaG = 1000 , esperaB = 1500;
int indiceR = 0;
int indiceG = 0;
int indiceB = 0;
//float frecSeno = 0.0005;
bool direccionR = true;
bool direccionG = true;
bool direccionB = true;
bool rampa = false;

Adafruit_NeoPixel strip(cantPixeles, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  tiempoActual = millis();
  marcaTiempoR = tiempoActual;
  marcaTiempoG = tiempoActual;
  marcaTiempoB = tiempoActual;
  marcaTiempoFrec = tiempoActual;
  Serial.begin(115200);

  for (int i = 0 ; i < cantPixeles; i++) {
    sumatoriasR[i] = 0.;
    sumatoriasG[i] = 0.;
    sumatoriasB[i] = 0.;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  tiempoActual = millis();

  //contador desplazamiento led
  if (tiempoActual > marcaTiempoR + esperaR) {
    if (rampa) {
      indiceR++;
      if (indiceR == cantPixeles - 1)indiceR = 0;
    } else {
      if (indiceR == cantPixeles - 1)direccionR = !direccionR;
      if (indiceR == 0 && !direccionR)direccionR = !direccionR;
      if (direccionR)indiceR ++;
      else indiceR--;
    }
    marcaTiempoR = millis();
  }
  if (tiempoActual > marcaTiempoG + esperaG) {
    if (rampa) {
      indiceG++;
      if (indiceG == cantPixeles - 1)indiceG = 0;
    } else {
      if (indiceG == cantPixeles - 1)direccionG = !direccionG;
      if (indiceG == 0 && !direccionG)direccionG = !direccionG;
      if (direccionG)indiceG ++;
      else indiceG--;
    }
    marcaTiempoG = millis();
  }
  if (tiempoActual > marcaTiempoB + esperaB) {
    if (rampa) {
      indiceB++;
      if (indiceB == cantPixeles - 1)indiceB = 0;
    } else {
      if (indiceB == cantPixeles - 1)direccionB = !direccionB;
      if (indiceB == 0 && !direccionB)direccionB = !direccionB;
      if (direccionB)indiceB ++;
      else indiceB--;
    }
    marcaTiempoB = millis();
  }
  //(cantPixeles - 1) - indice
  // contador muestreo de brillo y led
  if (tiempoActual > marcaTiempoFrec + muestreo) {
    for (int i = 0 ; i < cantPixeles; i++) {

      // float brillo = (sin(float(tiempoActual) * frecSeno * PI * 2. ) * 0.5 + .5) * 255.;
      float r = pow((float(cantPixeles - 1) - abs(i - indiceR)) / float(cantPixeles - 1), potencia) ;
      float g = pow((float(cantPixeles - 1) - abs(i - indiceG)) / float(cantPixeles - 1), potencia) ;
      float b = pow((float(cantPixeles - 1) - abs(i - indiceB)) / float(cantPixeles - 1), potencia) ;
      
      if (r <= thresh) r = 0.0;
      if (g <= thresh) g = 0.0;
      if (b <= thresh) b = 0.0;
      
      r *= 255. *brilloGeneral ;
      g *= 255. *brilloGeneral ;
      b *= 255. *brilloGeneral ;
      
      sumatoriasR[i] =  sumatoriasR[i]*f +  (1.-f)*r;
      sumatoriasG[i] =  sumatoriasG[i]*f +  (1.-f)*g;
      sumatoriasB[i] =  sumatoriasB[i]*f +  (1.-f)*b;
      strip.setPixelColor(i, strip.Color(  sumatoriasR[i], sumatoriasG[i], sumatoriasB[i]));         //  Set pixel's color (in RAM)
    }
    strip.show();

    marcaTiempoFrec = tiempoActual;
  }
}
