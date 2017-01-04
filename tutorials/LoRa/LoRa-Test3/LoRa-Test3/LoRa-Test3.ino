// LoRa-Test3 Node
/*
 * This sketch demonstrates a message propogating node.
 * Creative Commons BY SA licence. Author T. Gillett
 * 
 * Two buttons are used as inputs - Power-Up and Power_Down.
 * If one of these buttons is operated, the Power output is set to the required state 
 * and a message is transmitted requesting power up or down on other nodes.
 * 
 * If a node receives a power up or down request, it will forward the message on and 
 * set the local power state.
 * 
 * There is a dead time (WAITPERIOD) after each transmission to allow the message to 
 * propogate without being received by the node that sent it.
 * 
 * LEDPIN is used to indicate a received message, connect LED and resistor to Gnd
 * POWERPIN is used to set or indicate local power state, connect LED and resistor to Gnd
 * PWRONPIN is connected via a button to GND, requests power up operation
 * PWROFFPIN is connected via a button to GND, requests power down operation
 * 
 * This example is designed to run on an Arduino board such at the UNO, fitted 
 * with a LoRa/GPS shield such as the Dragino device:    
 * Ref: http://www.dragino.com/products/lora/item/108-lora-gps-shield.html
 * 
 */

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
#define TXPOWER    6      // Set to 5 - 23 dBm

#define LEDPIN 7     // Connect the LED to this I/O pin with a resistor to Gnd.
#define POWERPIN 8   // This pin controls power to load
#define PWRONPIN A4   // Pull LOW to send PowerOn request
#define PWROFFPIN A5 // Pull LOW to send PowerOff request

#define WAITPERIOD 5000  // Dead time after sending message

#include <SPI.h> 
#include <RH_RF95.h>

// Create an instance of the radio driver
RH_RF95 rf95;

int led = LEDPIN;
int pwr = POWERPIN; 
int pon = PWRONPIN; 
int poff = PWROFFPIN; 
bool power_flag = false; // Flag for power to main board
bool power_req = false;  // Local Power Required 
long randNumber;
char messg_str[10];


void send_message()
{
  uint8_t data[10];

  strcpy((char*)data, messg_str);

  Serial.print("Send data:  ");
  rf95.send(data, sizeof(data));
  Serial.println((char*)data);
  rf95.waitPacketSent();

  delay(WAITPERIOD);  // Wait for the dead time period

  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  // Flush rx buffer
  if (rf95.available())
    {
      rf95.recv(buf, &len);  // Flush rx buffer
    }
  Serial.println("Finished send and wait period\n");
}


void setup() 
{
  pinMode(led, OUTPUT);     
  pinMode(pwr, OUTPUT);     
  pinMode(pon, INPUT_PULLUP);     
  pinMode(poff, INPUT_PULLUP);     

  Serial.begin(9600);

  while (!Serial) ;            // Wait for serial port to be available
  Serial.println("LoRa-Test3 Node.\n");
  
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
    
    Serial.println("End setup. Starting loop\n");
    Serial.print("Pwr Flag: ");
    Serial.println(power_flag);
    Serial.println("");
  }
  randomSeed(analogRead(0));
}

void loop()
{
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);


  // Check buttons
  int pon_req = digitalRead(pon);
  int poff_req = digitalRead(poff);

  if (!pon_req && poff_req)  // Power up request
    {
      strcpy(messg_str,"PReq_0");
      power_flag = true;
      digitalWrite(pwr, HIGH);
      Serial.println("Pwr ON Btn");
      Serial.print("Pwr Flag: ");
      Serial.println(power_flag);
      send_message();
    }
  
  if (pon_req && !poff_req)  // Power down request
    {
      strcpy(messg_str,"PDwn_0");
      power_flag = false;
      digitalWrite(pwr, LOW);
      Serial.println("Pwr OFF Btn");
      Serial.print("Pwr Flag: ");
      Serial.println(power_flag);
      send_message();
    }

 delay(100);


// Handle incoming messagees
  if (rf95.available())          
  {    
    if (rf95.recv(buf, &len))    // Get the message
    {
      digitalWrite(led, HIGH);   // Turn LED on

      //RH_RF95::printBuffer("Request: ", buf, len);  // Print rx buffer for debug

      Serial.print("\nRecd: ");
      Serial.print((char*)buf); // Output the request message string
      Serial.print("   RSSI: ");
      Serial.println(rf95.lastRssi(), DEC); // Output the signal strength
     
      delay(200);              // Keep the LED on for a period
      digitalWrite(led, LOW);  //Turn LED off
    }
    else
    {
      Serial.println("Recv failed");
    }

    //Serial.print("\nGot request: ");
    //Serial.println((char*)buf); // Output the request message string

    randNumber = random(10, 200);
    randNumber = 10 * randNumber;
    char str1[10];

    strcpy(str1,(char*)buf);
 
    if (((String)str1 == "PReq_0") || ((String)str1 == "PReq_1"))
    {
      Serial.print("Rand: ");
      Serial.println(randNumber);
      delay(randNumber);

      strcpy(messg_str,"PReq_1");
      send_message();

      digitalWrite(pwr, HIGH);
      power_flag = true;       // Turn on local power
      Serial.print("Pwr Flag: ");
      Serial.println(power_flag);
    }

    if (((String)str1 == "PDwn_0") || ((String)str1 == "PDwn_1"))
    {
      Serial.print("Rand: ");
      Serial.println(randNumber);
      delay(randNumber);

      strcpy(messg_str,"PDwn_1");
      send_message();

      digitalWrite(pwr, LOW);
      power_flag = false;       // Turn off local power
      Serial.print("Pwr Flag: ");
      Serial.println(power_flag);      
    }
  }

}  // loop


