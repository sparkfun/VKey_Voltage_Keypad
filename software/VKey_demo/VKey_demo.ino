/*
 12-10-2015
 Spark Fun Electronics
 Byron Jacquot
 
 This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 
 This demonstrates decoding the input from the VKey voltage keypad.
*/

#include <Arduino.h>
#include <VKey.h>


VKey keypad(1, VKey::THREE );

void setup()
{
  Serial.begin(9600);
  Serial.println("VKey read example");
}

void loop() 
{
  VKey::keynum k;

  if(keypad.CheckKeys(k))
  {
    // CheckKeys will always return the current key in k.
    // The boolean return value indicates whether that value is different than the
    // previous value.
    
    // Only print when key has changed
    Serial.print("Got key: ");
    Serial.println(k);
  }

  delay(50);
}

