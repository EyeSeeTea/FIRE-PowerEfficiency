/*
Test program for BLE Module
Adapted from Apploader project http://www.apploader.info

USB(mega2560)-to-BLE sketch
Modified to work with "bolutek" BT05 module - added CR/LF to AT commands

BLE is connected to Serial-1 (Tx1, Rx1) port of Arduino Mega board.
The default Serial port is used for terminal display.

Note:
  HM-10 BLE module applies baud rate set in 'AT+BAUD' after power cycle.
*/

int led = 13;
int ledHIGH = 0;

long bauds[] = {
    // major 
    9600, 57600, 115200,
    
    // others
    19200, 38400, 4800, 2400, 1200, 230400
  };

bool detectBleBaudRate() {
  Serial.println("Detecting BLE baud rate:");
  for (int i=0; i<(sizeof(bauds)/sizeof(long)); i++) {
    Serial.write("Checking ");
    long cur_baud = bauds[i];
    Serial.println(cur_baud, DEC);
    
    Serial1.begin(cur_baud);
/*    Serial1.write("AT"); */
    Serial1.write("AT\r\n"); /*Added \r\n for bolutek module */
    Serial1.flush();

    delay(50);
    String response = Serial1.readString();

    Serial.write("Response: ");
    Serial.println(response);
    
/*    if (response == "OK") {     */
    if (response == "OK\r\n") {    /*Added \r\n for bolutek module */
      Serial.println("Detected");
      return true;
    } else {
      Serial1.end();
    }
  }
  return false;
}

void setup() {
  // init
  Serial.begin(9600);  // USB (choose 9600 in terminal)
  
  if (detectBleBaudRate()) {
    Serial.write("AT Command Set \n\n");
    Serial1.write("AT+help\r\n"); /*Added \r\n for bolutek module */
    String str = Serial1.readString();
    Serial.print(str);
    Serial.write('\n');
    
    Serial.write("Ready, type AT commands\n\n");
  }
  else
    Serial.write("Not ready. Halt");
  
  pinMode(led, OUTPUT);
}

void loop() {
  // read from BLE Module
  if (Serial1.available()) {
    Serial.write("ble: ");
    String str = Serial1.readString();
    Serial.print(str);
    Serial.write('\n');
  }

  // read from USB (Arduino Terminal)
  if (Serial.available()) {
    Serial.write("usb: ");
    String str = Serial.readString(); // read from terminal
    Serial1.print(str);               // copy to BLE
    Serial.print(str);
    Serial.write('\n');
  }
}
