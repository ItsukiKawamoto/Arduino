#include <avr/io.h>
#include <TimerOne.h>
#include <TimerThree.h>
#include <TimerFour.h>
#include <TimerFive.h>

void togglePulse1() { //引数のピンのHIGHとLOWを切り替える
  static boolean output = true;
  if (output) {
    PORTA |= _BV(PA0); //digitalWrite(22, HIGH);
    output = false;
  } else {
    PORTA &= ~_BV(PA0); //digitalWrite(22, LOW);
    output = true;
  }
}

void togglePulse2() {
  static boolean output = true;
  if (output) {
    PORTA |= _BV(PA4); //digitalWrite(26, HIGH);
    output = false;
  } else {
    PORTA &= ~_BV(PA4); //digitalWrite(26, LOW);
    output = true;
  }
}

void togglePulse3() {
  static boolean output = true;
  if (output) {
    PORTC |= _BV(PC7); //digitalWrite(30, HIGH);
    output = false;
  } else {
    PORTC &= ~_BV(PC7); //digitalWrite(30, LOW);
    output = true;
  }
}

void togglePulse4() {
  static boolean output = true;
  if (output) {
    PORTC |= _BV(PC3); //digitalWrite(34, HIGH);
    output = false;
  } else {
    PORTC &= ~_BV(PC3); //digitalWrite(34, LOW);
    output = true;
  }
}

void motor(float val1, float val2, float val3, float val4) {
  unsigned int microsec1, microsec2, microsec3, microsec4;
  //入力した出力値の絶対値からPWM制御のタイマー用の時間を算出(単位:㎲)
  if (val1 == 0) {
    PORTA &= ~_BV(PA0); //digitalWrite(22, LOW);
    Timer1.stop();
  } else {
    if (val1 > 0) {
      PORTA &= ~_BV(PA2); //digitalWrite(24, LOW);
    } else {
      PORTA |= _BV(PA2); //digitalWrite(24, HIGH);
    }
    microsec1 = 50000 / (abs(val1) ); //10000 / (abs(val1) / 5
    Timer1.initialize(microsec1);
  }
  if (val2 == 0) {
    PORTA &= ~_BV(PA4); //digitalWrite(26, LOW);
    Timer3.stop();
  } else {
    if (val2 > 0) {
      PORTA &= ~_BV(PA6); //digitalWrite(28, LOW);
    } else {
      PORTA |= _BV(PA6); //digitalWrite(28, HIGH);
    }
    microsec2 = 50000 / (abs(val2));
    Timer3.initialize(microsec2);
  }
  if (val3 == 0) {
    PORTC &= ~_BV(PC7); //digitalWrite(30, LOW);
    Timer4.stop();
  } else {
    if (val3 > 0) {
      PORTC &= ~_BV(PC5); //digitalWrite(32, LOW);
    } else {
      PORTC |= _BV(PC5); //digitalWrite(32, HIGH);
    }
    microsec3 = 50000 / (abs(val3));
    Timer4.initialize(microsec3);
  }
  if (val4 == 0) {
    PORTC &= ~_BV(PC3); //digitalWrite(34, LOW);
    Timer5.stop();
  } else {
    if (val4 > 0) {
      PORTC &= ~_BV(PC1 ); //digitalWrite(36, LOW);
    } else {
      PORTC |= _BV(PC1); //digitalWrite(36, HIGH);
    }
    microsec4 = 50000 / (abs(val4));
    Timer5.initialize(microsec4);
  }
}
