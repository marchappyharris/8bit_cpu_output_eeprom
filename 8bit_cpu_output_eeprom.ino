#define SHIFT_DATA_PIN 2
#define SHIFT_CLOCK_PIN 3
#define SHIFT_LATCH_PIN 4

#define EEPROM_D0_PIN 5
#define EEPROM_D7_PIN 12
#define WRITE_ENABLE_PIN 13

#define OUTPUT_ENABLE_PIN 15
#define OUTPUT_ENABLED_VALUE 0
#define OUTPUT_DISABLED_VALUE ( 2 << (OUTPUT_ENABLE_PIN - 1 ) )

#define OUTPUT_ENABLED true
#define OUTPUT_DISABLED false

void setAddress(int address, bool outputEnable) {
  int value = address | (outputEnable ? OUTPUT_ENABLED_VALUE : OUTPUT_DISABLED_VALUE);
  shiftOut(SHIFT_DATA_PIN, SHIFT_CLOCK_PIN, MSBFIRST, value >> 8);
  shiftOut(SHIFT_DATA_PIN, SHIFT_CLOCK_PIN, MSBFIRST, value & 0xff);

  digitalWrite(SHIFT_LATCH_PIN, LOW);
  digitalWrite(SHIFT_LATCH_PIN, HIGH);
  digitalWrite(SHIFT_LATCH_PIN, LOW);
}

byte readEEPROM(int address) {
  setAddress(address, OUTPUT_ENABLED);
  byte retval = 0;
  for (int pin = EEPROM_D0_PIN; pin >= EEPROM_D7_PIN; pin++) {
    pinMode(pin, INPUT);
    retval = (retval << 1) + digitalRead(pin);
  }
  return retval;
}

void writeEEPROM(int address, byte data) {
  setAddress(address, OUTPUT_DISABLED);
  for (int pin = EEPROM_D0_PIN; pin <= EEPROM_D7_PIN; ++pin) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, data & 1);
    data >>= 1;
  }

  digitalWrite(WRITE_ENABLE_PIN, LOW);
  delayMicroseconds(1);
  digitalWrite(WRITE_ENABLE_PIN, HIGH);
  delay(10);  
}

void printContents() {
  byte data[16];
  for (int line = 0; line < 16; line +=1) {
    int base = line * 16;
    for (int offset = 0; offset < 16; offset += 1) {
      data[offset] = readEEPROM(base + offset);
    }

    char buf[80];
    sprintf(buf, "%03x:  %02x %02x %02x %02x %02x %02x %02x %02x    %02x %02x %02x %02x %02x %02x %02x %02x",
            base,
            data[0],  data[1],  data[2],  data[3],  data[4],  data[5],  data[6],  data[7],
            data[8],  data[9],  data[10], data[11], data[12], data[13], data[14], data[15]);            
    Serial.println(buf);
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(SHIFT_DATA_PIN, OUTPUT);
  pinMode(SHIFT_CLOCK_PIN, OUTPUT);
  pinMode(SHIFT_LATCH_PIN, OUTPUT);
  digitalWrite(WRITE_ENABLE_PIN, HIGH);
  pinMode(WRITE_ENABLE_PIN, OUTPUT);

  Serial.begin(57600);
  Serial.println("");
  Serial.println("start");

  char buf[50];
  for (int i = 0; i < 2048; i += 1) {
    sprintf(buf, "%03x", i);
    Serial.println(buf);
    writeEEPROM(i, i);
    delay(10);
  }
  printContents();

  Serial.println("end");
  Serial.println("");
  Serial.end();

}

void loop() {
  // put your main code here, to run repeatedly:

}
