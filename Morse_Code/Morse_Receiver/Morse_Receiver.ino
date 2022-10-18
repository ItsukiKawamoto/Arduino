const int pin = 0;
const int unitTime = 150;

void setup() {
  pinMode(pin, INPUT);
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(pin) == HIGH) Serial.println(reader());
}

String reader() {
  String code;
  String plaintext = "-> ";
  boolean reading = true;
  boolean flag = false;
  delay(unitTime / 2);
  while (reading) {
    delay(unitTime);
    if (digitalRead(pin) == HIGH) {
      if (flag) {
        code.concat("-");
      } else {
        code = "-";
        flag = true;
      }
      Serial.print("-");
      delay(unitTime * 3);
    } else {
      if (flag) {
        code.concat(".");
      } else {
        code = ".";
        flag = true;
      }
      Serial.print(".");
      delay(unitTime);
    }
    if (digitalRead(pin) == LOW) {
      flag = false;
      plaintext.concat(decoder(code));
      Serial.print(" ");
      delay(unitTime * 2);
      if (digitalRead(pin) == LOW) {
        plaintext.concat(" ");
        Serial.print("/ ");
        delay(unitTime * 4);
        if (digitalRead(pin) == LOW) reading = false;
      }
    }
  }
  Serial.println();
  return plaintext;
}

char decoder(String str) {
  if (str == ".-") return 'a';
  else if (str == "-...") return 'b';
  else if (str == "-.-.") return 'c';
  else if (str == "-..") return 'd';
  else if (str == ".") return 'e';
  else if (str == "..-.") return 'f';
  else if (str == "--.") return 'g';
  else if (str == "....") return 'h';
  else if (str == "..") return 'i';
  else if (str == ".---") return 'j';
  else if (str == "-.-") return 'k';
  else if (str == ".-..") return 'l';
  else if (str == "--") return 'm';
  else if (str == "-.") return 'n';
  else if (str == "---") return 'o';
  else if (str == ".--.") return 'p';
  else if (str == "--.-") return 'q';
  else if (str == ".-.") return 'r';
  else if (str == "...") return 's';
  else if (str == "-") return 't';
  else if (str == "..-") return 'u';
  else if (str == "...-") return 'v';
  else if (str == ".--") return 'w';
  else if (str == "-..-") return 'x';
  else if (str == "-.--") return 'y';
  else if (str == "--..") return 'z';
  else if (str == ".-.-.-") return '.';
  else if (str == "--..--") return ',';
  else if (str == ".----") return '1';
  else if (str == "..---") return '2';
  else if (str == "...--") return '3';
  else if (str == "....-") return '4';
  else if (str == ".....") return '5';
  else if (str == "-....") return '6';
  else if (str == "--...") return '7';
  else if (str == "---..") return '8';
  else if (str == "----.") return '9';
  else if (str == "-----") return '0';
  else return ' ';
}
