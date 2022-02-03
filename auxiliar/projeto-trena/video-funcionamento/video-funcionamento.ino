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
void imprimirDistancia();
void imprimirUnidade();
void calcularArea();

void setup() {
  pinMode(BT_CONVERSAO, INPUT_PULLUP);
  pinMode(BT_AREA, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
}

void loop() {

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

  Serial.print("Distancia: " + String(distancia) + " ");
  
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
    lcd.print(F("cm"));
    Serial.println(F("cm"));
    break;
    case POL:
    lcd.print(F("pol"));
    Serial.println(F("pol"));
    break;
    case MT:
    lcd.print(F("m"));
    Serial.println(F("m"));
    break;
    case PES:
    lcd.print(F("pes"));
    Serial.println(F("pes"));
    break;
  }
}

void calcularArea() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Calculo de area"));
  
  Serial.println(F("Calculo de area"));
  
  delay(500);

  float area = 0;
  float distancia;
  float mensurado;

  for(int i = 0; i < 2; i++) {
    distancia = 0;
    mensurado = false;
    while(mensurado == false) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(i == 0 ? F("Comprimento") : F("Largura"));
      
      Serial.print(i == 0 ? F("Comprimento") : F("Largura"));
      Serial.print(" ");
      
      lcd.setCursor(0, 1);
      distancia = medirDistancia();
      lcd.print(distancia < 0 ? F("fora de escala") : String(distancia));
      lcd.print(F(" "));

      Serial.print(distancia < 0 ? F("fora de escala") : String(distancia));
      
      imprimirUnidade();
  
      long t_ref = millis();
  
      while(millis() - t_ref < TAXA_ATUALIZACAO) {
        if(!digitalRead(BT_AREA)) {
          while(!digitalRead(BT_AREA));
          area = i==0 ? distancia : distancia*area;
          mensurado = true;
        }
      }
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Area"));
  lcd.setCursor(0, 1);
  lcd.print(area);
  lcd.print(F(" "));
  Serial.println("Area: " + String(area) + " ");
  imprimirUnidade();
  lcd.print(F("2"));

  while(digitalRead(BT_AREA));
}
