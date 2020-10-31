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

#define logDEBUG NULL // "DEBG"
#define logINFO "INFO"
#define logWARNING "WARN"
#define logERROR "EROR"

int logLevel = 0;

void log(const char* level, const char *format, ...) {
  if (level == NULL) {
    return;
  }

  char fullFormat[80];
  char buf[80];
  sprintf(fullFormat, "%s: %s", level, format);

  va_list args;
  va_start (args, format);
  vsprintf(buf, fullFormat, args);
  va_end (args);

  Serial.print(buf);
  if (logLevel == 0) {
    Serial.println("");
  }
  else {
    Serial.print(" ");
  }
}

void startLog(char* level, const char *format, ...) {
  if (level == NULL) {
    return;
  }

  char fullFormat[80];
  char buf[80];
  sprintf(fullFormat, "%s: %s ... ", level, format);

  va_list args;
  va_start (args, format);
  vsprintf(buf, fullFormat, args);
  va_end (args);

  Serial.print(buf);
  ++logLevel;
}

void endLog(char* level, const char *format, ...) {
  if (level == NULL) {
    return;
  }

  char buf[80];

  va_list args;
  va_start (args, format);
  vsprintf(buf, format, args);
  va_end (args);

  Serial.print(buf);
  --logLevel;
  if (logLevel == 0) {
    Serial.println("");
  }
  else {
    Serial.print(" ");
  }
}

void setAddress(int address, bool outputEnable) {
  startLog(logDEBUG, "Setting address=%3x, outputEnable=%i", address, outputEnable);
  int value = address | (outputEnable ? OUTPUT_ENABLED_VALUE : OUTPUT_DISABLED_VALUE);
  shiftOut(SHIFT_DATA_PIN, SHIFT_CLOCK_PIN, MSBFIRST, value >> 8);
  shiftOut(SHIFT_DATA_PIN, SHIFT_CLOCK_PIN, MSBFIRST, value & 0xff);

  digitalWrite(SHIFT_LATCH_PIN, LOW);
  digitalWrite(SHIFT_LATCH_PIN, HIGH);
  digitalWrite(SHIFT_LATCH_PIN, LOW);
  endLog(logDEBUG, "done");
}

byte readEEPROM(int address) {
  setAddress(address, OUTPUT_ENABLED);
  byte retval = 0;
  for (int pin = EEPROM_D0_PIN; pin <= EEPROM_D7_PIN; ++pin) {
    pinMode(pin, INPUT);
    int bit = digitalRead(pin);
    retval += (bit << (pin - EEPROM_D0_PIN));
    log(logDEBUG, "%03x, pin %2i = %i. retval so far = %03x", address,  pin, bit, retval);
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

  log(logINFO, "start");
  
  for (int i = 0; i < 256; i += 1) {
    startLog(logDEBUG, "writing %2i to %03x", i, i);
    writeEEPROM(i, i);
    delay(10);
    endLog(logDEBUG, "done");
  }

  printContents();

  log(logINFO, "end");

  Serial.println("");
  Serial.end();

}

void loop() {
  // put your main code here, to run repeatedly:

}
