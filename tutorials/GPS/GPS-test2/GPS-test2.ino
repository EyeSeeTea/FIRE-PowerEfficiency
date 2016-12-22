/*
GPS-test2

This sketch displays data from a GPS shield on an Arduino UNO board or 
similar board with one hardware serial port. 

A second Software Serial port is created on designated Tx/Rx I/O pins.

Data is simply read from each port and sent to the other port

The GPS module serial Tx/Rx is connected to the Software Serial1 Rx/Tx pins,
and the hardware serial port is used for the Serial Monitor in the Arduino IDE. 

The NEMA stream from the GPS module is displayed in the Serial Monitor.

*/

#define  RXPIN  3  // Software serial port pins
#define  TXPIN  4

#include <SoftwareSerial.h>

// Start Software Serial port
SoftwareSerial SoftSerial(RXPIN, TXPIN);  

void setup() {
  // Initialize both serial ports:
  Serial.begin(9600);
  SoftSerial.begin(9600);

  // Wait for serial ports
  while (!SoftSerial) {
     delay(100);
  };
  while (!Serial) {
     delay(100);
  };

  // Check output display
  Serial.println("Serial port initialised");
}
 
void loop() {
  // read from port 1, send to port 0:
  if (SoftSerial.available()) {
    int inByte = SoftSerial.read();
    Serial.write(inByte);
  }
 
  // read from port 0, send to port 1:
  if (Serial.available()) {
    int inByte = Serial.read();
    SoftSerial.write(inByte);
 }
}

