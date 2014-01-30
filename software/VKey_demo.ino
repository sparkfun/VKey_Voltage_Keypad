/*
 1-29-2014
 Spark Fun Electronics
 Byron Jacquot
 
 This code is public domain but you buy me a beer if you use this and we meet 
 someday (Beerware license).
 
 This demonstrates interfacing the VKey voltage keypad using the VKey library.
 
 The VKey has an array of 12 pushbuttons, each producing a unique analog voltage 
 when  pushed.  A microcontroller can read the voltage, and determine which 
 key has been pressed.  The VKey library keeps track of the analog pin 
 connection, key status, and voltage calculation details.
 
 To use the library, instantiate a VKey object and periodically check for input 
 using the checkKeys() function.
*/

#include <Arduino.h>
#include <VKey.h>

// Global declaration of the VKey class
// Initialized with analog pin number and supply voltage
VKey keypad(1, VKey::FIVE );

void setup()
{
  
  Serial.begin(9600);
  Serial.println("Welcome to VKey example");
  
  // No VKey specific initialization required.
}

void loop() 
{
  VKey::eKeynum k;  // Variable to receive the 

  if(keypad.checkKeys(k))
  {
    // CheckKeys will always return the current key in k.
    // The boolean return value indicates whether that value is different than
    // the previous value.
    
    // Only print when value has changed
    Serial.print("Got key: ");
    Serial.println(k);
  }

  // The responsiveness of the keypad depends on how frequently it is checked. 
  // 50 milliseconds seems to be a reasonable poll interval.
  delay(50);
}

