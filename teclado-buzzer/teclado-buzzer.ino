//notas e suas frequências
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494

//constantes
#define DURACAO_NOTA 50
#define NUM_TECLAS 12
#define BUZZER 2

//representação de uma tecla, com o pino em que estão localizados e sua nota
struct Tecla {
  int pino;
  int nota;

  Tecla(int novoPino, int novaNota) {
    pino = novoPino;
    nota = novaNota;
  }
};

//representação do teclado
Tecla teclas[NUM_TECLAS] = {
  Tecla(13, NOTE_C4),
  Tecla(12, NOTE_CS4),
  Tecla(11, NOTE_D4),
  Tecla(10, NOTE_DS4),
  Tecla(9, NOTE_E4),
  Tecla(8, NOTE_F4),
  Tecla(7, NOTE_FS4),
  Tecla(6, NOTE_G4),
  Tecla(5, NOTE_GS4),
  Tecla(4, NOTE_A4),
  Tecla(3, NOTE_AS4),
  Tecla(A0, NOTE_B4),
};

void setup() {
  //definição dos pinos
  for(int i = 0; i < NUM_TECLAS; i++) {
    pinMode(teclas[i].pino, INPUT_PULLUP);
  }

  pinMode(BUZZER, OUTPUT);
}

void loop() {
  //verificação de todas as teclas por alguma pressionada
  for(int i = 0; i < NUM_TECLAS; i++) {
    //se alguma for pressionada, toca-se a nota correspondente
    if(digitalRead(teclas[i].pino) == LOW) {
      tone(BUZZER, teclas[i].nota, DURACAO_NOTA);
      delay(DURACAO_NOTA);
      break;
    }
  }
}
