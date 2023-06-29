#include "LCD_DWIN.h"

LCDSHOW dwinlcd;

typedef uint32_t millis_t;
millis_t next_LCD_update_ms = 0;
int recnum = 0;

LCDSHOW::LCDSHOW() {
  recdat.head[0] = snddat.head[0] = FHONE;
  recdat.head[1] = snddat.head[1] = FHTWO;
  memset(databuf, 0, sizeof(databuf));
}

int LCDSHOW::LCD_RecData() {
  while (Serial2.available() >= 1 && (recnum < SizeofDatabuf)) {

    databuf[recnum] = Serial2.read();
    // ignore the invalid data
    if (databuf[0] != FHONE) {

      // prevent the program from running.
      if (recnum > 0) {
        memset(databuf, 0, sizeof(databuf));
        recnum = 0;
      }
      continue;
    }
    delay(10);
    recnum++;
  }
  // receive nothing
  if (recnum < 1) {
    return -1;
  } else if ((recdat.head[0] == databuf[0]) && (recdat.head[1] == databuf[1]) &&
             recnum > 2) {
    recdat.len = databuf[2];
    recdat.command = databuf[3];
    if (recdat.len == 0x03 &&
        (recdat.command == 0x82 || recdat.command == 0x80) &&
        (databuf[4] == 0x4F) &&
        (databuf[5] == 0x4B)) // response for writing byte
    {
      memset(databuf, 0, sizeof(databuf));
      recnum = 0;
      return -1;
    } else if (recdat.command == 0x83) {
      // response for reading the data from the variate
      recdat.addr = databuf[4];
      recdat.addr = (recdat.addr << 8) | databuf[5];
      recdat.bytelen = databuf[6];

      if (recdat.bytelen == 1) {
        for (int i = 0; i < recdat.bytelen; i += 2) {
          recdat.data[i / 2] = databuf[7 + i];
          recdat.data[i / 2] = (recdat.data[i / 2] << 8) | databuf[8 + i];
        }
      } else {
        mydatabuf[0] = databuf[7];
        mydatabuf[1] = databuf[8]; // read for InputSwitchVar
        mydatabuf[2] = databuf[9];
        mydatabuf[3] = databuf[10]; // read for InputSwitchVar + 1 eg 0x1001
        recdat.data[0] = (databuf[7] << 16) + databuf[8];
        recdat.data[0] = recdat.data[0] + (databuf[9] << 8) + databuf[10];
        recdat.data[1] = databuf[10];

        if (recdat.bytelen ==
            8) { // get our adc level we only need the last for ADC7
          recdat.data[0] = (databuf[21] * 256) + databuf[22];
        }
      }
    } else if (recdat.command == 0x81) {
      // response for reading the page from the register
      recdat.addr = databuf[4];
      recdat.bytelen = databuf[5];
      for (unsigned int i = 0; i < recdat.bytelen; i++) {
        recdat.data[i] = databuf[6 + i];
        // recdat.data[i]= (recdat.data[i] << 8 )| databuf[7+i];
      }
    }
  } else {
    memset(databuf, 0, sizeof(databuf));
    recnum = 0;
    // receive the wrong data
    return -1;
  }
  memset(databuf, 0, sizeof(databuf));
  recnum = 0;
  return GOODSERIALDATA;
}

void LCDSHOW::LCD_SndData(void) {
  if ((snddat.head[0] == FHONE) && (snddat.head[1] == FHTWO) &&
      snddat.len >= 3) {
    databuf[0] = snddat.head[0];
    databuf[1] = snddat.head[1];
    databuf[2] = snddat.len;
    databuf[3] = snddat.command;

    // to write data to the register
    if (snddat.command == 0x80) {
      databuf[4] = snddat.addr;
      for (int i = 0; i < (snddat.len - 2); i++) {
        databuf[5 + i] = snddat.data[i];
      }
    } else if (snddat.len == 3 && (snddat.command == 0x81)) {
      // to read data from the register
      databuf[4] = snddat.addr;
      databuf[5] = snddat.bytelen;
    } else if (snddat.command == 0x82) {
      // to write data to the variate
      databuf[4] = snddat.addr >> 8;
      databuf[5] = snddat.addr & 0xFF;
      for (int i = 0; i < (snddat.len - 3); i += 2) {
        databuf[6 + i] = snddat.data[i / 2] >> 8;
        databuf[7 + i] = snddat.data[i / 2] & 0xFF;
      }
    } else if (snddat.len == 4 && (snddat.command == 0x83)) {
      // to read data from the variate
      databuf[4] = snddat.addr >> 8;
      databuf[5] = snddat.addr & 0xFF;
      databuf[6] = snddat.bytelen;
    }
    for (int i = 0; i < (snddat.len + 3); i++) {
      Serial2.write(databuf[i]);
      delayMicroseconds(1);
#ifdef serialDebug
      Serial.print(databuf[i], HEX);
      Serial.print(" ");
#endif
    }
    memset(&snddat, 0, sizeof(snddat));
    memset(databuf, 0, sizeof(databuf));
    snddat.head[0] = FHONE;
    snddat.head[1] = FHTWO;
  }
}

void LCDSHOW::LCD_SndData(const String &s, unsigned long addr,
                          unsigned char cmd /*= VarAddr_W*/) {
  if (s.length() < 1) {
    return;
  }
  LCD_SndData(s.c_str(), addr, cmd);
}

void LCDSHOW::LCD_SndData(const char *str, unsigned long addr,
                          unsigned char cmd /*= VarAddr_W*/) {
  int len = strlen(str);
  if (len > 0) {
    databuf[0] = FHONE;
    databuf[1] = FHTWO;
    databuf[2] = 3 + len;
    databuf[3] = cmd;
    databuf[4] = addr >> 8;
    databuf[5] = addr & 0x00FF;
    for (int i = 0; i < len; i++) {
      databuf[6 + i] = str[i];
    }

    for (int i = 0; i < (len + 6); i++) {
      Serial2.write(databuf[i]);
#ifdef serialDebug
      Serial.print(databuf[i], HEX);
      Serial.print(" ");
#endif
      delayMicroseconds(1);
    }
    memset(databuf, 0, sizeof(databuf));
  }
}

void LCDSHOW::LCD_SndData(char c, unsigned long addr,
                          unsigned char cmd /*= VarAddr_W*/) {
  snddat.command = cmd;
  snddat.addr = addr;
  snddat.data[0] = (unsigned long)c;
  snddat.data[0] = snddat.data[0] << 8;
  snddat.len = 5;
  LCD_SndData();
}

void LCDSHOW::LCD_SndData(unsigned char *str, unsigned long addr,
                          unsigned char cmd) {
  LCD_SndData((char *)str, addr, cmd);
}

void LCDSHOW::LCD_SndData(int n, unsigned long addr,
                          unsigned char cmd /*= VarAddr_W*/) {
  if (cmd == VarAddr_W) {
    if (n > 0xFFFF) {
      snddat.data[0] = n >> 16;
      snddat.data[1] = n & 0xFFFF;
      snddat.len = 7;
    } else {
      snddat.data[0] = n;
      snddat.len = 5;
    }
  } else if (cmd == RegAddr_W) {
    snddat.data[0] = n;
    snddat.len = 3;
  } else if (cmd == VarAddr_R) {
    snddat.bytelen = n;
    snddat.len = 4;
  }
  snddat.command = cmd;
  snddat.addr = addr;
  LCD_SndData();
}

void LCDSHOW::LCD_SndData(unsigned int n, unsigned long addr,
                          unsigned char cmd) {
  LCD_SndData((int)n, addr, cmd);
}

void LCDSHOW::LCD_SndData(float n, unsigned long addr, unsigned char cmd) {
  LCD_SndData((int)n, addr, cmd);
}

void LCDSHOW::LCD_SndData(long n, unsigned long addr, unsigned char cmd) {
  LCD_SndData((unsigned long)n, addr, cmd);
}

void LCDSHOW::LCD_SndData(unsigned long n, unsigned long addr,
                          unsigned char cmd /*= VarAddr_W*/) {

  if (cmd == VarAddr_W) {
    if (n > 0xFFFF) {
      snddat.data[0] = n >> 16;
      snddat.data[1] = n & 0xFFFF;
      snddat.len = 7;
    } else {
      snddat.data[0] = n;
      snddat.len = 5;
    }
  } else if (cmd == VarAddr_R) {
    snddat.bytelen = n;
    snddat.len = 4;
  }
  snddat.command = cmd;
  snddat.addr = addr;

  LCD_SndData();
}

void LCDSHOW::LCD_SndDataL(unsigned long iVal,
                           int iAdr) // Send iVal for VP= iAdr to DGUS
{
  byte bAdrL, bAdrH;
  bAdrL = iAdr & 0xFF;
  bAdrH = (iAdr >> 8) & 0xFF;
  byte *b = (byte *)&iVal;

  Serial2.write(0x5A);
  Serial2.write(0xA5);
  Serial2.write(0x07);
  Serial2.write(0x82);
  Serial2.write(bAdrH);
  Serial2.write(bAdrL);
  for (int i = 3; i >= 0; i--) {
    Serial2.write(b[i]);
    delayMicroseconds(1);
  }
}

void LCDSHOW::update_variable(void) {
  millis_t ms = millis();
  if (ms > next_LCD_update_ms) {
    next_LCD_update_ms = ms + DWIN_UPDATE_INTERVAL;
  }
}

void LCDSHOW::LCD_Init() {
  //For ESP
  //Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  // For Arduino Mega etc.
  Serial2.begin(115200,SERIAL_8N1);
  delay(1000);
  LCD_SndData(ChangePageBase,
              ChangepageAddr); // RESET Page back to "0" -> Boot screen
  LCD_SndData(PlayStartSound2, SoundAddress);
  // for(int j = 0;j < 20;j++)	{//clean filename
  // LCD_SndData(0,RSSIVar+j);
  // LCD_SndData(0,IPaddressVar+j);
  //}
  LCD_SndData(SystemSet, SystemSetAddress); // LCD setting etc.
  dwinlcd.LCD_SndData(LCDBackLightNormal, LCDBackLightAddress);
  
}

String LCDSHOW::bytesToDecString(byte byteHi, byte byteLow) {
  unsigned int word = (byteHi * 256) + byteLow;
  return String(word, DEC);
}

void LCDSHOW::clearTextVar(int varToClear) {
  for (int j = 0; j < 0x19; j++) { // clean filename
    dwinlcd.LCD_SndData(0, varToClear + j);
  }
}

void LCDSHOW::norReadWrite(bool write,int DGUSAddress,int NORAddress,int numWords) {
  Serial2.write(FHONE);
  Serial2.write(FHTWO);
  Serial2.write(0x0B);
  Serial2.write(0x82);
  Serial2.write(0x00);
  Serial2.write(0x08);    // nor address 08
  if (write) {
    Serial2.write(0xA5);  // Write nor
  } else {
    Serial2.write(0x5A);  // Read nor
  }
  Serial2.write(0x00);    //Hibyte 32bit address
  Serial2.write(NORAddress >> 8);
  Serial2.write(NORAddress & 0xFF);   // nor storage address 02000
  Serial2.write(DGUSAddress >> 8);
  Serial2.write(DGUSAddress & 0xFF);  // read/write from to 0x1000-0x1001 inputs on/off
  Serial2.write(numWords >> 8);       // No Words Hibyte
  Serial2.write(numWords & 0xFF);     // read/write 2 words
}