/*
  Set and Get Time And Date
  Author: Denics Devices (Aaditya Dengle)
  Date: Sep 2024
*/

#include "src/DS1307.h"

#define testSet 1        // define this if set of RTC is needed

//DS1307 rtc(0x68,6,7,&Wire1);  // initiate library using Wire1 (I2C1) instance on RP2040 initiated on SDA=Pin(6) and SCL=Pin(7)
DS1307 rtc(0x68, 8, 9, &Wire);  // initiate library using Wire (I2C0) instance on RP2040 initiated on SDA (GPIO8) and SCL (GPIO9)

#ifdef testSet
  unsigned long beginSetMillis = 0;
  unsigned long endSetMillis = 0;
#endif

uint8_t dataRam[RAMSIZE];

void setup() {
  Serial.begin(115200);
  while (!Serial) ;   // wait for serial ready
  delay(200);

  if (rtc.begin()) {    // if access to RTC registers is OK 
#ifdef testSet    
    beginSetMillis = millis();  // check if registers set within 1 second                          
      rtc.setFormat(12);        // Set 12 Hours Format
      rtc.setAMPM(1);           // Set AM or PM    0 = AM  1 =PM
      rtc.setTime("11:59:30");  // Set Time    Hour:Minute:Seconds (always after AM/PM register update)
      rtc.setDate("12/31/23");  // Set Date    Month/Date/Year
      rtc.setDay(6);            // 6 for Sunday, see _weekDays[][]
    endSetMillis = millis();
    Serial.printf("time needed for RTC set (ms): %lu \n", endSetMillis - beginSetMillis);
#endif
  }
  else {
    Serial.print("No access to RTC registers - Check connexions then reboot \n");
    while(1);
  }
  rtc.getRam(dataRam);
  Serial.println("index : data");
  for (byte i = 0; i < RAMSIZE; i = i + 1) {
    Serial.printf("%02X : %02X \n", i, dataRam[i]);
  }
}

void loop() {
  if (rtc.getTime()) {      // all registers are read in one step
    Serial.printf("Time %02d:%02d:%02d ", rtc.getHour(), rtc.getMinute(), rtc.getSeconds());

    if (rtc.getFormat() == 12) {  // returns 12 or 24 hour format

      if (rtc.getAMPM()) {  //return 0 = AM  1 = PM
        Serial.print("PM  ");
      } else {
        Serial.print("AM  ");
      }
    }
    Serial.printf("Date %s, %02d-%02d-%d \n ", rtc.getDay(), rtc.getMonth(), rtc.getDate(), rtc.getYear());
  }
  else {
    Serial.println("Error when reading RTC");
  }
  delay(1000);
}