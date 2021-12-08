#include <Ultrasonic.h>

Ultrasonic sensor(2, 3);

void setup() {
  Serial.begin(9600);
}

void loop() {
  int distCm = sensor.read(CM);
  int distIn = sensor.read(INC);

  Serial.println("Distância em cm: " + String(distCm));
  Serial.println("Distância em pol: " + String(distIn));
  delay(1000);
}
