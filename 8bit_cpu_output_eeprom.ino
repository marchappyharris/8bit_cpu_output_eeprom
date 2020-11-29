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

#define EEPROM_control0   (0)
#define EEPROM_control1   (1)
#define EEPROM_control2   (2)
#define EEPROM_decimal    (3)
#define EEPROM_hex_octal  (4)

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

#define c_halt     (1L << 23)
#define c_addr_in  (1L << 22)
#define c_mem_in   (1L << 21)
#define c_mem_out  (1L << 20)
#define c_inst_out (1L << 19)
#define c_inst_in  (1L << 18)
#define c_a_in     (1L << 17)
#define c_a_out    (1L << 16)
#define c_sum_out  (1L << 15)
#define c_subtr    (1L << 14)
#define c_b_in     (1L << 13)
#define c_disp_in  (1L << 12)
#define c_cnt_en   (1L << 11)
#define c_cnt_out  (1L << 10)
#define c_jump     (1L <<  9)
#define c_flag_in  (1L <<  8)
#define c_spare_7  (1L <<  7)
#define c_spare_6  (1L <<  6)
#define c_spare_5  (1L <<  5)
#define c_spare_4  (1L <<  4)
#define c_spare_3  (1L <<  3)
#define c_spare_2  (1L <<  2)
#define c_spare_1  (1L <<  1)
#define c_spare_0  (1L <<  0)

#define instr_CMP   0 // load memory into B, subtract, latch registers
#define instr_CMI   1 // load immediate into B, subtract, latch registers
#define instr_LDA   2 // load memory into A
#define instr_LDI   3 // load immediate into A
#define instr_ADD   4 // load memory into B, add, latch registers, store result in A
#define instr_ADI   5 // load immediate into B, add, latch registers, store result in A
#define instr_SUB   6 // load memory into B, subtract, latch registers, store result in A
#define instr_STA   7 // store A into memory
#define instr_JMP   8 // jump unconditional
#define instr_JZ    9 // jump if zeroFlag == 1
#define instr_JNZ  10 // jump if zeroFlag == 0
#define instr_JC   11 // jump if carryFlag == 1
#define instr_JLT  12 // jump if zeroFlag == 0 && set carryFlag == 0
#define instr_JGE  13 // jump if zeroFlag == 1 || set carryFlag == 1
#define instr_OUT  14 // copy A into output
#define instr_HLT  15 // halt

#define f_Z      (1L << 0) // zero
#define f_C      (1L << 1) // carry

#define fm_UNSET  (0)       // ignore flags
#define fm_ALLSET (1)       // ignore flags
#define fm_ANYSET (2)       // ignore flags

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

void printContents(int bytesPerBlock, int blocksPerRow, int rowsPerChunk, bool binary, bool skipZeros) {
  int total = 2048;
  int bytesPerRow = bytesPerBlock * blocksPerRow;
  int rows = total / bytesPerRow;
  int chunks = rows / rowsPerChunk;
  
  byte data[bytesPerRow];
  bool skippedLast = false;
  for (int chunk = 0; chunk < chunks; chunk +=1) {
    bool nonZeroChunk = !skipZeros;  // if not skipping zeros, just pretend we have found a non-zero
    for (int row = 0; row < rowsPerChunk; row +=1) {
      bool nonZeroRow = !skipZeros;  // if not skipping zeros, just pretend we have found a non-zero
      int base = (chunk * rowsPerChunk + row) * bytesPerRow;
      for (int offset = 0; offset < bytesPerRow; offset += 1) {
        byte dataByte= readEEPROM(base + offset);
        data[offset] = dataByte;
        nonZeroRow |= (dataByte != 0);
        nonZeroChunk |= (dataByte != 0);
      }

      if (!nonZeroRow) {
        if (!skippedLast) {
          Serial.println("  ...");
          skippedLast = true;
        }
        continue;
      }
      else {
        skippedLast = false;
      }

      char buf[80];
      sprintf(buf, "%04x:", base);
      Serial.print(buf);
      for (int block = 0; block < blocksPerRow; ++block) {
        Serial.print("   ");
        for (int byt = 0; byt < bytesPerBlock; ++byt) {
          byte value = data[(block * bytesPerBlock) + byt];
          if (binary) {
            sprintf(buf, "%i%i%i%i%i%i%i%i ", (value & 0x80) / 0x80, (value & 0x40) / 0x40, (value & 0x20) / 0x20, (value & 0x10) / 0x10,
                                              (value & 0x08) / 0x08, (value & 0x04) / 0x04, (value & 0x02) / 0x02, (value & 0x01) / 0x01);
          } else {
            sprintf(buf, "%02x ", value);
          }
          Serial.print(buf);
        }
      }
      if (nonZeroRow) {
        Serial.println("");
      }
    }
    if (nonZeroChunk) {
      Serial.println("");
    }
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

  int EEPROM_type = EEPROM_control1;
  switch (EEPROM_type) {
    case  EEPROM_control0:
      writeControlLogic(0);
      break;
    case  EEPROM_control1:
      writeControlLogic(1);
      break;
    case  EEPROM_control2:
      writeControlLogic(2);
      break;
    case  EEPROM_decimal:
      writeSevenSegmentDecoder(true);
      break;
    case  EEPROM_hex_octal:
      writeSevenSegmentDecoder(false);
      break;
    default:
      break;
  }

  log(logINFO, "end");
  Serial.println("");
  Serial.end();
}

void writeSevenSegmentDecoder(bool decimal) {
  byte digits[] = {ss_0, ss_1, ss_2, ss_3, ss_4, ss_5, ss_6, ss_7, ss_8, ss_9, ss_A, ss_B, ss_C, ss_D, ss_E, ss_F};

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

  printContents(8, 4, 32, false, false);
}

void writeControlLogic(int EEPROM_num) {
  
  startLog(logINFO, "Clearing chip");
  for (int address = 0; address < 2048; ++address) {
    writeEEPROM(address, 0);
  }
  endLog(logINFO, "done");

  log(logINFO, "Writing control logic chip number %i",  EEPROM_num);
  writeInstr(EEPROM_num, instr_CMP , 0         , 0         , c_inst_out | c_addr_in  , c_mem_out  | c_b_in            , c_flag_in | c_subtr                      , 0, 0, 0 );
  writeInstr(EEPROM_num, instr_CMI , 0         , 0         , c_inst_out | c_b_in     , c_flag_in  | c_subtr           , 0                                        , 0, 0, 0 );
  writeInstr(EEPROM_num, instr_LDA , 0         , 0         , c_inst_out | c_addr_in  , c_mem_out  | c_a_in            , 0                                        , 0, 0, 0 );
  writeInstr(EEPROM_num, instr_LDI , 0         , 0         , c_inst_out | c_a_in     , 0                              , 0                                        , 0, 0, 0 );
  writeInstr(EEPROM_num, instr_ADD , 0         , 0         , c_inst_out | c_addr_in  , c_mem_out  | c_b_in            , c_sum_out | c_a_in | c_flag_in           , 0, 0, 0 );
  writeInstr(EEPROM_num, instr_ADI , 0         , 0         , c_inst_out | c_b_in     , c_sum_out | c_a_in | c_flag_in , 0                                        , 0, 0, 0 );
  writeInstr(EEPROM_num, instr_SUB , 0         , 0         , c_inst_out | c_addr_in  , c_mem_out  | c_b_in            , c_sum_out | c_a_in | c_flag_in | c_subtr , 0, 0, 0 );
  writeInstr(EEPROM_num, instr_STA , 0         , 0         , c_inst_out | c_addr_in  , c_a_out    | c_mem_in          , 0                                        , 0, 0, 0 );
  writeInstr(EEPROM_num, instr_JMP , 0         , 0         , c_inst_out | c_jump     , 0                              , 0                                        , 0, 0, 0 );
  writeInstr(EEPROM_num, instr_JZ  , fm_ALLSET , f_Z       , c_inst_out | c_jump     , 0                              , 0                                        , 0, 0, 0 );
  writeInstr(EEPROM_num, instr_JNZ , fm_UNSET  , f_Z       , c_inst_out | c_jump     , 0                              , 0                                        , 0, 0, 0 );
  writeInstr(EEPROM_num, instr_JC  , fm_ALLSET , f_C       , c_inst_out | c_jump     , 0                              , 0                                        , 0, 0, 0 );
  writeInstr(EEPROM_num, instr_JLT , fm_UNSET  , f_Z | f_C , c_inst_out | c_jump     , 0                              , 0                                        , 0, 0, 0 );
  writeInstr(EEPROM_num, instr_JGE , fm_ANYSET , f_Z | f_C , c_inst_out | c_jump     , 0                              , 0                                        , 0, 0, 0 );
  writeInstr(EEPROM_num, instr_OUT , fm_ANYSET , 0         , c_a_out    | c_disp_in  , 0                              , 0                                        , 0, 0, 0 );
  writeInstr(EEPROM_num, instr_HLT , fm_ANYSET , 0         , c_halt                  , 0                              , 0                                        , 0, 0, 0 );
  log(logINFO, "Done writing control logic chip number %i",  EEPROM_num);

  log(logINFO, "Reading contents of EEPROM ...");
  printContents(8, 1, 8, true, false);
  log(logINFO, "done");
}

void writeInstr(int EEPROM_num, int instruction, int flags_set, int flag_mask, long op2, long op3, long op4, long op5, long op6, long op7) {
  
  int stepBits = 3;
  int stepCount = 1 << stepBits;
  int flagBits = 2;
  int flagCount = 1 << flagBits;
  int instructionBits = 4;
  int instructionCount = 1 << instructionBits;
  int fetchInstructions = 2;
  unsigned long fetch[fetchInstructions] = { c_addr_in | c_cnt_out,
                                             c_mem_out | c_inst_in | c_cnt_en };

  unsigned long steps[stepCount] = { fetch[0], fetch[1], op2, op3, op4, op5, op6, op7 };

  for (int codeStep = 0; codeStep < stepCount; ++codeStep) {
    int instrAndStep = (instruction << stepBits) | codeStep;
    unsigned long val = steps[codeStep];
    byte bVal = (byte)((val >> (EEPROM_num * 8)) & 0xff);
    for (long flag = 0; flag < flagCount; ++flag) {
      int address = instrAndStep | flag << (instructionBits + stepBits);
      long flagMatch = flag && flag_mask;
      bool enabled;
      if (codeStep < fetchInstructions) {
        enabled = true;
      }
      else if (flags_set == fm_UNSET) {
        enabled = ( flagMatch == 0);
      }
      else if (flags_set == fm_ANYSET) {
        enabled = ( flagMatch != 0);
      }
      else { // flags_set == fm_ALLSET
        enabled = ( flagMatch == flag_mask);
      }
      log(logINFO, "instrAndStep=%04x, address=%04x, enabled=%i, bVal=%i", instrAndStep, address, enabled, bVal);
      writeEEPROM(address, enabled ? bVal : 0);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
