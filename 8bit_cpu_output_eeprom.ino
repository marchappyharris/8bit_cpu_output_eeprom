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

#define ss_mid        (1 << 0)
#define ss_top_left   (1 << 1)
#define ss_low_left   (1 << 2)
#define ss_low        (1 << 3)
#define ss_low_right  (1 << 4)
#define ss_top_right  (1 << 5)
#define ss_top        (1 << 6)
#define ss_dot        (1 << 7)

#define ss_0  (   0    | ss_top_left | ss_top | ss_top_right | ss_low_left | ss_low | ss_low_right )
#define ss_1  (   0    |      0      |   0    | ss_top_right |      0      |    0   | ss_low_right )
#define ss_2  ( ss_mid |      0      | ss_top | ss_top_right | ss_low_left | ss_low |      0       )
#define ss_3  ( ss_mid |      0      | ss_top | ss_top_right |      0      | ss_low | ss_low_right )
#define ss_4  ( ss_mid | ss_top_left |   0    | ss_top_right |      0      |    0   | ss_low_right )
#define ss_5  ( ss_mid | ss_top_left | ss_top |      0       |      0      | ss_low | ss_low_right )
#define ss_6  ( ss_mid | ss_top_left | ss_top |      0       | ss_low_left | ss_low | ss_low_right )
#define ss_7  (   0    |      0      | ss_top | ss_top_right |      0      |    0   | ss_low_right )
#define ss_8  ( ss_mid | ss_top_left | ss_top | ss_top_right | ss_low_left | ss_low | ss_low_right )
#define ss_9  ( ss_mid | ss_top_left | ss_top | ss_top_right |      0      | ss_low | ss_low_right )
#define ss_A  ( ss_mid | ss_top_left | ss_top | ss_top_right | ss_low_left |    0   | ss_low_right )
#define ss_B  ( ss_mid | ss_top_left |   0    |      0       | ss_low_left | ss_low | ss_low_right )
#define ss_C  (   0    | ss_top_left | ss_top |      0       | ss_low_left | ss_low |      0       )
#define ss_D  ( ss_mid |      0      |   0    | ss_top_right | ss_low_left | ss_low | ss_low_right )
#define ss_E  ( ss_mid | ss_top_left | ss_top |      0       | ss_low_left | ss_low |      0       )
#define ss_F  ( ss_mid | ss_top_left | ss_top |      0       | ss_low_left |    0   |      0       )
#define ss_n  ( ss_mid |      0      |   0    |      0       |      0      |    0   |      0       )

#define ss_units    (0)
#define ss_tens     (1 << 8)
#define ss_hundreds (1 << 9)
#define ss_minus    ((1 << 8) | (1 << 9))
#define ss_unsigned (0)
#define ss_signed   (1 << 10)
#define ss_hex      (0)
#define ss_oct      (1 << 10)

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
  int total = 2048;
  int bytesPerBlock = 8;
  int blocksPerRow = 4;
  int bytesPerRow = bytesPerBlock * blocksPerRow;
  int rows = total / bytesPerRow;
  
  byte data[bytesPerRow];
  for (int line = 0; line < rows; line +=1) {
    int base = line * bytesPerRow;
    for (int offset = 0; offset < bytesPerRow; offset += 1) {
      data[offset] = readEEPROM(base + offset);
    }

    char buf[80];
    sprintf(buf, "%04x:", base);
    Serial.print(buf);
    for (int block = 0; block < blocksPerRow; ++block) {
      Serial.print("   ");
      for (int byt = 0; byt < bytesPerBlock; ++byt) {
        sprintf(buf, "%02x ", data[(block * bytesPerBlock) + byt]);            
        Serial.print(buf);
      }
    }
    Serial.println("");
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

  byte digits[] = {ss_0, ss_1, ss_2, ss_3, ss_4, ss_5, ss_6, ss_7, ss_8, ss_9, ss_A, ss_B, ss_C, ss_D, ss_E, ss_F};

  boolean decimal = false;

  if (decimal) {
    startLog(logINFO, "Writing unsigned decimal");
    for (int value = 0; value < 256; ++value) {
      writeEEPROM(value | ss_units    | ss_unsigned, digits[value % 10]);
      writeEEPROM(value | ss_tens     | ss_unsigned, digits[(value / 10) % 10]);
      writeEEPROM(value | ss_hundreds | ss_unsigned, digits[(value / 100) % 10]);
      writeEEPROM(value | ss_minus    | ss_unsigned, 0);
    }
    endLog(logINFO, "done");
    startLog(logINFO, "Writing signed decimal");
    for (int value = -128; value < 128; ++value) {
      writeEEPROM((value & 0xFF) | ss_units    | ss_signed, digits[abs(value) % 10]);
      writeEEPROM((value & 0xFF) | ss_tens     | ss_signed, digits[(abs(value) / 10) % 10]);
      writeEEPROM((value & 0xFF) | ss_hundreds | ss_signed, digits[(abs(value) / 100) % 10]);
      writeEEPROM((value & 0xFF) | ss_minus    | ss_signed, (value < 0) ? ss_n : 0);
    }
    endLog(logINFO, "done");
  }
  else {
    startLog(logINFO, "Writing unsigned hexadecimal");
    for (int value = 0; value < 256; ++value) {
      writeEEPROM(value | ss_units    | ss_hex, digits[abs(value) % 16]);
      writeEEPROM(value | ss_tens     | ss_hex, digits[(abs(value) / 16) % 16]);
      writeEEPROM(value | ss_hundreds | ss_hex, 0);
      writeEEPROM(value | ss_minus    | ss_hex, 0);
    }
    endLog(logINFO, "done");
    startLog(logINFO, "Writing unsigned octal");
    for (int value = 0; value < 256; ++value) {
      writeEEPROM(value | ss_units    | ss_oct, digits[abs(value) % 8]);
      writeEEPROM(value | ss_tens     | ss_oct, digits[(abs(value) / 8) % 8]);
      writeEEPROM(value | ss_hundreds | ss_oct, digits[(abs(value) / 64) % 8]);
      writeEEPROM(value | ss_minus    | ss_oct, 0);
    }
    endLog(logINFO, "done");
  }

  printContents();

  log(logINFO, "end");

  Serial.println("");
  Serial.end();

}

void loop() {
  // put your main code here, to run repeatedly:

}
