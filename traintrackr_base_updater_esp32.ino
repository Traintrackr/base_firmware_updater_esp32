/* *************************
 *  traintrackr base updater for esp32 based boards
 *  Designed by Richard Hawthorn
 *  2024-01-08
 *  
 *  Allows firmware to be downloaded and installed from the traintrackr servers
 *  
 *  **** IMPORTANT PROGRAMMING STEPS **** 
 *  
 *  Install Arduino IDE - https://www.arduino.cc/en/Main/Software
 *  Install ESP32 Board software - https://github.com/espressif/arduino-esp32
 *  
 *  If you haven't touched the file system on the esp32 then your previous board settings should still work, set erase flash settings as 'only sketch'
 *  tools -> Erase flash -> only sketch
 *  
 *  If you have modified the file system, or are having problems getting the board working, then you can wipe the flash using 'all flash content'
 *  tools -> Erase flash ->  all flash content
 *  
 *  Board Settings:
 *  
 *  Set board type to: ESP32 Dev Module
 *  Set the flash size to: Minimal SPIFFS 4MB with spiffs (1.9MB APP with OTA, 190MB SPIFFS)
 *  Set the upload speed to: 460800
 *  Set serial port to the correct port
 *  
 *  Then set the WiFi and update_code variables below, Once this has been done you can flash the board.
 *  
 *  Once the board is programmed you can power cycle, and use the serial monitor (set to 115200) to view the update process
 *  
 *  You may not receive a success message through the serial port, but if you don't receive a "Update Failed" message, the update was a success.   
 *  If the Status LEDs come on when powered, then the update was a success.
 *  
 *  If the filesystem was previously untouched your board may spring back to life, if it doesn't you should go through the setup process
 *  on the traintrackr website.
 *   
 *  Troubleshooting
 *  
 *  - If you receive a "Forbidden (403)" message, it is probably due to an incorrect update code.  
 *  These codes only work once, so you may need to request a second if you have already tried with this code
 *  
 *  - If you receive a "HTTP error: connection refused" message it means it didn't manage to connect to the WiFi
 *  It will try again until it connects succesfully, but you can also check the SSID and password to make sure they are correct
 *  
 *  
 *  -------------------
 *  
 *  
 *  Enter your WiFi SSID and Password 
 */
#define STASSID "enter-ssid-here"
#define STAPSK  "enter-password-here"
/* 
 *  
 * Enter your update code (speak to us to get this)
 */
char update_code[] = "enter-update-code-here";
/* 
 *  
 * use the first line for boards purchased in UK/Europe.
 * use the second line for boards purchased in the US.
 */
char host[] = "api.traintrackrdata.co.uk/api/ota/download";
//char host[] = "api.traintrackrdata.com/api/ota/download";
/* 
 *  
 *  ----------------------------
 *  
 *  Don't touch the code below
 *  
 *  
 */
#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPUpdate.h>

int minuteNow = 0;
int minuteLast = 999;
const char* ssid     = STASSID;
const char* password = STAPSK;


void setup() {

  //start serial
  Serial.begin(115200);

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("Wait %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  Serial.println("");
  Serial.println("traintrackr updater");
  Serial.println("");

  //start WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
}



void loop() {

  minuteNow = round(millis() / 1000 / 60);
  if (minuteNow != minuteLast){ 
    minuteLast = minuteNow;

    //check for updates
    Serial.println("Checking for ota update");

    WiFiClient ota_client;

    String url = "http://";
    url += host;
    url += "?code=";
    url += update_code;

    Serial.print("Update URL: ");
    Serial.println(url);

    Serial.println("Processing...");
    
    t_httpUpdate_return ret = httpUpdate.update(ota_client, url);

    switch (ret) {
      case HTTP_UPDATE_FAILED:

        Serial.println("Update Failed");
        Serial.println(httpUpdate.getLastErrorString());
        Serial.println("Trying again in 1 minute");
        Serial.println("");
        break;

      case HTTP_UPDATE_NO_UPDATES:

        Serial.println("Nothing to update");
        Serial.println("");
        break;

      case HTTP_UPDATE_OK:

        Serial.println("Uppdate success");
        Serial.println("");
        break;
    }
    
  }
}
