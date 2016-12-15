// Test program for DS3231 RTC Module
// Adapted from Tronix Labs  http://tronixstuff.com/
// Connect module to Gnd, 3.3V, SDA, SCL pins on Arduino board

#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68


// Data conversion utility routines
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}


// RTC Access functions

void set_time(byte hour, byte minute, byte second)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(00); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds (0 to 59)
  Wire.write(decToBcd(minute)); // set minutes (0 to 59)
  Wire.write(decToBcd(hour));   // set hours   (0 to 24)
  Wire.endTransmission();
}

void set_date(byte year, byte month, byte day)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(04); // set next input to start at the date register
  Wire.write(decToBcd(day));   // set date  (1 to 31)
  Wire.write(decToBcd(month)); // set month (1 to 12)
  Wire.write(decToBcd(year));  // set year  (0 to 99)
  Wire.endTransmission();
}

void set_daily_alarm1(byte hour, byte minute, byte second)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(07); // set next input to start at the Alarm1 seconds register
  Wire.write(decToBcd(second)); // set seconds (0 to 59)
  Wire.write(decToBcd(minute)); // set minutes (0 to 59)
  Wire.write(decToBcd(hour));   // set hours   (0 to 24)
  Wire.write(128);              // set daily operation
  Wire.endTransmission();
}

void set_daily_alarm2(byte hour, byte minute)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(11); // set next input to start at the Alarm2 seconds register
  Wire.write(decToBcd(minute)); // set minutes (0 to 59)
  Wire.write(decToBcd(hour));   // set hours   (0 to 24)
  Wire.write(0);   // dummy day/date field)
  Wire.write(128); // set daily operation
  Wire.endTransmission();
}


void setDS3231time(byte second, 
byte minute, 
byte hour, 
byte dayOfWeek, 
byte
dayOfMonth, 
byte month, 
byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}

void get_time(byte *second,
byte *minute,
byte *hour)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 3);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
}

void get_date(byte *dayOfMonth,
byte *month,
byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(4); // set DS3231 register pointer to 04h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 3);
  // request seven bytes of data from DS3231 starting from register 00h
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}


void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  
  // send it to the serial monitor
  Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute<10)
  {
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second<10)
  {
    Serial.print("0");
  }
  Serial.print(second, DEC);
  Serial.print(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/20");
  Serial.print(year, DEC);
  Serial.print(" Day of week: ");
  switch(dayOfWeek){
  case 1:
    Serial.println("Sunday");
    break;
  case 2:
    Serial.println("Monday");
    break;
  case 3:
    Serial.println("Tuesday");
    break;
  case 4:
    Serial.println("Wednesday");
    break;
  case 5:
    Serial.println("Thursday");
    break;
  case 6:
    Serial.println("Friday");
    break;
  case 7:
    Serial.println("Saturday");
    break;
  }
}


void displayDate()
{
  byte dayOfMonth, month, year;
  // retrieve data from DS3231
  get_date(&dayOfMonth, &month, &year);
  
  // send it to the serial monitor
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.println(year, DEC);
}

void displayTime2()
{
  byte hour, minute, second;
  // retrieve data from DS3231
  get_time(&second, &minute, &hour);
  
  // send it to the serial monitor
  Serial.print(hour, DEC);
  Serial.print(":");
  Serial.print(minute, DEC);
  Serial.print(":");
  Serial.println(second, DEC);
}


void setup()
{
  Wire.begin();
  Serial.begin(9600);
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day-of-week, date, month, year
  Serial.println("Set time, date ");
   setDS3231time(02,01,12,1,27,11,16);  // Uncomment and run once to initialise the time.

  // set_time(12, 01, 02); // hh, mm, ss Set time to 12:01:02
  // set_date(16, 01, 20); // yy, mm, dd Set date to 2016/01/20

	// set_daily_alarm1(13, 01); //Set time to 13:01:02
  // set_daily_alarm2(13, 01); //Set time to 13:01
}

void loop()
{
  Serial.print("RTC data : ");
  displayTime();  // display the real-time clock data on the Serial Monitor
  
  Serial.print("get_date : ");
  displayDate();  // display date
  
  Serial.print("get_time : ");
  displayTime2(); // display time
  
  delay(10000);   // every 10 seconds
}

