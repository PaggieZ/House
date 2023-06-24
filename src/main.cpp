#include <Arduino.h> // platform IO
#include "avr8-stub.h" // debug
#include "app_api.h" // only needed with flash breakpoints
#include <CapacitiveSensor.h>

/* The program takes serial input values (1, 2, 3, 4) to control
   brightness of 4 LEDs individually. 
*/

CapacitiveSensor cs_4_2 = CapacitiveSensor(4,2); // 10 megohm resistor between pins 4 & 2, pin 2 is sensor pin, add wire, foil

const uint8_t LED1 = 3; // LED pin number
uint8_t bright1 = 0; // index of brightArr
const uint8_t LED2 = 5;
uint8_t bright2 = 0;
const uint8_t LED3 = 6;
uint8_t bright3 = 0;
const uint8_t LED4 = 9;
uint8_t bright4 = 0;
const uint8_t NUM_OF_BRIGHT_LEVEL = 3;
// PWM prequency for different brightness level
uint8_t brightArr[NUM_OF_BRIGHT_LEVEL] = {0, 20, 255}; 


// writes a message to serial port with a LF terminator
void serial_write(const char* message) {
  Serial.println(message);
  Serial.write(10); // LF terminator (is 10 in ASCII)
}

// returns index of next brightness level
int get_next_bright(int currBright) {
  uint8_t nextBright = 0;
  // adds 1 when not at max index
  if(currBright != (NUM_OF_BRIGHT_LEVEL - 1)) {
    nextBright = currBright + 1;
  }
  return nextBright;
}

// prints current brightness indecies for each LED 
void print_bright_status() {
  Serial.println("!!");
  Serial.print(bright1);
  Serial.print(" ");
  Serial.print(bright2);
  Serial.print(" ");
  Serial.print(bright3);
  Serial.print(" ");
  Serial.print(bright4);
  Serial.print("\n");
}

// updates brightness index for each LED
void update_bright(char receivedData) {
  uint8_t nextBright = 0;
  switch(receivedData) {
    case '1':
      nextBright = get_next_bright(bright1);
      bright1 = nextBright;
      break;
    case '2':
      nextBright = get_next_bright(bright2);
      bright2 = nextBright;
      break;
    case '3':
      nextBright = get_next_bright(bright3);
      bright3 = nextBright;
      break;
    case '4':
      nextBright = get_next_bright(bright4);
      bright4 = nextBright;
      break;
  }
}

// changes the LED brightness
void change_led_bright() {
  print_bright_status();
  analogWrite(LED1, brightArr[bright1]);
  analogWrite(LED2, brightArr[bright2]); 
  analogWrite(LED3, brightArr[bright3]); 
  analogWrite(LED4, brightArr[bright4]);    
}

void setup() {
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT); 
  pinMode(LED3, OUTPUT);  
  pinMode(LED4, OUTPUT); 
  cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on channel 1 - just as an example Serial.begin(9600);
  change_led_bright(); // turns off all LEDs
}

char receivedData = NULL; // holds serial input

void loop() {
  long start = millis();

  long total1 = cs_4_2.capacitiveSensor(30);

  Serial.print(millis() - start); // check on performance in milliseconds

  Serial.print("\t"); // tab character for debug window spacing

  Serial.println(total1); // print sensor output 1

  delay(10); // arbitrary delay to limit data to serial port

  // char receivedData = Serial.read();   // read one byte from serial buffer and save to receivedData
  int nextBright = 0;
  if(total1 > 50000) {
    nextBright = get_next_bright(bright1);
    bright1 = nextBright;
  }
  analogWrite(LED1, brightArr[bright1]);
}

