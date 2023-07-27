#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

int LED_PIN = D6 ;
float potencia = 10;
float distanciaGeneral = 1. ;
int cantPixeles =  7;
float distancias[7] ;
float filtroD = .9995;
long marcaTiempo, tiempoActual;
long marcaTiempoFrec, muestreo = 25;
float thresh = 0.; //en num normalizados
int espera = 250;
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
  for (int i = 0 ; i < cantPixeles; i++) {
    distancias[i] = .5;
  }
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
  // contador muestreo de distancia y led
  if (tiempoActual > marcaTiempoFrec + muestreo) {
    for (int i = 0 ; i < cantPixeles; i++) {

      // float distancia = (sin(float(tiempoActual) * frecSeno * PI * 2. ) * 0.5 + .5) * 255.;
      float distancia = abs(i - indice) / float(cantPixeles - 1) ;
      if (distancia <= thresh) distancia = 0.0;
      distancia *= distanciaGeneral;
      distancias[i] = distancias[i] * filtroD + (1. - filtroD) * distancia;

      Serial.println(distancia);
      Serial.println(distancias[i]);
        distancia = distancia * 255.;

        strip.setPixelColor(i, strip.Color(  distancia, distancia, distancia));         //  Set pixel's color (in RAM)
      }
    strip.show();

    marcaTiempoFrec = tiempoActual;
  }
}
