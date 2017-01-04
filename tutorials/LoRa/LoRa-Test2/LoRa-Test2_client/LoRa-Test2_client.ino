/*
LoRa-Test2_client

 This sketch shows a simple messaging client sending data from a GPS module
 to the server device.

Creative Commons BY SA licence. Author T. Gillett

Based on the following libraries:
RadioHead library class RH_RF95 (rf95)
    Ref: http://www.airspayce.com/mikem/arduino/RadioHead/

TinyGPS library
    Ref: http://http://arduiniana.org/libraries/tinygps/

This example is designed to run on an Arduino board such at the UNO,
fitted with a LoRa/GPS shield such as the Dragino device:
    Ref: http://www.dragino.com/products/lora/item/108-lora-gps-shield.html

Communication between the Arduino and LoRa chip utilises the SPI bus.

Communication with the GPS module uses a software serial port.
The GPS serial data Tx/Rx lines need to be connected to the designated 
I/O pins used for the Software Serial port.

This client device sends requests to a similar LoRa device running 
corresponding server sketch.

The primary serial port outputs messages showing the startup process and 
the details of individual server responses. These may be viewed using the
Serial Monitor facility in the Arduino IDE.

*/


// Define LoRa radio and modem configurations
// Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Default medium range
#define MODE1 rf95.Bw125Cr45Sf128 
// Bw = 500 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Fast+short range
#define MODE2 rf95.Bw500Cr45Sf128
// Bw = 31.25 kHz, Cr = 4/8, Sf = 512chips/symbol, CRC on. Slow+long range
#define MODE3 rf95.Bw31_25Cr48Sf512

#define MODE       MODE1   // Set modem configuration.
#define FREQUENCY  915.0   // Frequency in MHz
#define TXPOWER    23      // Set to 5 - 23 dBm

#define SSRX  3  // Software serial port pins
#define SSTX  4

#include <SoftwareSerial.h>
#include <TinyGPS.h>

#include <SPI.h>
#include <RH_RF95.h>


RH_RF95 rf95;   // Create instance of the radio driver
TinyGPS gps;    // Create instance of TinyGPS
SoftwareSerial ss(SSRX, SSTX); // Create software serial port


void setup()
{
  Serial.begin(9600);
  ss.begin(9600);

  // Wait for serial ports
  while (!ss) {
     delay(100);
  }
  while (!Serial) {
     delay(100);
  }

  Serial.println(F("\nLoRa-test2_client\n")); // Print the program id

  if (!rf95.init())            // Initialise the LoRa chip
  {
    Serial.println(F("Init rf95 failed"));
  }
  else
  {
    Serial.println(F("Init rf95 success"));
    Serial.println(F("Set Freq, TxPower and Mode"));
    rf95.setFrequency(FREQUENCY);          
    rf95.setTxPower(TXPOWER, false);        
    rf95.setModemConfig(MODE);

    // Print registers if reqd.
    //Serial.println(F("\nPrint registers"));
    //rf95.printRegisters();
    //Serial.println();
    
    Serial.print(F("TinyGPS library v. ")); Serial.println(TinyGPS::library_version());
    Serial.println();
    
    Serial.println(F("End setup. Starting loop\n"));
  }
}


void loop()
{ 

  char databuf[100];
  uint8_t dataoutgoing[100];
  float flat, flon;
  unsigned long age;

  char gps_lon[12]={"\0"};  
  char gps_lat[12]={"\0"}; 
  char gps_str[100]{"\0"};
  char str[5];
  int  num;
 
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;


  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      //Serial.println(F("Serial available"));
      char c = ss.read();
      
      //Serial.write(c);  // Uncomment this line if you want to see the GPS data flowing
      
      if (gps.encode(c))  // Check if a new valid NEMA sentence has come in
      {
        Serial.println(F("New GPS data available"));  // Uncomment to indicate new data
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

    // Assemble payload message to send to server
    flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6;          
    flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6; 
    dtostrf(flat, 0, 6, gps_lat); 
    dtostrf(flon, 0, 6, gps_lon);

    strcat(gps_str,"Lat:");
    strcat(gps_str, gps_lat);
    strcat(gps_str," Lon:");
    strcat(gps_str,gps_lon);
    
    itoa(gps.satellites(), str, 10);
    strcat(gps_str," Sats:");
    strcat(gps_str,str);
    
    itoa(gps.hdop(), str, 10);
    strcat(gps_str," Prec:");
    strcat(gps_str,str);
    
    Serial.print(F("Sending to rf95_server:  "));
    Serial.println(gps_str);
    Serial.println("");
   
    // Send the data to server
    strcpy((char*)dataoutgoing,gps_str); 
    rf95.send(dataoutgoing, sizeof(dataoutgoing));

    // Now wait for a reply
    uint8_t indatabuf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(indatabuf);

    if (rf95.waitAvailableTimeout(3000))
     { 
       // Should be a reply message for us now   
       if (rf95.recv(indatabuf, &len))
      {
         // Serial print "got reply:" and the reply message from the server
         Serial.print(F("Got reply: "));
         Serial.println((char*)indatabuf);
         Serial.print(F("RSSI: "));
         Serial.println(rf95.lastRssi(), DEC); // Output the signal strength
      }
      else
      {
      Serial.println(F("Recv failed"));
      }
    }
    else
    {
      Serial.println(F("No reply, is rf95_server running?")); // If don't get the reply .
    }
    
  delay(1000);    // Wait before next transmission
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



