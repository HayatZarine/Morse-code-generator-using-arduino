// Morse code generator on Arduino Uno. 
// LED on pin 13, buzzer on pin 8.
//
// Usage: edit message variable . 
// Non-letter characters produce a space.

const int ledPin = 13;
const int buzzerPin = 8; 

// timing (milliseconds)
const unsigned int dotTime = 120;      // unit time: dot length
const unsigned int dashTime = dotTime * 3;
const unsigned int intraSymbolGap = dotTime;     // between dots/dashes in same letter
const unsigned int interLetterGap = dotTime * 3; // between letters
const unsigned int wordGap = dotTime * 7;       // between words

// frequency for buzzer (Hz)
const unsigned int buzzFreq = 800;

const char* message = "Zarine"; 

// morse code lookup table 
struct MorseMap { char c; const char* code; };
const MorseMap morseTable[] = {
  {'A', ".-"}, {'B', "-..."}, {'C', "-.-."}, {'D', "-.."}, {'E', "."},
  {'F', "..-."}, {'G', "--."}, {'H', "...."}, {'I', ".."}, {'J', ".---"},
  {'K', "-.-"}, {'L', ".-.."}, {'M', "--"}, {'N', "-."}, {'O', "---"},
  {'P', ".--."}, {'Q', "--.-"}, {'R', ".-."}, {'S', "..."}, {'T', "-"},
  {'U', "..-"}, {'V', "...-"}, {'W', ".--"}, {'X', "-..-"}, {'Y', "-.--"},
  {'Z', "--.."},
  {'0', "-----"}, {'1', ".----"}, {'2', "..---"}, {'3', "...--"},
  {'4', "....-"}, {'5', "....."}, {'6', "-...."}, {'7', "--..."},
  {'8', "---.."}, {'9', "----."},
  {'.', ".-.-.-"},{',', "--..--"},{'?', "..--.."},{'/', "-..-."},
  {'@', ".--.-."},{'=', "-...-"}
};
const int morseTableSize = sizeof(morseTable) / sizeof(MorseMap);

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  noTone(buzzerPin);
  Serial.begin(9600);
  Serial.println("Morse ready. Send text over Serial or edit message in code.");
}

void loop() {
  // if serial data available, reAd a line and transmit it.
  if (Serial.available()) {
    String in = Serial.readStringUntil('\n');
    in.trim();
    if (in.length() > 0) {
      transmitText(in.c_str());
    }
  } else {
    // otherwise send default message, then pause..
    transmitText(message);
    delay(4000);
  }
}

void transmitText(const char* txt) {
  for (const char* p = txt; *p; ++p) {
    char ch = *p;
    if (ch >= 'a' && ch <= 'z') ch -= 'a' - 'A';
    if (ch == ' ' || ch == '\t') {
      delay(wordGap);
      continue;
    }
    const char* code = lookupMorse(ch);
    if (code) {
      transmitMorse(code);
      delay(interLetterGap - intraSymbolGap); // because last symbol already had intraSymbolGap
    } else {
      // unknown character -> treated as space
      delay(wordGap);
    }
  }
}

const char* lookupMorse(char c) {
  for (int i = 0; i < morseTableSize; ++i) {
    if (morseTable[i].c == c) return morseTable[i].code;
  }
  return nullptr;
}

void transmitMorse(const char* code) {
  for (const char* s = code; *s; ++s) {
    if (*s == '.') {
      flashDot();
    } else if (*s == '-') {
      flashDash();
    }
    // gap between symbols
    delay(intraSymbolGap);
  }
}

void flashDot() {
  digitalWrite(ledPin, HIGH);
  tone(buzzerPin, buzzFreq);
  delay(dotTime);
  digitalWrite(ledPin, LOW);
  noTone(buzzerPin);
}

void flashDash() {
  digitalWrite(ledPin, HIGH);
  tone(buzzerPin, buzzFreq);
  delay(dashTime);
  digitalWrite(ledPin, LOW);
  noTone(buzzerPin);
}
