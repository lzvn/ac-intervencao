#define BOTAO 2
#define LED 13

void setup() {
  //pinMode(BOTAO, INPUT);
  pinMode(BOTAO, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
}

void loop() {
  //if(digitalWrite(BOTAO)) { //para o caso do botão sem pull-up
  if(!digitalWrite(BOTAO)) { //botao com pull-up
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
}
