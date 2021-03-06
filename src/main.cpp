/****************************************************************************************************************************
  ESP32_FSWebServer_DRD - Example WebServer with SPIFFS backend for esp32 with DoubleResetDetect feature
  For ESP32 boards
  
  ESP_WiFiManager is a library for the ESP8266/ESP32 platform (https://github.com/esp8266/Arduino) to enable easy
  configuration and reconfiguration of WiFi credentials using a Captive Portal.
  
  Modified from Tzapu https://github.com/tzapu/WiFiManager
  and from Ken Taylor https://github.com/kentaylor
  
  Built by Khoi Hoang https://github.com/khoih-prog/ESP_WiFiManager
  Licensed under MIT license
  
  Example modified from https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WebServer/examples/FSBrowser/FSBrowser.ino
  
  Copyright (c) 2015 Hristo Gochkov. All rights reserved.
  This file is part of the ESP8266WebServer library for Arduino environment.
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  
  Version: 1.6.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      07/10/2019 Initial coding
  1.0.1   K Hoang      13/12/2019 Fix bug. Add features. Add support for ESP32
  1.0.2   K Hoang      19/12/2019 Fix bug thatkeeps ConfigPortal in endless loop if Portal/Router SSID or Password is NULL.
  1.0.3   K Hoang      05/01/2020 Option not displaying AvailablePages in Info page. Enhance README.md. Modify examples
  1.0.4   K Hoang      07/01/2020 Add RFC952 setHostname feature.
  1.0.5   K Hoang      15/01/2020 Add configurable DNS feature. Thanks to @Amorphous of https://community.blynk.cc
  1.0.6   K Hoang      03/02/2020 Add support for ArduinoJson version 6.0.0+ ( tested with v6.14.1 )
  1.0.7   K Hoang      13/04/2020 Reduce start time, fix SPIFFS bug in examples, update README.md
  1.0.8   K Hoang      10/06/2020 Fix STAstaticIP issue. Restructure code. Add LittleFS support for ESP8266 core 2.7.1+
  1.0.9   K Hoang      29/07/2020 Fix ESP32 STAstaticIP bug. Permit changing from DHCP <-> static IP using Config Portal.
                                  Add, enhance examples (fix MDNS for ESP32)
  1.0.10  K Hoang      08/08/2020 Add more features to Config Portal. Use random WiFi AP channel to avoid conflict.
  1.0.11  K Hoang      17/08/2020 Add CORS feature. Fix bug in softAP, autoConnect, resetSettings.
  1.1.0   K Hoang      28/08/2020 Add MultiWiFi feature to autoconnect to best WiFi at runtime
  1.1.1   K Hoang      30/08/2020 Add setCORSHeader function to allow flexible CORS. Fix typo and minor improvement.
  1.1.2   K Hoang      17/08/2020 Fix bug. Add example.
  1.2.0   K Hoang      09/10/2020 Restore cpp code besides Impl.h code to use if linker error. Fix bug.
  1.3.0   K Hoang      04/12/2020 Add LittleFS support to ESP32 using LITTLEFS Library
  1.4.1   K Hoang      22/12/2020 Fix staticIP not saved. Add functions. Add complex examples. Sync with ESPAsync_WiFiManager
  1.4.2   K Hoang      14/01/2021 Fix examples' bug not using saved WiFi Credentials after losing all WiFi connections.
  1.4.3   K Hoang      23/01/2021 Fix examples' bug not saving Static IP in certain cases.
  1.5.0   K Hoang      12/02/2021 Add support to new ESP32-S2
  1.5.1   K Hoang      26/03/2021 Fix compiler error if setting Compiler Warnings to All. Retest with esp32 core v1.0.6
  1.5.2   K Hoang      08/04/2021 Fix example misleading messages.
  1.5.3   K Hoang      13/04/2021 Add dnsServer error message.
  1.6.0   K Hoang      20/04/2021 Add support to new ESP32-C3 using SPIFFS or EEPROM
 *****************************************************************************************************************************/
/*****************************************************************************************************************************
   How To Use:
   1) Upload the contents of the data folder with MkSPIFFS Tool ("ESP32 Sketch Data Upload" in Tools menu in Arduino IDE)
   2) or you can upload the contents of a folder if you CD in that folder and run the following command:
      for file in `\ls -A1`; do curl -F "file=@$PWD/$file" esp32-fs-browser.local/edit; done
   3) access the sample web page at http://esp32-fs-browser.local
   4) edit the page by going to http://esp32-fs-browser.local/edit
*****************************************************************************************************************************/


#include <LXESP32DMX.h>
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`

#define DMX_DIRECTION_PIN 12
#define DMX_SERIAL_OUTPUT_PIN 14
#define DMX_SEQUENCE_UNIVERSE_SELECTOR 27

#define DISPLAY_SDA_PIN 4
#define DISPLAY_SCL_PIN 15
#define DISPLAY_RESET_PIN 16

#define SCREEN_HEIGHT 64
#define SCREEN_WIDTH 128

// Initialize the OLED display using Wire library
SSD1306  display(0x3c, 4, 15);
char lcdBuffer[20]="";
//----------------------------------------------------------------------------------------------------------------------
#include <ArtnetWifi.h>

ArtnetWifi artnet;

#define DMX_DIRECTION_PIN 12
#define DMX_SERIAL_OUTPUT_PIN 14
#define DMX_SEQUENCE_UNIVERSE_SELECTOR 27

#define  DMX_CHANNELS 512
#define  CONFIG_VALUES 3
#define  EXTRA_PARAMETERS 1
#define  DMX_CHANNELS_HTML_DEFAULT 14
#define  DMX_CHANNEL_START_HTML_DEFAULT 0
#define  DMX_MAX_UNIVERSE 512

int numChannels;
int startAddr=0;
int ch[10];
int universe;

uint8_t level;
uint8_t dmxbuffer[DMX_MAX_FRAME];
uint16_t universeSelect = 0;
//----------------------------------------------------------------------------------------------------------------------
#include "esp_task_wdt.h"

//----------------------------------------------------------------------------------------------------------------------

#if !defined(ESP32)
  #error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.
#endif

#define ESP_WIFIMANAGER_VERSION_MIN_TARGET     "ESP_WiFiManager v1.6.0"

// Use from 0 to 4. Higher number, more debugging messages and memory usage.
#define _WIFIMGR_LOGLEVEL_    3

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

// From v1.1.0
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
//////

// LittleFS has higher priority than SPIFFS
#if ( ARDUINO_ESP32C3_DEV )
  // Currently, ESP32-C3 only supporting SPIFFS and EEPROM. Will fix to support LittleFS
  #define USE_LITTLEFS          false
  #define USE_SPIFFS            true
#else
  #define USE_LITTLEFS    true
  #define USE_SPIFFS      false
#endif

// These defines must be put before #include <ESP_DoubleResetDetector.h>
// to select where to store DoubleResetDetector's variable.
// For ESP32, You must select one to be true (EEPROM or SPIFFS)
// Otherwise, library will use default EEPROM storage

#if USE_LITTLEFS
  #define ESP_DRD_USE_LITTLEFS    true
  #define ESP_DRD_USE_SPIFFS      false
  #define ESP_DRD_USE_EEPROM      false
#elif USE_SPIFFS
  #define ESP_DRD_USE_LITTLEFS    false
  #define ESP_DRD_USE_SPIFFS      true
  #define ESP_DRD_USE_EEPROM      false
#else
  #define ESP_DRD_USE_LITTLEFS    false
  #define ESP_DRD_USE_SPIFFS      false
  #define ESP_DRD_USE_EEPROM      true
#endif

#define DOUBLERESETDETECTOR_DEBUG       true  //false

#include <ESP_DoubleResetDetector.h>      //https://github.com/khoih-prog/ESP_DoubleResetDetector

// Number of seconds after reset during which a
// subseqent reset will be considered a double reset.
#define DRD_TIMEOUT 10

// RTC Memory Address for the DoubleResetDetector to use
#define DRD_ADDRESS 0

//DoubleResetDetector drd(DRD_TIMEOUT, DRD_ADDRESS);
DoubleResetDetector* drd;
//////

// You only need to format the filesystem once
//#define FORMAT_FILESYSTEM       true
#define FORMAT_FILESYSTEM         false

#if USE_LITTLEFS
  // Use LittleFS
  #include "FS.h"

  // The library has been merged into esp32 core release 1.0.6
  #include <LITTLEFS.h>             // https://github.com/lorol/LITTLEFS
  
  FS* filesystem =      &LITTLEFS;
  #define FileFS        LITTLEFS
  #define FS_Name       "LittleFS"
#elif USE_SPIFFS
  #include <SPIFFS.h>
  FS* filesystem =      &SPIFFS;
  #define FileFS        SPIFFS
  #define FS_Name       "SPIFFS"
#else
  // Use FFat
  #include <FFat.h>
  FS* filesystem =      &FFat;
  #define FileFS        FFat
  #define FS_Name       "FFat"
#endif

#define DBG_OUTPUT_PORT Serial

// SSID and PW for Config Portal
String ssid = "ESP_" + String((uint32_t)ESP.getEfuseMac(), HEX);
const char* password = "your_password";

// SSID and PW for your Router
String Router_SSID;
String Router_Pass;

// From v1.1.0
#define MIN_AP_PASSWORD_SIZE    8

#define SSID_MAX_LEN            32
//From v1.0.10, WPA2 passwords can be up to 63 characters long.
#define PASS_MAX_LEN            64

typedef struct
{
  char wifi_ssid[SSID_MAX_LEN];
  char wifi_pw  [PASS_MAX_LEN];
}  WiFi_Credentials;

typedef struct
{
  String wifi_ssid;
  String wifi_pw;
}  WiFi_Credentials_String;

#define NUM_WIFI_CREDENTIALS      2

typedef struct
{
  WiFi_Credentials  WiFi_Creds [NUM_WIFI_CREDENTIALS];
} WM_Config;

WM_Config         WM_config;

#define  CONFIG_FILENAME              F("/wifi_cred.dat")

typedef struct
{
  int  ch[10];
  int  chNum;
  int  universe;
  int  startChannel;
} DMX_Config;

DMX_Config         DMX_config;

bool dmxConfigFileOk = false;
bool webPageInitalConnect = false;

#define  CONFIG_DMX_FILENAME              F("/dmx_config.dat")
//////

// Indicates whether ESP has WiFi credentials saved from previous session, or double reset detected
bool initialConfig = false;

// Use false if you don't like to display Available Pages in Information Page of Config Portal
// Comment out or use true to display Available Pages in Information Page of Config Portal
// Must be placed before #include <ESP_WiFiManager.h>
#define USE_AVAILABLE_PAGES     false

// From v1.0.10 to permit disable/enable StaticIP configuration in Config Portal from sketch. Valid only if DHCP is used.
// You'll loose the feature of dynamically changing from DHCP to static IP, or vice versa
// You have to explicitly specify false to disable the feature.
//#define USE_STATIC_IP_CONFIG_IN_CP          false

// Use false to disable NTP config. Advisable when using Cellphone, Tablet to access Config Portal.
// See Issue 23: On Android phone ConfigPortal is unresponsive (https://github.com/khoih-prog/ESP_WiFiManager/issues/23)
#define USE_ESP_WIFIMANAGER_NTP     false

// Use true to enable CloudFlare NTP service. System can hang if you don't have Internet access while accessing CloudFlare
// See Issue #21: CloudFlare link in the default portal (https://github.com/khoih-prog/ESP_WiFiManager/issues/21)
#define USE_CLOUDFLARE_NTP          false

// New in v1.0.11
#define USING_CORS_FEATURE          true
//////

// Use USE_DHCP_IP == true for dynamic DHCP IP, false to use static IP which you have to change accordingly to your network
#if (defined(USE_STATIC_IP_CONFIG_IN_CP) && !USE_STATIC_IP_CONFIG_IN_CP)
  // Force DHCP to be true
  #if defined(USE_DHCP_IP)
    #undef USE_DHCP_IP
  #endif
  #define USE_DHCP_IP     true
#else
  // You can select DHCP or Static IP here
  #define USE_DHCP_IP     true
  //#define USE_DHCP_IP     false
#endif

#if ( USE_DHCP_IP )
  // Use DHCP
  #warning Using DHCP IP
  IPAddress stationIP   = IPAddress(0, 0, 0, 0);
  IPAddress gatewayIP   = IPAddress(192, 168, 2, 1);
  IPAddress netMask     = IPAddress(255, 255, 255, 0);
#else
  // Use static IP
  #warning Using static IP
  #ifdef ESP32
    IPAddress stationIP   = IPAddress(192, 168, 2, 232);
  #else
    IPAddress stationIP   = IPAddress(192, 168, 2, 186);
  #endif
  
  IPAddress gatewayIP   = IPAddress(192, 168, 2, 1);
  IPAddress netMask     = IPAddress(255, 255, 255, 0);
#endif

#define USE_CONFIGURABLE_DNS      true

IPAddress dns1IP      = gatewayIP;
IPAddress dns2IP      = IPAddress(8, 8, 8, 8);

// New in v1.4.0
IPAddress APStaticIP  = IPAddress(192, 168, 100, 1);
IPAddress APStaticGW  = IPAddress(192, 168, 100, 1);
IPAddress APStaticSN  = IPAddress(255, 255, 255, 0);

#include <ESP_WiFiManager.h>              //https://github.com/khoih-prog/ESP_WiFiManager

const char* host = "esp32-fs-browser";

#define HTTP_PORT     80

WebServer server(HTTP_PORT);

void DataReceiveFromWebClient();     // Wenn "http://<ip address>/dataClient2Server" aufgerufen wurde
void DataSendFromToClient();     // Wenn "http://<ip address>/triggerServer2Client" aufgerufen wurde

//holds the current upload
File fsUploadFile;

// Function Prototypes
uint8_t connectMultiWiFi();

///////////////////////////////////////////
// New in v1.4.0
/******************************************
 * // Defined in ESPAsync_WiFiManager.h
typedef struct
{
  IPAddress _ap_static_ip;
  IPAddress _ap_static_gw;
  IPAddress _ap_static_sn;

}  WiFi_AP_IPConfig;

typedef struct
{
  IPAddress _sta_static_ip;
  IPAddress _sta_static_gw;
  IPAddress _sta_static_sn;
#if USE_CONFIGURABLE_DNS  
  IPAddress _sta_static_dns1;
  IPAddress _sta_static_dns2;
#endif
}  WiFi_STA_IPConfig;
******************************************/

WiFi_AP_IPConfig  WM_AP_IPconfig;
WiFi_STA_IPConfig WM_STA_IPconfig;

void initAPIPConfigStruct(WiFi_AP_IPConfig &in_WM_AP_IPconfig)
{
  in_WM_AP_IPconfig._ap_static_ip   = APStaticIP;
  in_WM_AP_IPconfig._ap_static_gw   = APStaticGW;
  in_WM_AP_IPconfig._ap_static_sn   = APStaticSN;
}

void initSTAIPConfigStruct(WiFi_STA_IPConfig &in_WM_STA_IPconfig)
{
  in_WM_STA_IPconfig._sta_static_ip   = stationIP;
  in_WM_STA_IPconfig._sta_static_gw   = gatewayIP;
  in_WM_STA_IPconfig._sta_static_sn   = netMask;
#if USE_CONFIGURABLE_DNS  
  in_WM_STA_IPconfig._sta_static_dns1 = dns1IP;
  in_WM_STA_IPconfig._sta_static_dns2 = dns2IP;
#endif
}

void displayIPConfigStruct(WiFi_STA_IPConfig in_WM_STA_IPconfig)
{
  LOGERROR3(F("stationIP ="), in_WM_STA_IPconfig._sta_static_ip, F(", gatewayIP ="), in_WM_STA_IPconfig._sta_static_gw);
  LOGERROR1(F("netMask ="), in_WM_STA_IPconfig._sta_static_sn);
#if USE_CONFIGURABLE_DNS
  LOGERROR3(F("dns1IP ="), in_WM_STA_IPconfig._sta_static_dns1, F(", dns2IP ="), in_WM_STA_IPconfig._sta_static_dns2);
#endif
}

void configWiFi(WiFi_STA_IPConfig in_WM_STA_IPconfig)
{
  #if USE_CONFIGURABLE_DNS  
    // Set static IP, Gateway, Subnetmask, DNS1 and DNS2. New in v1.0.5
    WiFi.config(in_WM_STA_IPconfig._sta_static_ip, in_WM_STA_IPconfig._sta_static_gw, in_WM_STA_IPconfig._sta_static_sn, in_WM_STA_IPconfig._sta_static_dns1, in_WM_STA_IPconfig._sta_static_dns2);  
  #else
    // Set static IP, Gateway, Subnetmask, Use auto DNS1 and DNS2.
    WiFi.config(in_WM_STA_IPconfig._sta_static_ip, in_WM_STA_IPconfig._sta_static_gw, in_WM_STA_IPconfig._sta_static_sn);
  #endif 
}

///////////////////////////////////////////

uint8_t connectMultiWiFi()
{
#if ESP32
  // For ESP32, this better be 0 to shorten the connect time. 
  // For ESP32-S2, must be > 500
  #if ( ARDUINO_ESP32S2_DEV || ARDUINO_FEATHERS2 || ARDUINO_PROS2 || ARDUINO_MICROS2 )
    #define WIFI_MULTI_1ST_CONNECT_WAITING_MS           500L
  #else
    // For ESP32 core v1.0.6, must be >= 500
    #define WIFI_MULTI_1ST_CONNECT_WAITING_MS           800L
  #endif
#else
  // For ESP8266, this better be 2200 to enable connect the 1st time
  #define WIFI_MULTI_1ST_CONNECT_WAITING_MS             2200L
#endif

#define WIFI_MULTI_CONNECT_WAITING_MS                   100L
  
  uint8_t status;

  LOGERROR(F("ConnectMultiWiFi with :"));
  
  if ( (Router_SSID != "") && (Router_Pass != "") )
  {
    LOGERROR3(F("* Flash-stored Router_SSID = "), Router_SSID, F(", Router_Pass = "), Router_Pass );
  }

  for (uint8_t i = 0; i < NUM_WIFI_CREDENTIALS; i++)
  {
    // Don't permit NULL SSID and password len < MIN_AP_PASSWORD_SIZE (8)
    if ( (String(WM_config.WiFi_Creds[i].wifi_ssid) != "") && (strlen(WM_config.WiFi_Creds[i].wifi_pw) >= MIN_AP_PASSWORD_SIZE) )
    {
      LOGERROR3(F("* Additional SSID = "), WM_config.WiFi_Creds[i].wifi_ssid, F(", PW = "), WM_config.WiFi_Creds[i].wifi_pw );
    }
  }
  
  LOGERROR(F("Connecting MultiWifi..."));

  WiFi.mode(WIFI_STA);

#if !USE_DHCP_IP
  // New in v1.4.0
  configWiFi(WM_STA_IPconfig);
  //////
#endif

  int i = 0;
  status = wifiMulti.run();
  delay(WIFI_MULTI_1ST_CONNECT_WAITING_MS);

  while ( ( i++ < 20 ) && ( status != WL_CONNECTED ) )
  {
    status = wifiMulti.run();

    if ( status == WL_CONNECTED )
      break;
    else
      delay(WIFI_MULTI_CONNECT_WAITING_MS);
  }

  if ( status == WL_CONNECTED )
  {
    LOGERROR1(F("WiFi connected after time: "), i);
    LOGERROR3(F("SSID:"), WiFi.SSID(), F(",RSSI="), WiFi.RSSI());
    LOGERROR3(F("Channel:"), WiFi.channel(), F(",IP address:"), WiFi.localIP() );
  }
  else
    LOGERROR(F("WiFi not connected"));

  return status;
}

void heartBeatPrint()
{
  static int num = 1;

  if (WiFi.status() == WL_CONNECTED)
    Serial.print(F("H"));        // H means connected to WiFi
  else
    Serial.print(F("F"));        // F means not connected to WiFi

  if (num == 80)
  {
    Serial.println();
    num = 1;
  }
  else if (num++ % 10 == 0)
  {
    Serial.print(F(" "));
  }
}

void check_WiFi()
{
  if ( (WiFi.status() != WL_CONNECTED) )
  {
    Serial.println(F("\nWiFi lost. Call connectMultiWiFi in loop"));
    connectMultiWiFi();
  }
}  

void check_status()
{
  static ulong checkstatus_timeout  = 0;
  static ulong checkwifi_timeout    = 0;

  static ulong current_millis;

#define WIFICHECK_INTERVAL    1000L
#define HEARTBEAT_INTERVAL    10000L

  current_millis = millis();
  
  // Check WiFi every WIFICHECK_INTERVAL (1) seconds.
  if ((current_millis > checkwifi_timeout) || (checkwifi_timeout == 0))
  {
    check_WiFi();
    checkwifi_timeout = current_millis + WIFICHECK_INTERVAL;
  }

  // Print hearbeat every HEARTBEAT_INTERVAL (10) seconds.
  if ((current_millis > checkstatus_timeout) || (checkstatus_timeout == 0))
  {
    heartBeatPrint();
    checkstatus_timeout = current_millis + HEARTBEAT_INTERVAL;
  }
}

//format bytes
String formatBytes(size_t bytes) 
{
  if (bytes < 1024) 
  {
    return String(bytes) + "B";
  } 
  else if (bytes < (1024 * 1024)) 
  {
    return String(bytes / 1024.0) + "KB";
  } 
  else if (bytes < (1024 * 1024 * 1024)) 
  {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  } 
  else 
  {
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
  }
}

String getContentType(String filename) 
{
  if (server.hasArg("download")) 
  {
    return "application/octet-stream";
  } 
  else if (filename.endsWith(".htm")) 
  {
    return "text/html";
  } 
  else if (filename.endsWith(".html")) 
  {
    return "text/html";
  } 
  else if (filename.endsWith(".css")) 
  {
    return "text/css";
  } 
  else if (filename.endsWith(".js")) 
  {
    return "application/javascript";
  } 
  else if (filename.endsWith(".png")) 
  {
    return "image/png";
  } 
  else if (filename.endsWith(".gif")) 
  {
    return "image/gif";
  } 
  else if (filename.endsWith(".jpg")) 
  {
    return "image/jpeg";
  } 
  else if (filename.endsWith(".ico")) 
  {
    return "image/x-icon";
  } 
  else if (filename.endsWith(".xml")) 
  {
    return "text/xml";
  } 
  else if (filename.endsWith(".pdf")) 
  {
    return "application/x-pdf";
  } 
  else if (filename.endsWith(".zip")) 
  {
    return "application/x-zip";
  } 
  else if (filename.endsWith(".gz")) 
  {
    return "application/x-gzip";
  }
  return "text/plain";
}

bool handleFileRead(String path) 
{
  Serial.println("handleFileRead: " + path);
  
  if (path.endsWith("/")) 
  {
    path += "index.htm";
  }
  
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  
  if (filesystem->exists(pathWithGz) || filesystem->exists(path)) 
  {
    if (filesystem->exists(pathWithGz)) 
    {
      path += ".gz";
    }
    
    File file = filesystem->open(path, "r");
    server.streamFile(file, contentType);
    file.close();
    return true;
  }
  
  return false;
}

void handleFileUpload() 
{
  if (server.uri() != "/edit") 
  {
    return;
  }
  
  HTTPUpload& upload = server.upload();
  
  if (upload.status == UPLOAD_FILE_START) 
  {
    String filename = upload.filename;
    if (!filename.startsWith("/")) 
    {
      filename = "/" + filename;
    }
    
    Serial.print(F("handleFileUpload Name: ")); Serial.println(filename);
    fsUploadFile = filesystem->open(filename, "w");
    filename.clear();
  } 
  else if (upload.status == UPLOAD_FILE_WRITE) 
  {
    //Serial.print(F("handleFileUpload Data: ")); Serial.println(upload.currentSize);
    if (fsUploadFile) 
    {
      fsUploadFile.write(upload.buf, upload.currentSize);
    }
  } 
  else if (upload.status == UPLOAD_FILE_END) 
  {
    if (fsUploadFile) 
    {
      fsUploadFile.close();
    }
    Serial.print(F("handleFileUpload Size: ")); Serial.println(upload.totalSize);
  } 
}

void handleFileDelete() 
{
  if (server.args() == 0) 
  {
    return server.send(500, "text/plain", "BAD ARGS");
  }
  String path = server.arg(0);
  Serial.println("handleFileDelete: " + path);
  
  if (path == "/") 
  {
    return server.send(500, "text/plain", "BAD PATH");
  }
  
  if (!filesystem->exists(path)) 
  {
    return server.send(404, "text/plain", "FileNotFound");
  }
  
  filesystem->remove(path);
  server.send(200, "text/plain", "");
  path.clear();
}

void handleFileCreate() 
{
  if (server.args() == 0) 
  {
    return server.send(500, "text/plain", "BAD ARGS");
  }
  
  String path = server.arg(0);
  Serial.println("handleFileCreate: " + path);
  
  if (path == "/") 
  {
    return server.send(500, "text/plain", "BAD PATH");
  }
  
  if (filesystem->exists(path)) 
  {
    return server.send(500, "text/plain", "FILE EXISTS");
  }
  
  File file = filesystem->open(path, "w");
  
  if (file) 
  {
    file.close();
  } 
  else 
  {
    return server.send(500, "text/plain", "CREATE FAILED");
  }
  server.send(200, "text/plain", "");
  path.clear();
}

void handleFileList() 
{
  if (!server.hasArg("dir")) 
  {
    server.send(500, "text/plain", "BAD ARGS");
    return;
  }

  String path = server.arg("dir");
  Serial.println("handleFileList: " + path);
  
  File root = filesystem->open(path);
  path = String();

  String output = "[";
  
  if (root.isDirectory()) 
  {
    File file = root.openNextFile();
    
    while (file) 
    {
      if (output != "[") 
      {
        output += ',';
      }
      
      output += "{\"type\":\"";
      output += (file.isDirectory()) ? "dir" : "file";
      output += "\",\"name\":\"";
      output += String(file.name()).substring(1);
      output += "\"}";
      file = root.openNextFile();
    }
  }
  
  output += "]";

  Serial.println("handleFileList: " + output);

  server.send(200, "text/json", output);
}

bool loadConfigData()
{
  File file = FileFS.open(CONFIG_FILENAME, "r");
  LOGERROR(F("LoadWiFiCfgFile "));

  memset(&WM_config,       0, sizeof(WM_config));

  // New in v1.4.0
  memset(&WM_STA_IPconfig, 0, sizeof(WM_STA_IPconfig));
  //////
    
  if (file)
  {
    file.readBytes((char *) &WM_config,   sizeof(WM_config));

    // New in v1.4.0
    file.readBytes((char *) &WM_STA_IPconfig, sizeof(WM_STA_IPconfig));
    //////
    
    file.close();
    LOGERROR(F("OK"));

    // New in v1.4.0
    displayIPConfigStruct(WM_STA_IPconfig);
    //////

    return true;
  }
  else
  {
    LOGERROR(F("failed"));

    return false;
  }
}

void copyDMXToOutput(void);
    
bool loadDmxConfigData()
{
  char buffer[100];
  File file = FileFS.open(CONFIG_DMX_FILENAME, "r");
  LOGERROR(F("LoadDmxCfgFile "));

  memset(&DMX_config,       0, sizeof(DMX_config));

  // New in v1.4.0
  // memset(&WM_STA_IPconfig, 0, sizeof(WM_STA_IPconfig));
  //////
    
  if ((file.size() != sizeof(DMX_config))||(!file)){
    LOGERROR(F("No or Non-matching DmxConfigFile found...Skip reading - Set defaults"));

    numChannels = 10; //channelMode
    ch[0] = 0; //ch1_rIntens
    ch[1] = 0; //ch2_gIntens
    ch[2] = 0; //ch3_bIntens
    ch[3] = 0; //ch4_uvIntens
    ch[4] = 255; //ch5_stobeMode: 255 = "LED Ein"
    ch[5] = 255; //ch6_masterDimmer: 255 = "Vollgas"
    ch[6] = 175; //ch7_modeSelect: 175 = "Farb-Fade-Modus"
    ch[7] = 7*16; //ch8_progSel: Irgendwas zwischen 0 u. 15 *16 (wenn ch[6] in Farbwechsel, Farbfade u. Musiksteuerungsmodus)
    ch[8] = 127; //ch9_speedSenseSel: 127 = "Mittlere Geschwindigkeit"
    ch[9] = 200; //ch10_dimCurve: 200 = "Standard auf Ger??t"
    universe = 0; //universeSet
    startAddr = 0; //startAddrSet
  }
  else{
    file.readBytes((char *) &DMX_config,   sizeof(DMX_config));

    // New in v1.4.0
    //file.readBytes((char *) &WM_STA_IPconfig, sizeof(WM_STA_IPconfig));
    //////
    
    file.close();
    LOGERROR(F("OK"));

    numChannels = DMX_config.chNum;
    startAddr = DMX_config.startChannel;

    for(int i=0;i<10;i++){
      ch[i] = DMX_config.ch[i];
    }
    universe = DMX_config.universe;
  }

  Serial.println("Loaded from DMXconfigFile:");
  sprintf(buffer, "Rcv: numCh: %d", numChannels);
  Serial.println(buffer);
  sprintf(buffer, "Rcv: Ch[1..10]: %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", ch[0], ch[1], ch[2], ch[3], ch[4], ch[5], ch[6], ch[7], ch[8], ch[9]);
  Serial.println(buffer);
  sprintf(buffer, "Rcv: universe: %d", universe);
  Serial.println(buffer);

  int chStart = startAddr;
  for (int i = 0; i < numChannels; i++)
  {
    dmxbuffer[((chStart+i)%DMX_CHANNELS)+1] = ch[i];
  }
  //write new dmx-channel-values to output
  copyDMXToOutput();

  // New in v1.4.0
  //displayIPConfigStruct(WM_STA_IPconfig);
  //////
  dmxConfigFileOk = true;
  return true;
}

void saveConfigData()
{
  File file = FileFS.open(CONFIG_FILENAME, "w");
  LOGERROR(F("SaveWiFiCfgFile "));

  if (file)
  {
    file.write((uint8_t*) &WM_config,   sizeof(WM_config));

    displayIPConfigStruct(WM_STA_IPconfig);

    // New in v1.4.0
    file.write((uint8_t*) &WM_STA_IPconfig, sizeof(WM_STA_IPconfig));
    //////
    
    file.close();
    LOGERROR(F("OK"));
  }
  else
  {
    LOGERROR(F("failed"));
  }
}

void saveDmxConfigData()
{
  File file = FileFS.open(CONFIG_DMX_FILENAME, "w");
  LOGERROR(F("SaveDmxCfgFile "));

  if (file)
  {
    file.write((uint8_t*) &DMX_config,   sizeof(DMX_config));

    //displayIPConfigStruct(WM_STA_IPconfig);

    // New in v1.4.0
    //file.write((uint8_t*) &WM_STA_IPconfig, sizeof(WM_STA_IPconfig));
    //////
    
    file.close();
    LOGERROR(F("OK"));
  }
  else
  {
    LOGERROR(F("failed"));
  }
}


void clearDisplayLine(uint8_t lineStartY, uint8_t lineHeightY)
{
  display.setColor(BLACK);
  display.fillRect(0, lineStartY, display.getWidth(), lineHeightY);
  display.setColor(WHITE);
  display.display();
}

void copyDMXToOutput(void) {
  xSemaphoreTake( ESP32DMX.lxDataLock, portMAX_DELAY );
	for (int i=1; i<DMX_MAX_FRAME; i++) {
    	ESP32DMX.setSlot(i , dmxbuffer[i]);
   }
   xSemaphoreGive( ESP32DMX.lxDataLock );
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data)
{
  boolean tail = false;
    
  Serial.println("DMX: Univ: " + String(universe) + ", Seq: " + sequence + ", Data (" + length + "):");
  
  display.println("Connecting to WiFi");
  display.display();  
  if (length > DMX_CHANNELS) {
    length = DMX_CHANNELS;
  }
  // send out the buffer

  sprintf(lcdBuffer, "Data rcv. univ.: %d ", universe);
  
  // sprintf(lcdBuffer, "RX (%d): %d,%d,%d", length, data[0], data[1], data[2]);
  //sprintf(lcdBuffer, "RX");
  if (universeSelect == universe){
    for (int i = 0; i < DMX_CHANNELS; i++)
    {
      if (i<=16){
        Serial.print(String(data[i]) + " ");
      }
      dmxbuffer[i+1] = data[i];
    }
    //write new dmx-channel-values to output
    copyDMXToOutput();
  }
  else{
    char skipUseIndicator[] = "(skp)";
    strcat(lcdBuffer,skipUseIndicator);
  }

  clearDisplayLine(22, 12);
  display.drawString(0, 22, lcdBuffer);
  display.display();

  Serial.println("...");
}

void startServer(){
  //SERVER INIT
  //list directory
  server.on("/list", HTTP_GET, handleFileList);
  
  //load editor
  server.on("/edit", HTTP_GET, []() 
  {
    if (!handleFileRead("/edit.htm")) 
    {
      server.send(404, "text/plain", "FileNotFound");
    }
  });
  
  //create file
  server.on("/edit", HTTP_PUT, handleFileCreate);
  
  //delete file
  server.on("/edit", HTTP_DELETE, handleFileDelete);
  
  //first callback is called after the request has ended with all parsed arguments
  //second callback handles file uploads at that location
  server.on("/edit", HTTP_POST, []() 
  {
    server.send(200, "text/plain", "");
  }, handleFileUpload);

  //called when the url is not defined here
  //use it to load content from SPIFFS
  server.onNotFound([]() 
  {
    if (!handleFileRead(server.uri())) 
    {
      server.send(404, "text/plain", "FileNotFound");
    }
  });

  //get heap status, analog input value and all GPIO statuses in one json call
  server.on("/all", HTTP_GET, []() 
  {
    String json = "{";
    json += "\"heap\":" + String(ESP.getFreeHeap());
    json += ", \"analog\":" + String(analogRead(A0));
    json += ", \"gpio\":" + String((uint32_t)(0));
    json += "}";
    server.send(200, "text/json", json);
    json = String();
  });

  server.on("/triggerServer2Client", DataSendFromToClient);

  server.on("/dataClient2Server", DataReceiveFromWebClient);

  server.begin();
}

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  delay(200);

  Serial.print(F("\nStarting ESP32_FSWebServer_DRD with DoubleResetDetect using ")); Serial.print(FS_Name);
  Serial.print(F(" on ")); Serial.println(ARDUINO_BOARD);
  Serial.println(ESP_WIFIMANAGER_VERSION);
  Serial.println(ESP_DOUBLE_RESET_DETECTOR_VERSION);

  if ( String(ESP_WIFIMANAGER_VERSION) < ESP_WIFIMANAGER_VERSION_MIN_TARGET )
  {
    Serial.print(F("Warning. Must use this example on Version equal or later than : "));
    Serial.println(ESP_WIFIMANAGER_VERSION_MIN_TARGET);
  }
  
  Serial.setDebugOutput(false);

  drd = new DoubleResetDetector(DRD_TIMEOUT, DRD_ADDRESS);

  if (FORMAT_FILESYSTEM) 
    FileFS.format();

  // Format FileFS if not yet
  if (!FileFS.begin(true))
  {
    Serial.println(F("SPIFFS/LittleFS failed! Already tried formatting."));
  
    if (!FileFS.begin())
    {     
      // prevents debug info from the library to hide err message.
      delay(100);
      
#if USE_LITTLEFS
      Serial.println(F("LittleFS failed!. Please use SPIFFS or EEPROM. Stay forever"));
#else
      Serial.println(F("SPIFFS failed!. Please use LittleFS or EEPROM. Stay forever"));
#endif

      while (true)
      {
        delay(1);
      }
    }
  }

  Serial.println("List files...");

  File root = filesystem->open("/");
  File file = root.openNextFile();
  
  while (file) 
  {
    String fileName = file.name();
    size_t fileSize = file.size();
    Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
    file = root.openNextFile();
  }
  
  Serial.println();

  unsigned long startedAt = millis();

  // New in v1.4.0
  initAPIPConfigStruct(WM_AP_IPconfig);
  initSTAIPConfigStruct(WM_STA_IPconfig);
  //////

  //Local intialization. Once its business is done, there is no need to keep it around
  // Use this to default DHCP hostname to ESP8266-XXXXXX or ESP32-XXXXXX
  //ESP_WiFiManager ESP_wifiManager;
  // Use this to personalize DHCP hostname (RFC952 conformed)
  ESP_WiFiManager ESP_wifiManager("ESP32-FSWebServer_DRD");

  //set custom ip for portal
  // New in v1.4.0
  ESP_wifiManager.setAPStaticIPConfig(WM_AP_IPconfig);
  //////

  ESP_wifiManager.setMinimumSignalQuality(-1);

  // From v1.0.10 only
  // Set config portal channel, default = 1. Use 0 => random channel from 1-13
  ESP_wifiManager.setConfigPortalChannel(0);
  //////
  
#if !USE_DHCP_IP    
    // Set (static IP, Gateway, Subnetmask, DNS1 and DNS2) or (IP, Gateway, Subnetmask). New in v1.0.5
    // New in v1.4.0
    ESP_wifiManager.setSTAStaticIPConfig(WM_STA_IPconfig);
    //////
#endif

  // New from v1.1.1
#if USING_CORS_FEATURE
  ESP_wifiManager.setCORSHeader("Your Access-Control-Allow-Origin");
#endif

  // We can't use WiFi.SSID() in ESP32as it's only valid after connected.
  // SSID and Password stored in ESP32 wifi_ap_record_t and wifi_config_t are also cleared in reboot
  // Have to create a new function to store in EEPROM/SPIFFS for this purpose
  Router_SSID = ESP_wifiManager.WiFi_SSID();
  Router_Pass = ESP_wifiManager.WiFi_Pass();

  //Remove this line if you do not want to see WiFi password printed
  Serial.println("ESP Self-Stored: SSID = " + Router_SSID + ", Pass = " + Router_Pass);

  // SSID to uppercase
  ssid.toUpperCase();

  bool configDataLoaded = false;

  // From v1.1.0, Don't permit NULL password
  if ( (Router_SSID != "") && (Router_Pass != "") )
  {
    LOGERROR3(F("* Add SSID = "), Router_SSID, F(", PW = "), Router_Pass);
    wifiMulti.addAP(Router_SSID.c_str(), Router_Pass.c_str());
    
    ESP_wifiManager.setConfigPortalTimeout(120); //If no access point name has been previously entered disable timeout.
    Serial.println(F("Got ESP Self-Stored Credentials. Timeout 120s for Config Portal"));
  }
  else if (loadConfigData())
  {
    configDataLoaded = true;
    
    ESP_wifiManager.setConfigPortalTimeout(120); //If no access point name has been previously entered disable timeout.
    Serial.println(F("Got stored Credentials. Timeout 120s for Config Portal")); 
  }
  else
  {
    // Enter CP only if no stored SSID on flash and file 
    Serial.println(F("Open Config Portal without Timeout: No stored Credentials."));
    initialConfig = true;
  }

  if (drd->detectDoubleReset())
  {
    // DRD, disable timeout.
    ESP_wifiManager.setConfigPortalTimeout(0);
    
    Serial.println(F("Open Config Portal without Timeout: Double Reset Detected"));
    initialConfig = true;
  }

  if (initialConfig)
  {
    // Starts an access point
    if (!ESP_wifiManager.startConfigPortal((const char *) ssid.c_str(), password))
      Serial.println(F("Not connected to WiFi but continuing anyway."));
    else
    {
      Serial.println(F("WiFi connected...yeey :)"));
    }

    // Stored  for later usage, from v1.1.0, but clear first
    memset(&WM_config, 0, sizeof(WM_config));
    
    for (uint8_t i = 0; i < NUM_WIFI_CREDENTIALS; i++)
    {
      String tempSSID = ESP_wifiManager.getSSID(i);
      String tempPW   = ESP_wifiManager.getPW(i);
  
      if (strlen(tempSSID.c_str()) < sizeof(WM_config.WiFi_Creds[i].wifi_ssid) - 1)
        strcpy(WM_config.WiFi_Creds[i].wifi_ssid, tempSSID.c_str());
      else
        strncpy(WM_config.WiFi_Creds[i].wifi_ssid, tempSSID.c_str(), sizeof(WM_config.WiFi_Creds[i].wifi_ssid) - 1);

      if (strlen(tempPW.c_str()) < sizeof(WM_config.WiFi_Creds[i].wifi_pw) - 1)
        strcpy(WM_config.WiFi_Creds[i].wifi_pw, tempPW.c_str());
      else
        strncpy(WM_config.WiFi_Creds[i].wifi_pw, tempPW.c_str(), sizeof(WM_config.WiFi_Creds[i].wifi_pw) - 1);  

      // Don't permit NULL SSID and password len < MIN_AP_PASSWORD_SIZE (8)
      if ( (String(WM_config.WiFi_Creds[i].wifi_ssid) != "") && (strlen(WM_config.WiFi_Creds[i].wifi_pw) >= MIN_AP_PASSWORD_SIZE) )
      {
        LOGERROR3(F("* Add SSID = "), WM_config.WiFi_Creds[i].wifi_ssid, F(", PW = "), WM_config.WiFi_Creds[i].wifi_pw );
        wifiMulti.addAP(WM_config.WiFi_Creds[i].wifi_ssid, WM_config.WiFi_Creds[i].wifi_pw);
      }
    }

    // New in v1.4.0
    ESP_wifiManager.getSTAStaticIPConfig(WM_STA_IPconfig);
    //////
    
    saveConfigData();
  }

  startedAt = millis();

  if (!initialConfig)
  {
    // Load stored data, the addAP ready for MultiWiFi reconnection
    if (!configDataLoaded)
      loadConfigData();

    for (uint8_t i = 0; i < NUM_WIFI_CREDENTIALS; i++)
    {
      // Don't permit NULL SSID and password len < MIN_AP_PASSWORD_SIZE (8)
      if ( (String(WM_config.WiFi_Creds[i].wifi_ssid) != "") && (strlen(WM_config.WiFi_Creds[i].wifi_pw) >= MIN_AP_PASSWORD_SIZE) )
      {
        LOGERROR3(F("* Add SSID = "), WM_config.WiFi_Creds[i].wifi_ssid, F(", PW = "), WM_config.WiFi_Creds[i].wifi_pw );
        wifiMulti.addAP(WM_config.WiFi_Creds[i].wifi_ssid, WM_config.WiFi_Creds[i].wifi_pw);
      }
    }

    if ( WiFi.status() != WL_CONNECTED ) 
    {
      Serial.println(F("ConnectMultiWiFi in setup"));
     
      connectMultiWiFi();
    }
  }

  IPAddress ip;

  Serial.print(F("After waiting "));
  Serial.print((float) (millis() - startedAt) / 1000);
  Serial.print(F(" secs more in setup(), connection result is "));

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print(F("connected. Local IP: "));
    ip = WiFi.localIP();
    Serial.println(ip);

    //Initialize display -> 4: SDA, 15: SCL
    Wire.begin(DISPLAY_SDA_PIN, DISPLAY_SCL_PIN);
    //Pull displays reset signal -> 16: reset-pin
    pinMode(DISPLAY_RESET_PIN,OUTPUT); 
    digitalWrite(DISPLAY_RESET_PIN, LOW); 
    delay(50); 
    digitalWrite(DISPLAY_RESET_PIN, HIGH);
    
    Serial.print(F("Display out:"));
    display.init();
    display.setBrightness(255);
    display.clear();
    display.setColor(WHITE);
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);

    display.drawString(0, 0, "ArtNet DMX server");
    sprintf(lcdBuffer, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    display.drawString(0, 33, WiFi.SSID());
    display.drawString(0, 44, lcdBuffer);
    display.display();
  }
  else
    Serial.println(ESP_wifiManager.getStatus(WiFi.status()));

  startServer();
  
  Serial.print(F("HTTP server started @ "));
  Serial.println(WiFi.localIP());

  MDNS.begin(host);
  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", HTTP_PORT);

  Serial.print(F("Open http://"));
  Serial.print(host);
  Serial.println(F(".local/edit to see the file browser"));

  // this will be called for each packet received
  artnet.setArtDmxCallback(onDmxFrame);
  artnet.begin();

  pinMode(DMX_DIRECTION_PIN, OUTPUT);
  digitalWrite(DMX_DIRECTION_PIN, HIGH);

  pinMode(DMX_SERIAL_OUTPUT_PIN, OUTPUT);
  ESP32DMX.startOutput(DMX_SERIAL_OUTPUT_PIN);

  pinMode(DMX_SEQUENCE_UNIVERSE_SELECTOR, INPUT_PULLUP);

  //draw universe once in setup  - afterwards - on update only - in main-loop
  //Serial.print("Universe: ");
  //Serial.println(universeSelect);    
  sprintf(lcdBuffer, "Universe: %d", universeSelect);
  display.drawString(0, 11, lcdBuffer);
  display.display();

  Serial.println("setup complete");

  loadDmxConfigData();

}

int buttonState = 0;         // variable for reading the pushbutton status
int buttonStatePrev = 0;     // variable for reading the pushbutton status
int longKeyPressCnt = 0;        // variable for reading the pushbutton status

void loop() 
{
  artnet.read();
  esp_task_wdt_feed();
  vTaskDelay(100);
  // Call the double reset detector loop method every so often,
  // so that it can recognise when the timeout expires.
  // You can also call drd.stop() when you wish to no longer
  // consider the next reset as a double reset.
  drd->loop();

  // this is just for checking if we are alive and connected to WiFi
  check_status();
  
  server.handleClient();

  // read the state of the pushbutton value:
  buttonState = digitalRead(DMX_SEQUENCE_UNIVERSE_SELECTOR);
  if ((buttonState == LOW) && (buttonStatePrev == HIGH)) {     
    // turn LED on:    
    universeSelect=(universeSelect+1)%256;
    longKeyPressCnt=0;

    Serial.print("Universe: ");
    Serial.println(universeSelect);    
    sprintf(lcdBuffer, "Universe: %d", universeSelect);

    clearDisplayLine(11, 10);
    display.drawString(0, 11, lcdBuffer);
    display.display();
  } 
  else if (buttonState == LOW){
    longKeyPressCnt=(longKeyPressCnt+1)%256; 
    if( longKeyPressCnt>30 ){
      universeSelect=0;
      longKeyPressCnt=0;

      Serial.print("Universe: ");
      Serial.println(universeSelect);    
      sprintf(lcdBuffer, "Universe: %d", universeSelect);
      
      clearDisplayLine(11, 10);
      display.drawString(0, 11, lcdBuffer);
      display.display();
    }
  }
  buttonStatePrev = buttonState;
  
  clearDisplayLine(22, 12);
  display.display();

}

void DataReceiveFromWebClient()     // Wenn "http://<ip address>/dataClient2Server" aufgerufen wurde
{

  char buffer[80];
  String saveAll = "Init";
  
  webPageInitalConnect = (bool)(server.arg("iRcv")).toInt();
  // only after webPageInitalConnect (1) has been returned from webpage, use data from webpage - before, keep data from config file (or defaults)!
  if (webPageInitalConnect){

    numChannels = (server.arg("chN")).toInt();
    ch[0] = (server.arg("ch1")).toInt();
    ch[1] = (server.arg("ch2")).toInt();
    ch[2] = (server.arg("ch3")).toInt();
    ch[3] = (server.arg("ch4")).toInt();
    ch[4] = (server.arg("ch5")).toInt();
    ch[5] = (server.arg("ch6")).toInt();
    ch[6] = (server.arg("ch7")).toInt();
    ch[7] = (server.arg("ch8")).toInt();
    ch[8] = (server.arg("ch9")).toInt();
    ch[9] = (server.arg("ch10")).toInt();
    saveAll = server.arg("save");
    universe = (server.arg("univ")).toInt();
    startAddr = (server.arg("startAddr")).toInt();
  }

  sprintf(buffer, "Rcv: numCh: %d", numChannels);
  Serial.println(buffer);
  sprintf(buffer, "Rcv: Ch[1..10]: %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", ch[0], ch[1], ch[2], ch[3], ch[4], ch[5], ch[6], ch[7], ch[8], ch[9]);
  Serial.println(buffer);
  sprintf(buffer, "Rcv: universe/saveall: %d/%s", universe, saveAll);
  Serial.println(buffer);

  int chStart = startAddr;
  for (int i = 0; i < numChannels; i++)
  {
    dmxbuffer[((chStart+i)%DMX_CHANNELS)+1] = ch[i];
  }
  //write new dmx-channel-values to output
  copyDMXToOutput();

  bool newCmd = true;

  if(saveAll == "true"){

    for(int i=0;i<10;i++){
      DMX_config.ch[i] = ch[i];
    }
    DMX_config.chNum = numChannels;
    DMX_config.universe = universe;
    DMX_config.startChannel = startAddr;

    saveDmxConfigData();

  }
}


void DataSendFromToClient()     // Wenn "http://<ip address>/triggerServer2Client" aufgerufen wurde
{
  server.sendHeader("Cache-Control", "no-cache");  // Sehr wichtig !!!!!!!!!!!!!!!!!!!
  
  //sending "true" as last parameter to check if data has been received by webpage
  server.send(200, "text/plain", (String(ch[0]) + "|" + String(ch[1]) + "|" + String(ch[2]) + "|" + String(ch[3]) + "|" + String(ch[4])+ "|" + String(ch[5])+ "|" + String(ch[6])+ "|" + String(ch[7])+ "|" + String(ch[8])+ "|" + String(ch[9])+ "|" + String(numChannels)+ "|" + String(startAddr)+ "|" + String(universe)+ "|1"));
  
}