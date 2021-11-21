#include <Ultrasonic.h>
#include <LiquidCrystal_I2C.h>
//#include <Wire.h>

#define PINO_TRIGGER 3
#define PINO_ECHO 2

#define BT_CONVERSAO 4
#define BT_AREA 5
#define BT_PITAGORAS 6

#define TAXA_ATUALIZACAO 1000
#define DISTANCIA_MAX 400 //4m

enum Unidades {
  CMT,
  POL,
  MT,
  PES
};

Ultrasonic sensor(PINO_TRIGGER, PINO_ECHO);
LiquidCrystal_I2C lcd(0x27, 20, 4);
Unidades unidade = CMT;

float medirDistancia();
void calcularArea();
void calcularPitagoras();

float converter(float distancia); //Converte de cm para qualquer outra unidade

void imprimir(String linha1, String linha2);
void imprimirDistancia(float distancia);

void setup()
{
  pinMode(BT_CONVERSAO, INPUT_PULLUP);
  pinMode(BT_AREA, INPUT_PULLUP);
  pinMode(BT_PITAGORAS, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
}

void loop()
{
  float distancia = medirDistancia();
  imprimirDistancia(distancia);
  
  long tempo_ref = millis();
  while (millis() - tempo_ref < TAXA_ATUALIZACAO) {
    if (!digitalRead(BT_CONVERSAO)) {
      unidade = unidade < PES ? unidade + 1 : CMT;
      while (!digitalRead(BT_CONVERSAO));
      break;
    }
    
    if(!digitalRead(BT_AREA)) {
      calcularArea();
      break;
    }
    
    if(!digitalRead(BT_PITAGORAS)) {
      calcularPitagoras();
      break;
    }
  }
}

float medirDistancia() {
  return sensor.read();
}

void calcularArea() {
  
}

void calcularPitagoras() {
  
}

float converter(float distancia) {
  switch(unidade) {
    case CMT:
    return distancia;
    case POL:
    return distancia/2.51;
    case MT:
    return distancia/100.0;
    case PES:
    return distancia/30.12;
  }
}

void imprimir(String linha1, String linha2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(linha1);
  lcd.setCursor(0, 1);
  lcd.print(linha2);
}

void imprimirDistancia(float distancia) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Distancia"));
  
  lcd.setCursor(0, 1);

  if(distancia > DISTANCIA_MAX) {
    lcd.print(F("fora de escala"));
    return;
  }
  
  distancia = converter(distancia);
  lcd.print(String(distancia));
  lcd.print(F(" "));

  switch(unidade) {
    case CMT:
    lcd.print(F("cm"));
    case POL:
    lcd.print(F("pol"));
    case MT:
    lcd.print(F("m"));
    case PES:
    lcd.print(F("pes"));
  }
}
