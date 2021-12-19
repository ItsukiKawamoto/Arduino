#include "HUSKYLENS.h"

#define Pgain 50
#define Igain 0.1
#define Dgain 80
#define Speed 20

int Left = 0;
int Right = 0;
float FormerDiff = 0;
float Integral = 0;
unsigned long Time = millis();

HUSKYLENS huskylens;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  while (!huskylens.begin(Wire)) Serial.println(F("Begin failed!"));
  huskylens.writeAlgorithm(ALGORITHM_LINE_TRACKING);
}

void loop() {
  if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS!"));
  else if (!huskylens.isLearned()) Serial.println(F("Nothing has been learned!"));
  else if (!huskylens.available()) Serial.println(F("No arrow appears on the screen!"));
  else {
    while (huskylens.available()) {
      HUSKYLENSResult result = huskylens.read();
      float Diff = inclination(result);
      PID(Diff, &Left, &Right);
      Serial.println(String() + F("Inclination = ") + Diff + F(" Left = ") + Left + F(" Right = ") + Right);
    }
  }
}

float inclination(HUSKYLENSResult result) {
  if (result.yOrigin != result.yTarget) return float(result.xTarget - result.xOrigin) / float(result.yOrigin - result.yTarget);
}

void PID(float diff, int *left, int *right) {
  unsigned long T = millis();
  Integral += (diff + FormerDiff) / 2 * (T - Time);
  if (Integral > 100 / Igain)Integral = 100 / Igain;
  else if (Integral < -100 / Igain)Integral = -100 / Igain;
  float P = diff * Pgain;
  float I = Integral * Igain;
  float D = (diff - FormerDiff) / (T - Time) * Dgain;
  int Move = int(P + I + D);
  *left = Speed + Move;
  *right = Speed - Move;
  if (*left > 100)*left = 100;
  else if (*left < -100)*left = -100;
  if (*right > 100)*right = 100;
  else if (*right < -100)*right = -100;
  FormerDiff = diff;
  Time = T;
}
