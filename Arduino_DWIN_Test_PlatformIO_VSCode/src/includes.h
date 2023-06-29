#include <Arduino.h>

const int WEBUPDATECODE = 9991;
const int OPENSWITCHSETTINGS = 9992;
const int RESETCODE= 9993;
const int WIFIRESETCODE= 9994;
const int FORCENIGHTCODE = 9995;
const int MAINCODE = 1664;
const int OTHERCODE1 = 7645;
const int OTHERCODE2 = 7412;



// alarm current state
#define ALARM_SET       1
#define ALARM_OFF       2
#define ALARM_ACTIVE    3
#define ALARM_ACTIVATED 4
#define ALARM_WONTSET   5
#define ALARM_PENDING   6
#define ALARM_NIGHT     7




 const char* ssid = "SSID";
 const char* password = "password";
 

 

/*
 IPAddress local_IP(192,168,0,189);
 IPAddress gateway(192, 168, 0, 1);
 IPAddress subnet(255, 255, 0, 0);
 IPAddress primaryDNS(8, 8, 8, 8); 
 IPAddress secondaryDNS(8, 8, 4, 4);
*/


