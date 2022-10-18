#include "HUSKYLENS.h"

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
      Serial.print(F("Inclination = "));
      Serial.println(inclination(result));
    }
  }
}

String inclination(HUSKYLENSResult result) {
  if (result.yOrigin == result.yTarget) return "Horizontal";
  else return String(float((result.xTarget - result.xOrigin)) / float((result.yOrigin - result.yTarget)));
}
