void writeLow();
void setPolarity(int polarity);
void writeBit(int bit);
void writeNumber(int num);
void writeSequence(int len, int sequence[]);

int leftPin = 10;
int rightPin = 11;
int clockSpeed = 500;
int polarity = 0;

void setup() {
  // The interrupts are key to reliable
  // reading of the clock and data feed
  pinMode(leftPin, OUTPUT);
  pinMode(rightPin, OUTPUT); 
  
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


int encodings[] = {
  0x01, // 0b00001
  0x10, // 0b10000
  0x08, // 0b01000
  0x19, // 0b11001
  0x04, // 0b00100
  0x15, // 0b10101
  0x0D, // 0b01101
  0x1C, // 0b11100
  0x02, // 0b00010
  0x13, // 0b10011
  0x0B, // 0b01011
  0x1A, // 0b11010
  0x07, // 0b00111
  0x16, // 0b10110
  0x0E, // 0b01110
  0x1F, // 0b11111
};

void writeNumber(int num) {
  writeBit(num & 0x10 >> 4);
  writeBit(num & 0x08 >> 3);
  writeBit(num & 0x04 >> 2);
  writeBit(num & 0x02 >> 1);
  writeBit(num & 0x01);
}

void writeSequence(int len, int sequence[]) {
  for (int i = 0; i < len; i++) {
    writeNumber(sequence[i]);
  }
}
 
void loop() {
  for(int i = 0; i < 20; i ++) {
    writeBit(0);
  }
  
  // Placeholder
  writeSequence(12, (int[]){11, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15});

  for(int i = 0; i < 20; i ++) {
    writeBit(0);
  }
  writeLow();
  delay(500);
}

