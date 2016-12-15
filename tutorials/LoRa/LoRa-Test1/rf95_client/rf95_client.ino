// rf95_client

// Example sketch showing how to create a simple messaging client
// Creative Commons licence. Author T. Gillett
// Based on RadioHead library class RH_RF95 (rf95)
// Ref: http://www.airspayce.com/mikem/arduino/RadioHead/

// The RH_RF95 class does not provide for addressing or reliability, 
// so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.

// This example is designed to run on an Arduino board such at the UNO,
// fitted with a LoRa shield. There is no special configuration required.

// This client device sends requests to a similar LoRa device
// running the rf_server sketch.

// Communication between the Arduino and LoRa chip utilises the SPI bus.

// The serial port outputs messages showing the startup process and 
// the details of individual server responses.

/*******************************/

// Define modem configurations
// Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Default medium range
#define MODE1 rf95.Bw125Cr45Sf128 
// Bw = 500 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Fast+short range
#define MODE2 rf95.Bw500Cr45Sf128); 
// Bw = 31.25 kHz, Cr = 4/8, Sf = 512chips/symbol, CRC on. Slow+long range
#define MODE3 (rf95.Bw31_25Cr48Sf512);

#define MODE       MODE1   // Set modem configuration.
#define FREQUENCY  915.0   // Frequency in MHz
#define TXPOWER    23      // Set to 5 - 23 dBm

#include <SPI.h>
#include <RH_RF95.h>

// Create a singleton instance of the radio driver
RH_RF95 rf95;

void setup() 
{
  Serial.begin(9600);
  while (!Serial) ;            // Wait for serial port to be available
  Serial.println("Serial available.\n");
  
  Serial.println("Start Client setup.");
  
  if (!rf95.init())            // Initialise the LoRa chip
  {
    Serial.println("Init rf95 failed");
  }
  else
  {
    Serial.println("Init rf95 success");
    Serial.println("Set Freq, TxPower and Mode");
    rf95.setFrequency(FREQUENCY);          
    rf95.setTxPower(TXPOWER, false);        
		rf95.setModemConfig(MODE);
    
    //rf95.printRegisters(); // Print registers if reqd.
    
    Serial.println("End setup.\nStart loop\n");
  }
}

void loop()
{
  Serial.println("\nSend request to rf95_server");

  uint8_t data[] = "Request from Client";   // Send request string to rf95_server
  rf95.send(data, sizeof(data));
  rf95.waitPacketSent();                  

  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  if (rf95.waitAvailableTimeout(3000))      // Now wait for a reply
  { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len))
   {
      Serial.print("Got reply: ");
      Serial.println((char*)buf);           // Output the reply string
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC); // Output the signal strength
    }
    else
    {
      Serial.println("Recv failed");
    }
  }
  else
  {
    Serial.println("No reply, is rf95_server running?");
  }
  delay(2000);   // Wait before sending next request
}


