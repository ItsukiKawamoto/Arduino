const int pin = 1;
const int dotLen = 150; // length of the morse code 'dot'
const int dashLen = dotLen * 3; // length of the morse code 'dash'
const int elemPause = dotLen; // length of the pause between elements of a character
const int Spaces = dotLen * 3; // length of the spaces between characters
const int wordPause = dotLen * 7; // length of the pause between words

void setup() {
  Serial.begin(9600);
  pinMode(pin, OUTPUT);
}

void loop() {
  String str;
  char buf[1024];
  if (serialScanf(&str) != 0) {
    Serial.println(str);
    Serial.print("-> ");
    str.toCharArray(buf, (int)(str.length()));
    for (int i = 0; i < (int)(str.length()); i++) {
      //Serial.print(buf[i]);
      buf[i] = toLowerCase(buf[i]);
      encode(buf[i]);
    }
    Serial.println();
    // At the end of the string long pause before looping and starting again
    delay(2000);
  }
}

void tx(const char morseCode[]) {
  for (int i = 0; i < 7; i++) { // Get the character in the current position
    char tmpChar = morseCode[i];
    //Serial.print("tmpChar = ");Serial.print(sizeof(morseCode));
    Serial.print(tmpChar);
    switch (tmpChar) {
      case '.': dit(); break;
      case '-': dah(); break;
      case '/':
      case ' ': delay(Spaces - elemPause); return; // pause between characters
    }
  }
}

void dit() {
  digitalWrite(pin, HIGH);
  delay(dotLen);
  charPause(elemPause);
}

void dah() {
  digitalWrite(pin, HIGH);
  delay(dashLen);
  charPause(elemPause);
}

void charPause(int delayTime) {
  digitalWrite(pin, LOW);
  delay(delayTime);
}

void encode(char tmpChar) {
  // Take the passed character and use a switch case to find the morse code for that characters
  switch (tmpChar) {
    case 'a': tx(".- "); break;
    case 'b': tx("-... "); break;
    case 'c': tx("-.-. "); break;
    case 'd': tx("-.. "); break;
    case 'e': tx(". "); break;
    case 'f': tx("..-. "); break;
    case 'g': tx("--. "); break;
    case 'h': tx(".... "); break;
    case 'i': tx(".. "); break;
    case 'j': tx(".--- "); break;
    case 'k': tx("-.- "); break;
    case 'l': tx(".-.. "); break;
    case 'm': tx("-- "); break;
    case 'n': tx("-. "); break;
    case 'o': tx("--- "); break;
    case 'p': tx(".--. "); break;
    case 'q': tx("--.- "); break;
    case 'r': tx(".-. "); break;
    case 's': tx("... "); break;
    case 't': tx("- "); break;
    case 'u': tx("..- "); break;
    case 'v': tx("...- "); break;
    case 'w': tx(".-- "); break;
    case 'x': tx("-..- "); break;
    case 'y': tx("-.-- "); break;
    case 'z': tx("--.. "); break;
    case '.': tx(".-.-.- "); break;
    case ',': tx("--..-- "); break;
    case '1': tx(".---- "); break;
    case '2': tx("..--- "); break;
    case '3': tx("...-- "); break;
    case '4': tx("....- "); break;
    case '5': tx("..... "); break;
    case '6': tx("-.... "); break;
    case '7': tx("--... "); break;
    case '8': tx("---.. "); break;
    case '9': tx("----. "); break;
    case '0': tx("----- "); break;
    default:
      // If a matching character was not found it will default to space between words
      Serial.print("/ ");
      delay(wordPause - Spaces);
  }
}

int serialScanf(String *str) {
  int len = Serial.available();
  if (len == 0) return 0;
  *str = Serial.readString();
  return len;
}
