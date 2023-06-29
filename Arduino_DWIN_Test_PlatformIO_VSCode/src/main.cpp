
#include <Arduino.h>
#include <includes.h>
//#include <WiFi.h>
#include <LCD_DWIN.h>

//void setup_wifi();
void processRecData();

void setup()
{
  
  Serial.begin(9600); //Serial port debug etc.
  delay(2000);
  Serial.println("Starting");

  dwinlcd.LCD_Init(); 
  //LCD_DWIN.cpp Serial2.begin(115200,SERIAL_8N1); For Arduino Serial2 Pins 16 17
  // plays a start sound
  // set the page to start on
  
  //setup_wifi();  // for ESP

  //Turn on our Green Icons
     for(byte i=0; i < 8 ; i++) { //loop thu the icons after 
    dwinlcd.LCD_SndData(GreenSensorIcon, GreenRedSensorVar1 + i);
    }
    delay(3000);

    // Turn to our page 2
    dwinlcd.LCD_SndData(AlarmOffPage,
              ChangepageAddr);
   }
  
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void loop()
{
 // check for new serial data
  if (dwinlcd.LCD_RecData() == GOODSERIALDATA)
    processRecData();  

}

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
void processRecData()
{

  Serial.println("dwinlcd.recdat.addr");
  Serial.println(dwinlcd.recdat.addr, HEX);
  Serial.println("recdat.0");
  Serial.println(dwinlcd.recdat.data[0], HEX);
  Serial.println("recdat.1");
  Serial.println(dwinlcd.recdat.data[1], HEX);



// Restart the DWIN display code 9993
  if ((dwinlcd.recdat.addr == AlarmCodeVar) &&
           (dwinlcd.recdat.data[0] == RESETCODE))
  {
    dwinlcd.LCD_SndData(RestartingPage, ChangepageAddr);
    delay(1000);
    dwinlcd.LCD_SndData(DWINReset, DWINResetAddress);
    delay(3000);
    
  }

// Switch to one of the settings pages code 9992
  if ((dwinlcd.recdat.addr == AlarmCodeVar) &&
      (dwinlcd.recdat.data[0] == OPENSWITCHSETTINGS) ) // switch to switch settings page
  {
    dwinlcd.LCD_SndData(AlarmInputsSet,
                        ChangepageAddr); // select inout setting page
  }
// Turn all the icons to red code 7645
  if ((dwinlcd.recdat.addr == AlarmCodeVar) &&
      (dwinlcd.recdat.data[0] == OTHERCODE1) ) 
  {
      for(byte i=0; i < 8 ; i++) { //loop thu the icons after 
       dwinlcd.LCD_SndData(RedSensorIcon, GreenRedSensorVar1 + i);
      }    
  }

//  Turn all the icons to Green code 7412  
  if ((dwinlcd.recdat.addr == AlarmCodeVar) &&
       (dwinlcd.recdat.data[0] == OTHERCODE2) )   
    for(byte i=0; i < 8 ; i++) { //loop thu the icons after 
       dwinlcd.LCD_SndData(GreenSensorIcon, GreenRedSensorVar1 + i);
    }

}

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
/*
// Connect to Wifi
void setup_wifi()
{
  // Turn off Access Point
  WiFi.mode(WIFI_STA);
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
  {
    Serial.println("STA Failed to configure");
  }
  delay(10);
// We start by connecting to a WiFi network
  Serial.println();
  Serial.println("Connecting..");
  WiFi.begin(ssid, password);
  byte wifi_count = 0;
  while ((WiFi.status() != WL_CONNECTED))
  {
    delay(500);
    wifi_count++;
    if (wifi_count > 30)
      break;
  }
  long rssi = WiFi.RSSI();
  Serial.println("rssi " + String(rssi));
  Serial.println("IP address");
  Serial.println(WiFi.localIP().toString().c_str());
  // sends info to our start page
  dwinlcd.LCD_SndData(String(rssi), RSSIVar);
  dwinlcd.LCD_SndData(WiFi.localIP().toString().c_str(), IPaddressVar);
} 

*/


  