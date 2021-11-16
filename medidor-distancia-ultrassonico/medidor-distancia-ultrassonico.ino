#include <Ultrasonic.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define PINO_TRIGGER 3
#define PINO_ECHO 2
#define PINO_BT_UN 4
#define TAXA_ATUALIZACAO 1000

enum Unidades {
  CMT,
  POL,
  MT,
  PES
};

Ultrasonic sensor(PINO_TRIGGER, PINO_ECHO);
LiquidCrystal_I2C lcd(0x27, 20, 4);
Unidades unidade = CMT;

void setup()
{
  pinMode(PINO_BT_UN, INPUT);
  lcd.init();
  lcd.backlight();
}

void loop()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Distancia:"));
  lcd.setCursor(0, 1);
  
  float distancia = sensor.read();
  String texto = "";
  switch (unidade) {
    case POL:
      texto+=String(distancia /= 2.51) + " pol";
      break;
    case MT:
      texto+=String(distancia /= 100.00) + " m";
      break;
    case PES:
      texto+=String(distancia /= 30,12) + " pes";
      break;
    default:
      texto+=String(distancia) + " cm";
  }
  
  lcd.print(texto);

  long tempo_ref = millis();
  while (millis() - tempo_ref < TAXA_ATUALIZACAO) {
    if (digitalRead(PINO_BT_UN)) {
      unidade = unidade < PES ? unidade + 1 : CMT;
      while (digitalRead(PINO_BT_UN));
      break;
    }
  }
}
