// RTC-Test2.ino

// Example sketch to set time/date/alarm on DS3231 RTC Module
// then continuously display time/date

// Creative Commons BY SA licence. Author T. Gillett
// Adapted from Tronix Labs  http://tronixstuff.com/

// This example is designed to run on an Arduino board such as the UNO or MEGA
// with a DS3231 RTC module connected using the I2C bus.
// Connect the module to GND, 3.3V (VCC), SDA, SCL pins on the Arduino board.

// The serial port outputs messages to the Serial Monitor showing the operation.


#define DS3231_I2C_ADDRESS 0x68 // I2C address of RTC chip

#include "Wire.h"								// I2C library

#include "functions.h"					// Include file containing the time/date access functions


void setup()
{
  Wire.begin();
  Serial.begin(9600);

  Serial.println("RTC-Test2 \n");
  Serial.println("Set time, date, alarms \n");
  
  // Set the initial time here if required.
  // Uncomment lines below as required to initialise the time/date/alarm settings.

  // DS3231 seconds, minutes, hours, day-of-week, date, month, year
  // setDS3231time(02,01,12,1,27,11,16);  

  // set_time(12, 01, 02); // hh, mm, ss Set time to 12:01:02
  // set_date(16, 01, 20); // yy, mm, dd Set date to 2016/01/20

	// set_daily_alarm1(13, 01); //Set time to 13:01:02
  // set_daily_alarm2(13, 01); //Set time to 13:01

  Serial.println("Display time, date, alarm data \n");

}

void loop()
{
  Serial.print("\nRTC data : ");
  displayTime();  // display the real-time clock data on the Serial Monitor
  
  Serial.print("get_date : ");
  displayDate();  // display date
  
  Serial.print("get_time : ");
  displayTime2(); // display time
  
  delay(10000);   // every 10 seconds
}

