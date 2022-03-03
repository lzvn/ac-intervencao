#include <Ultrasonic.h>
#include <LiquidCrystal_I2C.h>

//pinos do sensor
#define PINO_TRIGGER 3
#define PINO_ECHO 2

//pinos dos botões
#define BT_CONVERSAO 4 //botão de conversão de unidades
#define BT_AREA 5 //botão de cálculo de área

#define TAXA_ATUALIZACAO 1000
#define DISTANCIA_MAX 350 //3,5m

//unidades de medida: centímetros, polegadas, metros e pés
enum Unidades {
  CMT,
  POL,
  MT,
  PES
};

Ultrasonic sensor(PINO_TRIGGER, PINO_ECHO); //objeto para uso do sensor ultrassônico
LiquidCrystal_I2C lcd(0x27, 20, 4); //objeto para uso do display
Unidades unidade = CMT; //centímetro é a unidade padrão

float medirDistancia();
void imprimirUnidade();
void calcularArea();

void setup() {
  pinMode(BT_CONVERSAO, INPUT_PULLUP);
  pinMode(BT_AREA, INPUT_PULLUP);

  //inicialização do display
  lcd.init();
  lcd.backlight();
}

void loop() {

  //limpeza do display e escrita do distância medida
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Distancia");
  
  lcd.setCursor(0, 1);

  float distancia = medirDistancia(); //medição

  //mensagem em caso de distâncias fora da escala
  if(distancia < 0) {
     lcd.print("fora de escala");
     delay(TAXA_ATUALIZACAO);
     return;
  }

  //escrita da distância e unidade no display
  lcd.print(String(distancia) + " ");
  imprimirUnidade();

  //verificação dos botões no restante do tempo antes de atualização do display
  long tempo_ref = millis(); //referência de tempo
  
  while (millis() - tempo_ref < TAXA_ATUALIZACAO) {
    //mudança da unidade de medida
    if (digitalRead(BT_CONVERSAO) == LOW) {
      unidade = unidade < PES ? unidade + 1 : CMT;
      while (!digitalRead(BT_CONVERSAO));
      break;
    }

    //entrada da função de cálculo de área
    if(digitalRead(BT_AREA) == LOW) {
      calcularArea();
      while(!digitalRead(BT_AREA));
      break;
    }
  }
}

float medirDistancia() {
  float distancia = sensor.read(); //medição

   //verificação de distância acima da máxima
   if(distancia > DISTANCIA_MAX) {
    return -1.0; //em caso de distância acima da máxima, o valor de -1 é retornado como código de erro
  }

  //conversão da unidade de medida para a saída
  switch(unidade) {
    case POL:
    distancia /= 2.51;
    break;
    case MT:
    distancia /= 100.0;
    break;
    case PES:
    distancia /= 30.12; //divisão por 2.51 para converter para polegadas e por 12 para converter para pés
    break;
  }

  return distancia;
}

//impressão da unidade de medida conforme a selecionada
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

//cálculo de área
void calcularArea() {

  //confirmação da seleção
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Calculo de area");
  delay(500);

  //variáveis dos lados
  float a;
  float b;
  float distancia;
  
  boolean mensurado; //variável para confirmação da medição dos lados

  //loop para medição dos lados da sala para cálculo de área
  //quando i=0, mede-se um lado, quando i=1, mede-se outro lado
  for(int i = 0; i < 2; i++) {
    //reset das variáveis
    distancia = 0;
    mensurado = false;

    //enquanto o usuário não confirma que mediu a distância respectiva, o arduino continua a medição
    while(mensurado == false) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(i == 0 ? "Comprimento" : "Largura"); //quando i=0, mostra-se comprimento, quando i=1, mostra-se largura
      lcd.setCursor(0, 1);
      distancia = medirDistancia();

      //verificação se o valor medido está dentro do valor máximo e, caso sim, impressão da distância medida no display
      if(distancia < 0) {
        lcd.print("fora de escala");
        continue;
      } else {
        lcd.print(String(distancia) + " ");
        imprimirUnidade();
      }
  
      long t_ref = millis(); //referência de tempo para espera até a próxima atualização
  
      while(millis() - t_ref < TAXA_ATUALIZACAO) {
        //confirmação do usuário que a distância foi medida ao pressionar o botão de cálculo de área
        if(!digitalRead(BT_AREA)) {
          while(!digitalRead(BT_AREA));
          if(i == 0) {
            a = distancia;
          } else {
            b = distancia;
          }

          //mensurado tem o valor trocado para verdadeiro pois houve confirmação, indicando que o programa
          //deve seguir para o próximo lado ou encerrar o loop e mostrar a área calculada
          mensurado = true; 
        }
      }
    }
  }

  //impressão do valor de área calculado pelo produto do comprimento e da largura
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Area");
  lcd.setCursor(0, 1);
  lcd.print(a*b);
  lcd.print(" ");
  imprimirUnidade();
  lcd.print("2");

  while(digitalRead(BT_AREA)); //espera para o usuário sair da função e retornar a medição normal da trena
}
