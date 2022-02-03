#include <Ultrasonic.h>
#include <LiquidCrystal_I2C.h>

#define PINO_TRIGGER 2
#define PINO_ECHO 3

#define BT_CONVERSAO 4

#define TAXA_ATUALIZACAO 1000
#define DISTANCIA_MAX 350 //3,5m

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
void imprimirUnidade();
void calcularArea();

void setup() {
  pinMode(BT_CONVERSAO, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
}

void loop() {

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Distancia");
  lcd.setCursor(0, 1);

  float distancia = medirDistancia();
  if(distancia < 0) {
     lcd.print("fora de escala");
     return;
  }
  lcd.print(String(distancia) + " ");
  imprimirUnidade();

  long tempo_ref = millis();
  while (millis() - tempo_ref < TAXA_ATUALIZACAO) {
    if (digitalRead(BT_CONVERSAO) == LOW) {
      unidade = unidade < PES ? unidade + 1 : CMT;
      while (!digitalRead(BT_CONVERSAO));
      break;
    }
  }
}

float medirDistancia() {
  float distancia = sensor.read();

   if(distancia > DISTANCIA_MAX) {
    return -1.0;
  }

  switch(unidade) {
    case POL:
    distancia /= 2.51;
    break;
    case MT:
    distancia /= 100.0;
    break;
    case PES:
    distancia /= 30.12;
    break;
  }

  return distancia;
}

void imprimirUnidade() {
  switch(unidade) {
    case CMT:
    lcd.print("cm");
    break;
    case POL:
    lcd.print("pol");
    break;
    case MT:
    lcd.print("m");
    break;
    case PES:
    lcd.print("pes");
    break;
  }
}
