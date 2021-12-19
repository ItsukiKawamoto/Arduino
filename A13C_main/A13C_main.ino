#include "motorNew.h"
#include <Wire.h>
#include <VL53L0X.h>

VL53L0X laserSensor;

void setup() {
  DDRA |= _BV(0); //pinMode(22, OUTPUT); motor1
  DDRA |= _BV(2); //pinMode(24, OUTPUT);
  DDRA |= _BV(4); //pinMode(26, OUTPUT); motor2
  DDRA |= _BV(6); //pinMode(28, OUTPUT);
  DDRC |= _BV(7); //pinMode(30, OUTPUT); motor3
  DDRC |= _BV(5); //pinMode(32, OUTPUT);
  DDRC |= _BV(3); //pinMode(34, OUTPUT); motor4
  DDRC |= _BV(1); //pinMode(36, OUTPUT);
  DDRD |= _BV(3); //pinMode(18, OUTPUT); trig
  DDRD &= ~_BV(2); //pinMode(19, INPUT); echo
  DDRF &= ~_BV(0); //pinMode(A0, INPUT); PSD1
  DDRF &= ~_BV(1); //pinMode(A1, INPUT); PSD2
  pinMode(15, INPUT_PULLUP); //start

  Timer1.attachInterrupt(togglePulse1);
  Timer3.attachInterrupt(togglePulse2);
  Timer4.attachInterrupt(togglePulse3);
  Timer5.attachInterrupt(togglePulse4);

  //Serial.begin(9600);
  Wire.begin();

  laserSensor.setTimeout(500);
  laserSensor.init();
  laserSensor.startContinuous(100);
}

void loop() {
  while (true) {
    static boolean power = false;
    if (PINJ & _BV(0)) { //(digitalRead(15) == HIGH)
      power = false;
      //Serial.println("waiting");
    } else {
      if (!power) delay(800);
      power = true;
    }

    int x;
    int beforeX;
    static int y = 100;
    static boolean sonicFlag = false;

    if (power) {
      uint16_t lasorDistance = laserSensor.readRangeContinuousMillimeters();
      float sonicDistance = readSonic();
      int sonicTarget = 40;
      float sonicGain = 4;

      if (lasorDistance < 600) { //壁検知
        if (readPSD(0, 3) > 200) {
          //motorStop();
          //moveXY(0, -100);
          //delay(500);

          motorStop();
          motor(0, 0, 150, 150);
          delay(200);
          motor(-80, -80, 150, 150);
          delay(600);

          float gain = 0.01; //旋回速度調整
          while (true) {
            int dist = readPSD(1, 3);
            if (dist > 330) break;
            motor(-60, -60, 150, 150);
          }
          motor(60, 60, 60, 60);
          delay(100);

          //motorStop();
          //motor(40, 40, 150, 150);
          //delay(400);
          //motor(-60, -60, 150, 150);
          //delay(500);

          motorStop();
          while (readSonic() < sonicTarget) {
            float gain = 1.016;
            motor(150 * gain, -64, -150, 64 * gain);
          }
          y = 100;
          sonicFlag = false;

        }
      }

      if (abs(sonicTarget - sonicDistance) < 3) { //左右方向比例制御
        x = 0;
      } else {
        x = (sonicTarget - sonicDistance) * sonicGain;
      }
      if (sonicFlag) {
        x = constrain(x, beforeX - 20, beforeX + 20);
      } else {
        x = constrain(x, -100, 100);
      }
      beforeX = x;
      sonicFlag = true;

      moveXY(x, y);

      y++;
      y = constrain(y, 100, 300);

      delay(100);
    } else {
      moveXY(0, 0);
      y = 100;
      sonicFlag = false;
    }

    /* //TEST ************************************
      int low =  100;
      int high = 400;
      static float i = low;

      if (power) {
      static boolean accel = true;
      motor(-i, i, i, -i);
      delay(5);
      Serial.println(i);
      if (accel) {
        i += 1;
      } else {
        i -= 1;
      }
      if (i >= high) {
        accel = false;
      } else if (i <= -high) {
        accel = true;
      } else if (i < low && i > 0) {
        motor(0, 0, 0, 0);
        //while (!(PINJ & _BV(0)))
        delay(100);
        //accel = true;
        //i = low;
        i = -low;
      } else if (i > -low && i < 0) {
        motor(0, 0, 0, 0);
        delay(100);
        i = low;
      }
      } else {
      motor(0, 0, 0, 0);
      i = low; //停止時に出力を初期化
      }
    */ //************************************
  }
}

void moveXY(int x, int y) {
  float gain = 1.012; //回転補正用
  float PSDGain = 0;

  float r = max(abs(x), abs(y));;
  int m1, m2, n;
  //float dist1 = 8000 / analogRead(A0);
  //float dist2 =  8000 / analogRead(A1);
  //float diff = analogRead(A1) - analogRead(A0);
  //diff = diff * PSDGain;

  if (r == 0) {
    m1 = 0;
    m2 = 0;
    //diff = 0;
  } else {
    n = max(abs((650 / r) * (x + y)), abs((650 / r) * (x - y)));
    m1 = map(abs(x + y), 0, n, 0, 650);
    m2 = map(abs(x - y), 0, n, 0, 650);
    if (m1 < 40) m1 = 0;
    if (m2 < 40) m2 = 0;
    if ((x + y) < 0) m1 = -m1;
    if ((x - y) < 0) m2 = -m2;
  }
  motor(m1, m2 * gain, -m1 * gain, -m2);
  //motor(m1 + diff, m2 * gain, -m1 * gain, -(m2 + diff));
}

float readSonic() {
  PORTD &= ~_BV(3); //digitalWrite(18,LOW);
  PORTD &= ~_BV(2); //digitalWrite(19,LOW);
  delayMicroseconds(1);
  PORTD |= _BV(3); //digitalWrite(18,HIGH); 超音波パルス発射
  delayMicroseconds(10);
  PORTD &= ~_BV(3); //digitalWrite(18,LOW); 超音波パルス発射停止
  return pulseIn(19, HIGH) * 0.017; //echoピンがHIGHになっている時間*(㎲をsにする処理)*音速(34000cm/s)/2(道のりを片道分に)
}

int readPSD(uint8_t port, uint8_t count) {
  int distance = 0;
  switch (port) {
    case 0:
      for (uint8_t i = 0; i < count; i++) {
        distance += analogRead(A0);
        delay(30);
      }
      break;
    case 1:
      for (uint8_t i = 0; i < count; i++) {
        distance += analogRead(A1);
        delay(30);
      }
      break;
  }
  return distance / count;
}

void motorStop() {
  motor(0, 0, 0, 0);
  delay(50);
}
