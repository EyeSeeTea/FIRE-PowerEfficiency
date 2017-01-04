// EEPROM-Test1
// Example sketch to demontrate reading and writing to the non-volatile memory 
// on DS3231 RTC Module.

// Creative Commons BY SA licence. Author T. Gillett
// Adapted from:  http://www.hobbytronics.co.uk/arduino-external-eeprom

// This example is designed to run on an Arduino board such as the UNO or MEGA
// with a DS3231 RTC module connected using the I2C bus.
// Connect the module to GND, 3.3V (VCC), SDA, SCL pins on the Arduino board.

// The serial port outputs messages to the Serial Monitor showing the operation.


#include <Wire.h>     // SPI bus library
 
#define eeprom 0x57   //Address of AT24C32 eeprom chip

void setup(void)
{
  Serial.begin(9600);
  Serial.println("EEPROM-Test1\n");
  Serial.println("Start setup\n");

  Wire.begin();  
 
  //unsigned int address = 65535; // Address to write/read: 0 to 65535
  unsigned int address = 1127; // Address to write/read: 0 to 65535
  
  //unsigned int data = 255; // Data to write/read: 0 to 255
  unsigned int data = 86; // Data to write/read: 0 to 255
 
  Serial.print("EEPROM Address: ");
  Serial.println(address);

  Serial.print("Read EEPROM   Data: ");  // Read and display the original data 
  Serial.println(readEEPROM(eeprom, address), DEC);
  
  Serial.print("Write EEPROM  Data: ");  // Write the new data
  Serial.println(data);
  writeEEPROM(eeprom, address, data);

  Serial.print("Read EEPROM:  Data: ");  // Read the new data
  Serial.println(readEEPROM(eeprom, address), DEC);


}
 
void loop(){}


// --------------------------------------------------------------------------------
 
void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data ) 
{
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // Send the address MSB 
  Wire.write((int)(eeaddress & 0xFF)); // Send the address LSB
  Wire.write(data);                    // Send the data byte
  Wire.endTransmission();
 
  delay(5);
}
 
byte readEEPROM(int deviceaddress, unsigned int eeaddress ) 
{
  byte rdata = 0xFF; // Initialise to all ones
 
  Wire.beginTransmission(deviceaddress); // Select the EEPROM device
  Wire.write((int)(eeaddress >> 8));     // Send the address MSB
  Wire.write((int)(eeaddress & 0xFF));   // Send the address LSB
  Wire.endTransmission();
 
  Wire.requestFrom(deviceaddress,1);     // Request 1 byte from the EEPROM device
  if (Wire.available()) rdata = Wire.read(); // Read the data byte
 
  return rdata;
}
