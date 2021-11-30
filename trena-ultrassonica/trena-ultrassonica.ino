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

void imprimirDistancia(float distancia);
void imprimirUnidade(boolean area = false);
void transicaoTela(String str);

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
      while(!digitalRead(BT_AREA));
      break;
    }
    
    if(!digitalRead(BT_PITAGORAS)) {
      calcularPitagoras();
      while(!digitalRead(BT_PITAGORAS));
      break;
    }
  }
}

float medirDistancia() {
  return sensor.read();
}

void calcularArea() {
  while(!digitalRead(BT_AREA));

  transicaoTela("Calculo de area");  
  float area = 0;

  for(int i = 0; i < 2; i++) {
    transicaoTela(i==0?"Comprimento":"Largura");
    
    boolean distanciaMedida = false;
    while(!distanciaMedida) {
      float distancia = medirDistancia();
      imprimirDistancia(distancia);

      int tempo_ref = millis();
      while(millis() - tempo_ref < TAXA_ATUALIZACAO) {
        if(!digitalRead(BT_AREA)) {
          area = i==0 ? distancia: area*distancia;
          distanciaMedida=true;
          while(!digitalRead(BT_AREA));
          break;
        }
      }
    }
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Area "));
  lcd.setCursor(0, 1);
  lcd.print(String(area));
  lcd.print(F(" "));
  imprimirUnidade(true);

  while(digitalRead(BT_AREA));
}

void calcularPitagoras() {
  while(!digitalRead(BT_PITAGORAS));

  transicaoTela("Calc. Pitagórico");

  float altura = 0;

  for(int i = 0; i < 2; i++) {
    transicaoTela(i==0?"Hipotenusa":"Cateto");
    
    boolean distanciaMedida = false;
    while(!distanciaMedida) {
      float distancia = medirDistancia();
      imprimirDistancia(distancia);

      int tempo_ref = millis();
      while(millis() - tempo_ref < TAXA_ATUALIZACAO) {
        if(!digitalRead(BT_PITAGORAS)) {
          altura = i==0 ? distancia: sqrt(altura*altura - distancia*distancia);
          distanciaMedida=true;
          while(!digitalRead(BT_PITAGORAS));
          break;
        }
      }
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Altura "));
  lcd.setCursor(0, 1);
  lcd.print(String(altura));
  lcd.print(F(" "));
  imprimirUnidade();

  while(digitalRead(BT_PITAGORAS));
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
  imprimirUnidade();
}

void imprimirUnidade(boolean area = false) {
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
  if(area) {
    lcd.print(F("^2"));
  }
}

void transicaoTela(String str) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(str);
  delay(700);
}
