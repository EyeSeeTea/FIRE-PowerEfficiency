// LoRa-Test2_server

// Example sketch showing how to create a simple messaging server
// Creative Commons licence. Author T. Gillett

// Based on RadioHead library class RH_RF95 (rf95)
// Ref: http://www.airspayce.com/mikem/arduino/RadioHead/

// The RH_RF95 class does not provide for addressing or reliability, 
// so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.

// This example is designed to run on an Arduino board such at the UNO,
// fitted with a LoRa shield such as the Dragino devices:
//    Ref: http://www.dragino.com/products/lora/item/102-lora-shield.html
//         http://www.dragino.com/products/lora/item/108-lora-gps-shield.html

//There is no special configuration required.
// An LED may be fitted to the nominated I/O pin to indicate operation.

// This server device will respond to requests from similar LoRa 
// devices running the rf_client sketch.

// Communication between the Arduino and LoRa chip utilises the SPI bus.

// The serial port outputs messages showing the startup process and 
// the details of individual client requests. The LED will be turned ON
// when a request is received and OFF when the response has been sent.

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

#define LEDPIN 7 // Connect the LED to this I/O pin with a resistor to Gnd.

#include <SPI.h> 
#include <RH_RF95.h>

// Create a singleton instance of the radio driver
RH_RF95 rf95;

int led = LEDPIN; // Connect the LED to this I/O pin with a resistor to Gnd.

void setup() 
{
  pinMode(led, OUTPUT);     
  Serial.begin(9600);

  // Wait for serial port
  while (!Serial) {
     delay(100);
  }

  Serial.println("Serial available.\n");
  
  Serial.println("Start Server setup.");
  
  if (!rf95.init())           // Initialise the LoRa chip
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
    
    //rf95.printRegisters();
    
    Serial.println("End setup.\nStart loop\n");
  }
}

void loop()
{
  if (rf95.available())                     // Wait for a request
  {
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len))
    {
      digitalWrite(led, HIGH);          // Turn LED on

//    RH_RF95::printBuffer("Request: ", buf, len);  // Print rx buffer

      Serial.print("\nGot request: ");
      Serial.println((char*)buf);      // Output the request message string
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC); // Output the signal strength
      
      // Send a reply
      uint8_t data[] = "Response from server"; // Send this response string
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      Serial.println("Sent a reply");
      delay(200);                      // Keep the LED on for a period
      digitalWrite(led, LOW);          //Turn LED off
    }
    else
    {
      Serial.println("Recv failed");
    }
  }
}


