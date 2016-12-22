/*
GPS-test1

This sketch displays data from a GPS shield on an Arduino Mega board or 
similar board with two hardware serial ports. 

Data is simply read from each serial port and sent to the other port.

The GPS module serial Tx/Rx is connected to Serial1 Rx/Tx pins, and the 
main Serial port is used for the Serial Monitor in the Arduino IDE. 

The NEMA stream from the GPS module is displayed in the Serial Monitor

*/


void setup() {
  // initialize both serial ports:
  Serial1.begin(9600);
  Serial.begin(9600);
  
  while (!Serial1) {
     delay(10);
  };
  Serial.println("Serial port initialised");
}
 
void loop() {
  // read from port 1, send to port 0:
  if (Serial.available()) {
    int inByte = Serial.read();
    Serial1.write(inByte);
  }
 
  // read from port 0, send to port 1:
  if (Serial1.available()) {
    int inByte = Serial1.read();
    Serial.write(inByte);
 }
}

