#define BUZZER 2
#define INTERVALO 100

void setup() {
  pinMode(BUZZER, OUTPUT);

  int freq = 20;
  while(freq < 8000) {
    tone(BUZZER, freq, INTERVALO);
    delay(INTERVALO);
    freq+=50;
  }

  tone(BUZZER, 4000, 1000);
  delay(500);
  noTone(BUZZER);
}

void loop() {
}
