#include <Ultrasonic.h>
#include <LiquidCrystal_I2C.h>

#define PINO_TRIGGER 2
#define PINO_ECHO 3

#define BT_CONVERSAO 4
#define BT_AREA 5

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
  pinMode(BT_AREA, INPUT_PULLUP);
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

    if(digitalRead(BT_AREA) == LOW) {
      calcularArea();
      while(!digitalRead(BT_AREA));
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

void calcularArea() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Calculo de area");
  delay(500);

  float a;
  float b;
  boolean mensurado;

  for(int i = 0; i < 2; i++) {
    distancia = 0;
    mensurado = false;
    while(mensurado == false) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(i == 0 ? "Comprimento" : "Largura");
      lcd.setCursor(0, 1);
      distancia = medirDistancia();
      
      if(distancia < 0) {
        lcd.print("fora de escala");
        continue;
      } else {
        lcd.print(String(distancia) + " ");
        imprimirUnidade();
      }
  
      long t_ref = millis();
  
      while(millis() - t_ref < TAXA_ATUALIZACAO) {
        if(!digitalRead(BT_AREA)) {
          while(!digitalRead(BT_AREA));
          if(i == 0) {
            a = distancia;
          } else {
            b = distancia;
          }
          mensurado = true;
        }
      }
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Area");
  lcd.setCursor(0, 1);
  lcd.print(a*b);
  lcd.print(" ");
  imprimirUnidade();
  lcd.print("2");

  while(digitalRead(BT_AREA));
}
