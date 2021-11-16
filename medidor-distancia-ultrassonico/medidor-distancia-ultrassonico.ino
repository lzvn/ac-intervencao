#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Ultrasonic.h>

#define PINO_TRIGGER 3
#define PINO_ECHO 2
#define PINO_BOTAO 4
#define TAXA_ATUALIZACAO 1000

enum Unidades {
  CM,
  POL,
  M,
  PES
};

Ultrasonic sensorUltrassonico(PINO_TRIGGER, PINO_ECHO);
LiquidCrystal_I2C lcd(0x27, 20, 4);
Unidades unidade = CM;
long tempo_ref = 0;

float calcularDistancia(); //calcula em cm
void imprimirDistancia(float distancia);
 
void setup()
{
  pinMode(PINO_BOTAO, INPUT);
  lcd.init();
  lcd.backlight();
}
 
void loop()
{
  float distancia = medirDistancia();
  imprimirDistancia(distancia);

  tempo_ref = millis();
  while(millis       () - tempo_ref < TAXA_ATUALIZACAO) {
    if(digitalRead(PINO_BOTAO)) {
      unidade = unidade<PES?unidade+1:CM;
      while(digitalRead(PINO_BOTAO));
      break;
    }
  }
}


float medirDistancia() {
  long tempo = sensorUltrassonico.timing();
  float distancia = sensorUltrassonico.convert(tempo, Ultrasonic::CM);
  return distancia;
}

void imprimirDistancia(float distancia) {
  String str = "";
  switch(unidade) {
    case POL:
      str += String(distancia/2.51) + " pol.";
      break;
    case M:
      str += String(distancia/100.0) + " m";
      break;
    case PES:
      str += String(distancia/30.12) + " pes";
      break;
    default:
      str += String(distancia) + " cm";
  }
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Distancia:"));
  lcd.setCursor(0, 1);
  lcd.print(str);
}
