#ifndef	LCD_H
#define	LCD_H

#include "string.h"
#include <Arduino.h>

//#define serialDebug

/*********************************/
#define FHONE   (0x5A)
#define FHTWO   (0xA5)

#define SizeofDatabuf		26

#define	DWIN_UPDATE_INTERVAL 2000
#define GOODSERIALDATA 2
/*************Register and Variable addr*****************/
#define	RegAddr_W	0x80
#define	RegAddr_R	0x81
#define	VarAddr_W	0x82
#define	VarAddr_R	0x83
#define	ChangePageBase      ((unsigned long)0x5A010000)     // the first page ID. other page = first page ID + relevant num;
#define	AlarmOffPage        ((unsigned long)0x5A010001)     // Alarm Off Page
#define	AlarmActivatingPage ((unsigned long)0x5A010002)     // Alarm ActivatingPage
#define	AlarmActivePage     ((unsigned long)0x5A010003)     // Alarm Active
#define	AlarmActivated      ((unsigned long)0x5A010004)     // Alarm Activated
#define AlarmActiveNight    ((unsigned long)0x5A010005)     // Alarm Active night
#define AlarmWontSet        ((unsigned long)0x5A010006)     // Alarm Won't Set
#define AlarmInputsSet      ((unsigned long)0x5A010007)     // Page to turn alarm inputs on/off
#define WebUpdatePage       ((unsigned long)0x5A010008)     // Page Web Update
#define RestartingPage      ((unsigned long)0x5A010009)     // Page Restarting
#define WalkTestPage        ((unsigned long)0x5A01000B)     // Page Walktest
#define RelaysPage          ((unsigned long)0x5A01000C)     // Relay on/off settings

#define	StartSoundSet       ((unsigned long)0x060480A0)     // 06,start-music; 04, 4 musics; 80, the volume value; 04, return value about music number.
#define PlayStartSound6     ((unsigned long)0x06014000)
#define PlayErrorSound      ((unsigned long)0x05014000)
#define PlayStartSound2     ((unsigned long)0x02014000)
#define PlayBeep            ((unsigned long)0x03014000)
#define PlayDing            ((unsigned long)0x0B014000)
#define SystemSet           ((unsigned long)0x5A00007d)     //enable backlight timeouo
//#define LCDBackLightAddress 0x0082   //Low Brigtness 
#define LCDBackLightNormal  ((unsigned long)0x642A03E8)     //Normal Brigtness 
#define LCDBackLightLow     ((unsigned long)0x640803E8)     //Low Brigtness 0x640803E8
#define DWINReset           ((unsigned long)0x55AA5AA5)     // reset display

#define RXD2 16
#define TXD2 17

#define GreenSensorIcon 3
#define RedSensorIcon 4
#define DimRedSensorIcon 5 //not used

/*variable addr*/
#define	ChangepageAddr	0x0084
#define SoundAddress 0x00A0
#define SystemSetAddress 0x0080
#define LCDBackLightAddress 0x0082
#define DWINResetAddress 0x0004
#define ADCAddress 0x0032
#define ADCLength  0x08 // get all the adc's
     



#define InputSwitchVar 0x1000
#define InputSwitchVar2 0x1001
#define InputSwitchRetKeycode 0x1005
#define GreenRedSensorVar1  0x1006
#define GreenRedSensorVar2  0x1007
#define GreenRedSensorVar3  0x1008
#define GreenRedSensorVar4  0x1009
#define GreenRedSensorVar5  0x100A
#define GreenRedSensorVar6  0x100B
#define GreenRedSensorVar7  0x100C
#define GreenRedSensorVar8  0x100D
#define RSSIVar 0x1010  //21 dec long
#define IPaddressVar 0x1030  //21 dec long
#define WontSetTextVar 0x1050 //32 dec long
#define AlarmCodeVar 0x1070
#define RestartingVar   0x109A  //was 0x100A
#define NoWifiVar       0x109B   //was 0x100B
#define WalkReturnOff   0x109C //no used at the moment

#define ExternInputsVar 0x2001
#define RelayOutputVar  0x2002
#define RelaySwitchReturnKeyCode 0x2003

#define LEDColorAddress 0x4000
#define ADCLowHighAddress 0x4100  // sent from board when adc change

/************struct**************/
 typedef struct DataBuf {  
    unsigned char len;  
    unsigned char head[2];
    unsigned char command;
    unsigned long addr;
    unsigned long bytelen;
    unsigned short data[32];
    unsigned char reserv[4];
} DB;

class LCDSHOW {
  public:
    LCDSHOW();
    int LCD_RecData();
    void LCD_SndData(void);
    void LCD_SndDataL(long unsigned int , int);
    void LCD_SndData(const String &, unsigned long, unsigned char = VarAddr_W);
    void LCD_SndData(const char[], unsigned long, unsigned char = VarAddr_W);
    void LCD_SndData(char, unsigned long, unsigned char = VarAddr_W);
    void LCD_SndData(unsigned char*, unsigned long, unsigned char = VarAddr_W);
    void LCD_SndData(int, unsigned long, unsigned char = VarAddr_W);
    void LCD_SndData(float, unsigned long, unsigned char = VarAddr_W);
    void LCD_SndData(unsigned int,unsigned long, unsigned char = VarAddr_W);
    void LCD_SndData(long,unsigned long, unsigned char = VarAddr_W);
    void LCD_SndData(unsigned long,unsigned long, unsigned char = VarAddr_W);
    void update_variable(void);
    void LCD_Init();
    String bytesToDecString(byte byteHi,byte byteLow);
    void clearTextVar(int varToClear);
    void norReadWrite(bool write,int DGUSAddress,int NORAddress,int numWords);
    unsigned char mydatabuf[SizeofDatabuf];
   
    DB recdat;
    DB snddat;
  private:
    unsigned char databuf[SizeofDatabuf];
  };

extern LCDSHOW dwinlcd;

#endif// LCD_H
