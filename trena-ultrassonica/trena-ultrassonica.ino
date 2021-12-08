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

void calcularPitagoras();

float converter(float distancia); //Converte de cm para qualquer outra unidade

void imprimirDistancia(float distancia);
void imprimirUnidade();
void transicaoTela(String str);

void setup() {
  pinMode(BT_CONVERSAO, INPUT_PULLUP);
  pinMode(BT_ALTURA, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
}

void loop()
{
  float distancia = sensor.read();
  imprimirDistancia(distancia);
  
  long tempo_ref = millis();
  while (millis() - tempo_ref < TAXA_ATUALIZACAO) {
    if (!digitalRead(BT_CONVERSAO)) {
      unidade = unidade < PES ? unidade + 1 : CMT;
      while (!digitalRead(BT_CONVERSAO));
      break;
    }
    
    if(!digitalRead(BT_ALTURA)) {
      calcularPitagoras();
      while(!digitalRead(BT_ALTURA));
      break;
    }
  }
}

void calcularPitagoras() {
  while(!digitalRead(BT_ALTURA));

  transicaoTela("Calc. Pitagórico");

  float altura = 0;

  for(int i = 0; i < 2; i++) {
    transicaoTela(i==0?"Hipotenusa":"Cateto");
    
    boolean distanciaMedida = false;
    while(!distanciaMedida) {
      float distancia = sensor.read();
      imprimirDistancia(distancia);

      int tempo_ref = millis();
      while(millis() - tempo_ref < TAXA_ATUALIZACAO) {
        if(!digitalRead(BT_ALTURA)) {
          altura = i==0 ? distancia: sqrt(altura*altura - distancia*distancia);
          distanciaMedida=true;
          while(!digitalRead(BT_ALTURA));
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

  while(digitalRead(BT_ALTURA));
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

void transicaoTela(String str) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(str);
  delay(700);
}
