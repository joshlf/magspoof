void writeLow();
void setPolarity(int polarity);
void writeBit(int bit);
unsigned char *getEncoding(unsigned char character);
void writeEncodedCharacter(char character);
void writeSequence(int len, unsigned char sequence[]);

int leftPin = 10;
int rightPin = 11;
int clockSpeed = 500;
int polarity = 0;

unsigned char *buf;

void setup() {
  buf = malloc(81);
  // The interrupts are key to reliable
  // reading of the clock and data feed
  pinMode(leftPin, OUTPUT);
  pinMode(rightPin, OUTPUT);
  Serial.begin(9600);
  // Poor man's no timeout
  Serial.setTimeout(0xFFFFFFFFFFFFFFFF);
}

// Set the voltage to 0
void writeLow() {
  digitalWrite(leftPin, LOW);
  digitalWrite(rightPin, LOW);
}

// Set the polarity of the elctromagnet.
void setPolarity(int polarity) {
  // The polarity is set by
  // the appropriate pin high
  // to generate voltage and
  // setting the opposite pin
  // low so that it's not
  // generating counteracting
  // voltage.
  //
  // Make sure to set low before
  // high to avoid both pins
  // being high simultaneously.
  if (polarity == 1) {
    digitalWrite(rightPin, LOW);
    digitalWrite(leftPin, HIGH);
  } else {
    digitalWrite(leftPin, LOW);
    digitalWrite(rightPin, HIGH);
  }
}

// Write bit according to biphase mark coding
// (https://en.wikipedia.org/wiki/Biphase_mark_code)
void writeBit(int bit) {
  // Regardless of the bit, the signal 
  // always transitions on the positive 
  // edge of the clock transition.
  polarity = !polarity;
  setPolarity(polarity);
  delayMicroseconds(clockSpeed);

  // If the bit is a 1, the signal
  // transitions on the negative
  // edge of the clock transition.
  if (bit == 1) {
    polarity = !polarity;
    setPolarity(polarity);
  }

  delayMicroseconds(clockSpeed);
}

unsigned char encodings[][5] = {
  {0, 0, 0, 0, 1},  // 0
  {1, 0, 0, 0, 0},  // 1
  {0, 1, 0, 0, 0},  // 2
  {1, 1, 0, 0, 1},  // 3
  {0, 0, 1, 0, 0},  // 4
  {1, 0, 1, 0, 1},  // 5
  {0, 1, 1, 0, 1},  // 6
  {1, 1, 1, 0, 0},  // 7
  {0, 0, 0, 1, 0},  // 8
  {1, 0, 0, 1, 1},  // 9
  {0, 1, 0, 1, 1},  // : (control)
  {1, 1, 0, 1, 0},  // ; (start sentinel)
  {0, 0, 1, 1, 1},  // < (control)
  {1, 0, 1, 1, 0},  // = (field separator)
  {0, 1, 1, 1, 0},  // > (control)
  {1, 1, 1, 1 ,1}   // ? (field separator)
};

unsigned char* getEncoding(unsigned char character) {
  return encodings[character - '0'];
}

void writeCharacterEncoding(unsigned char *encoding) {
  writeBit(encoding[0]);
  writeBit(encoding[1]);
  writeBit(encoding[2]);
  writeBit(encoding[3]);
  writeBit(encoding[4]);
}

void writeSequence(int len, unsigned char sequence[]) {
  for (int i = 0; i < 50; i++) {
    writeBit(0);
  }

  unsigned char parity = 0;
  for (int i = 0; i < len; i++) {
    parity ^= sequence[i];
    unsigned char *encoding = getEncoding(sequence[i]);
    writeCharacterEncoding(encoding);
  }
  writeCharacterEncoding(encodings[parity]);

  for (int i = 0; i < 50; i++) {
    writeBit(0);
  }
}
 
void loop() {
  Serial.readBytes(buf, 81);
  writeSequence(buf[80], buf);
  writeLow();
  delay(500);
}

