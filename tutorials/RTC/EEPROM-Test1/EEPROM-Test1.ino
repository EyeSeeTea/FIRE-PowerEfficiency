// Adapted from:  http://www.hobbytronics.co.uk/arduino-external-eeprom
// This sketch demonstrates wrting and reading data to a location in the EEPROM

#include <Wire.h>    
 
#define eeprom 0x57    //Address of AT24C32 eeprom chip

void setup(void)
{
  Serial.begin(9600);
  Serial.println("Start setup\n");

  Wire.begin();  
 
  //unsigned int address = 65535; // Address to write/read: 0 to 65535
  unsigned int address = 1127; // Address to write/read: 0 to 65535
  
  unsigned int data = 86; // Data to write/read: 0 to 255
  //unsigned int data = 176; // Data to write/read: 0 to 255
 
  Serial.print("EEPROM Address: ");
  Serial.println(address);

  Serial.print("Read EEPROM   Data: ");
  Serial.println(readEEPROM(eeprom, address), DEC);
  
  Serial.print("Write EEPROM  Data: ");
  Serial.println(data);
  writeEEPROM(eeprom, address, data);

  Serial.print("Read EEPROM:  Data: ");
  Serial.println(readEEPROM(eeprom, address), DEC);


}
 
void loop(){}


// --------------------------------------------------------------------------------
 
void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data ) 
{
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(data);
  Wire.endTransmission();
 
  delay(5);
}
 
byte readEEPROM(int deviceaddress, unsigned int eeaddress ) 
{
  byte rdata = 0xFF; // initialise to all ones
 
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
 
  Wire.requestFrom(deviceaddress,1);
 
  if (Wire.available()) rdata = Wire.read();
 
  return rdata;
}
