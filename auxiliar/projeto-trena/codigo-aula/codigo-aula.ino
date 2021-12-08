#include <Ultrasonic.h>
#include <LiquidCrystal_I2C.h>

#define PINO_TRIGGER 3
#define PINO_ECHO 2

#define BT_CONVERSAO 4
#define BT_ALTURA 5

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
void imprimirDistancia();
void calcularAltura();
void imprimirUnidade();

void setup() {
  pinMode(BT_CONVERSAO, INPUT_PULLUP);
  pinMode(BT_ALTURA, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
}

void loop() {

  imprimirDistancia();

  long tempo_ref = millis();
  while (millis() - tempo_ref < TAXA_ATUALIZACAO) {
    if (!digitalRead(BT_CONVERSAO)) {
      while (!digitalRead(BT_CONVERSAO));
      unidade = unidade < PES ? unidade + 1 : CMT;
      break;
    }

    if(!digitalRead(BT_ALTURA)) {
      while(!digitalRead(BT_ALTURA));
      calcularAltura();
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
    case MT:
    distancia /= 100.0;
    case PES:
    distancia /= 30.12;
  }

  return distancia;
}

void imprimirDistancia() {
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Distancia"));
  
  lcd.setCursor(0, 1);

  float distancia = medirDistancia();

  if(distancia < 0) {
     lcd.print(F("fora de escala"));
     return;
  }
  
  lcd.print(String(distancia));
  lcd.print(F(" "));
   imprimirUnidade();
}

void calcularAltura() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Calculo de altura"));
  delay(500);

  boolean mensurado = false;
  float distancia = 0;
  float altura = 0;

  while(mensurado == false) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Cateto"));
    lcd.setCursor(0, 1);
    distancia = medirDistancia();
    lcd.print(distancia);
    lcd.print(F(" "));
    imprimirUnidade();

    long t_ref = millis();

    while(millis() - t_ref < TAXA_ATUALIZACAO) {
      if(!digitalRead(BT_ALTURA)) {
        while(!digitalRead(BT_ALTURA));
        altura = distancia;
        mensurado = true;
      }
    }
  }

  mensurado = false;

  while(digitalRead(BT_ALTURA)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Hipotenusa"));
    lcd.setCursor(0, 1);
    distancia = medirDistancia();
    lcd.print(distancia);
    lcd.print(F(" "));
    imprimirUnidade();

    long t_ref = millis();

    while(millis() - t_ref < TAXA_ATUALIZACAO) {
      if(!digitalRead(BT_ALTURA)) {
        while(!digitalRead(BT_ALTURA));
        altura = distancia*distancia - altura*altura;
        mensurado = true;
      }
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Altura"));
  lcd.setCursor(0, 1);
  lcd.print(altura);
  lcd.print(F(" "));
  imprimirUnidade();

  while(digitalRead(BT_ALTURA));
  
}

void imprimirUnidade() {
  switch(unidade) {
    case CMT:
    lcd.print(F("cm"));
    break;
    case POL:
    lcd.print(F("pol"));
    break;
    case MT:
    lcd.print(F("m"));
    break;
    case PES:
    lcd.print(F("pes"));
    break;
  }
}

 Serial.print(F("Hello World!"));
 Serial.println(F("Hello World!"));
