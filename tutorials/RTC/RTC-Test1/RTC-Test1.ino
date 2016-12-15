// RTC-Test1.ino

// Example sketch to set time/date/alarm on DS3231 RTC Module
// Creative Commons licence. Author T. Gillett
// Adapted from Tronix Labs  http://tronixstuff.com/

// This example is designed to run on an Arduino board such as the UNO or MEGA
// with a DS3231 RTC module connected using the I2C bus.
// Connect the module to GND, 3.3V (VCC), SDA, SCL pins on the Arduino board.

// The serial port outputs messages to the Serial Monitor showing the operation.

/*******************************/

// You may set the data on the RTC chip, or just to display the data. 
// Set the values below to true or flase as required.
#define SETTDD    false	// Set Time, Date and Day of Week
#define SETTIME   false	// Set Time
#define SETDATE   false	// Set Date
#define SETALRM1  true	// Set Alarm1
#define SETALRM2  true	// Set Alarm2

// Set the required time/date and daily alarm times to be set in the RTC.
#define HH  05	// Hours
#define MM  52	// Minutes
#define SS  02	// Seconds
#define DD  16 	// Date
#define MO  12	// Month
#define YY  16	// Year (20yy)
#define DAY Fr	// Day of Week 

#define AL1_HH 12 // Alarm1 Hours
#define AL1_MM 05 // Alarm1 Minutes
#define AL1_SS 00 // Alarm1 Seconds
#define AL2_HH 13 // Alarm2 Hours
#define AL2_MM 05 // Alarm2 Minutes

// Define days of the week
#define Su 01
#define Mo 02
#define Tu 03
#define We 04
#define Th 05
#define Fr 06
#define Sa 07

/*******************************/

#define DS3231_I2C_ADDRESS 0x68 // I2C address of RTC chip

#include "Wire.h"								// I2C library

#include "functions.h"


void setup()
{
  Wire.begin();        // Initialise I2C
  Serial.begin(9600);  // Initialise Serial Port

  /* Display the current data */
  Serial.print("RTC data : ");
  displayTime();  // display the real-time clock data on the Serial Monitor
  Serial.print("RTC Alarm1 : ");
  display_alarm1(); // Display time
  Serial.print("\RTC Alarm2 : ");
  display_alarm2(); // Display time
  Serial.println("\n---------------");

  /* Set time, day, date */
  if(SETTDD)
  {
    Serial.println("Set Time, Day, Date ");  
    setDS3231time (SS, MM, HH, DAY, DD, MO, YY);
  }

  /* Set time */
  if(SETTIME)
  {
    Serial.println("Set Time");  
    set_time(HH, MM, SS);
  }

  /* Set date */
  if(SETDATE)
  {
    Serial.println("Set Date");  
    set_date(YY, MO, DD);
  }

  /* Set alarm1 */
  if(SETALRM1)
  {
    Serial.println("Set Alarm1 time");  
    set_daily_alarm1(AL1_HH, AL1_MM, AL1_SS);
  }

  /* Set alarm2 */
  if(SETALRM2)
  {
    Serial.println("Set Alarm2 time");  
    set_daily_alarm2(AL2_HH, AL2_MM);
  }

  /* Display the time and date */
  Serial.println("---------------");
  
  Serial.print("RTC data : ");
  displayTime();  // display the real-time clock data on the Serial Monitor
  
  Serial.print("RTC Date : ");
  display_date();  // Display date
  
  Serial.print("RTC Time : ");
  display_time(); // Display time

  Serial.print("\nRTC Alarm1 : ");
  display_alarm1(); // Display time

  Serial.print("\RTC Alarm2 : ");
  display_alarm2(); // Display time

	Serial.println("\n\nSetup complete");
}

void loop()
{
	while(1); // Wait forever
}

