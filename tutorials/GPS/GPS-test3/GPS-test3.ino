/*
GPS-test3

Creative Commons licence. Author T. Gillett

 This sketch shows the use of the TinyGPS library.
 Ref: http://http://arduiniana.org/libraries/tinygps/

This example is designed to run on an Arduino board such as the UNO,
fitted with a GPS shield.

Communication with the GPS module uses a software serial port.
The GPS serial data Tx/Rx lines need to be connected to the designated 
I/O pins used for the Software Serial port.

The primary serial port outputs messages showing the startup process and 
the details of individual server responses. These may be viewed using the
Serial Monitor facility in the Arduino IDE.

*/


#define SSRX  3  // Software serial port pins
#define SSTX  4

#include <SoftwareSerial.h>
#include <TinyGPS.h>


TinyGPS gps;    // Create instance of TinyGPS
SoftwareSerial ss(SSRX, SSTX); // Create software serial port


void setup()
{
  Serial.begin(9600);
  ss.begin(9600);

  // Wait for serial port
  while (!ss) {
     delay(100);
  }
  while (!Serial) {
     delay(100);
  }

  Serial.println(F("\nGPS-test3\n")); // Print the program id
  Serial.print(F("TinyGPS library v. ")); Serial.println(TinyGPS::library_version());
  Serial.println();    
  Serial.println(F("End setup. Starting loop\n"));
}


void loop()
{ 
  float flat, flon;
  unsigned long age;
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;


  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      
      //Serial.write(c);  // Uncomment this line if you want to see the GPS data flowing
      
      if (gps.encode(c))  // Check if a new valid NEMA sentence has come in
      {
        //Serial.println(F("New GPS data available"));  // Uncomment to indicate new data
        newData = true;
      }
    }
  }

  //Get the GPS data
  if (newData)
  {   
    gps.f_get_position(&flat, &flon, &age);
    
    Serial.print(F("\nData:  LAT="));
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    
    Serial.print(F("  LON="));
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);

    Serial.print(F("  SPEED="));
    Serial.print(gps.f_speed_mps() == TinyGPS::GPS_INVALID_ANGLE ? 0.0 : gps.f_speed_mps(), 4);
    
    Serial.print(F("   AGE= "));
    Serial.print(age == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : age, 0);
    
    Serial.print(F("  SATS="));
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    
    Serial.print(F("  PREC="));
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
    Serial.println(F("\n"));
   
    delay(2000);    // Wait before next data read
 }
 
  // Uncomment following to see stats 
  /*
  gps.stats(&chars, &sentences, &failed);
  Serial.print(F(" CHARS="));
  Serial.print(chars);
  Serial.print(F(" SENTENCES="));
  Serial.print(sentences);
  Serial.print(F(" CSUM ERR="));
  Serial.println(failed);
  if (chars == 0)
  Serial.println(F("** No characters received from GPS: check wiring **"));
  */
}



