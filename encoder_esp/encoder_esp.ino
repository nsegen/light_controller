#include <Encoder.h>
 
Encoder myEnc(D1, D2);
 
long oldPosition  = -999;
boolean isButtonPressed = false;
long lastUpdateMillis = 0;
 
void handleKey() {
  isButtonPressed = true;  
}
 
void setup() {
  Serial.begin(115200);
  Serial.println("Basic Encoder Test:");
             //  "Базовый тест энкодера:"
   pinMode(D4, INPUT_PULLUP);
   attachInterrupt(D4, handleKey, RISING);
}
 
void loop() {
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    Serial.println(newPosition);
  }
  // программное устранение дребезга:
  if (isButtonPressed && millis() - lastUpdateMillis > 50) {
    isButtonPressed = false;
    lastUpdateMillis = millis();
    // сбрасываем счетчик:
    myEnc.write(0);
  }
}
