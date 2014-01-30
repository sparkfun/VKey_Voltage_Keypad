/**
 * @file	VKey.h
 * @brief 	Library for the SparkFun VKey analog voltage keypad
 * @author	Byron Jacquot(SparkFun Electronics)
 * 
 * @copyright	This code is public domain but you buy me a beer if you use
 * this and we meet someday (Beerware license).
 * 
 * This library interfaces with the VKey analog keypad.  It relies on the 
 * Arduino analogRead functionality.
 *
 * The keypad has 12 keys, and outputs a unique Voltage for each key.  It allows
 * an application to interface with up to 12 switches using only one ADC line.
 *
 * To use the library, instantiate a VKey object, then call checkKeys() to
 * read the keypad.
 */
#include "VKey.h"

/* Calibration data for each supply/reference voltage.
 * Unchanging, so declared as progmem.
 * 
 * See theory/ADC.ods for the derivation of these values, or
 * use that sheet to calculate for other voltages.
 */
const PROGMEM VKey::VKeyScale VKey::scales[MAX] =
{
   // min, step, max
   {26,    58,   721},
   {17,    40,   496}
};

/* Constructor.
 * Parameters specify details about the class:
 *    -pin = ADC pin number that the VKey output is attached to
 *    -reference = enumerated indication of supply & reference voltage
 */
VKey::VKey(uint8_t pin,
         eVref reference)
{
  // Store parameters as member data
  analog_pin = pin;
  scale_p    = &scales[reference];  
  
  // Initialize internal variables
  last_key   = KEY_NONE;
  
  // Configure related hardware
  pinMode(pin, INPUT);
}

/* checkKeys()
 * Sample the ADC.
 * Convert the sample into a key number.
 *
 * If the key is different than the last key seen, 
 * return true to indicate the change, otherwise return false.
 *
 * Always returns the current value in reference parameter k.
 *
 * This allows clients to query the present value,
 * or ignore the value when it is unchanging.
 */
bool VKey::checkKeys(eKeynum & k)
{
  int value;
  
  // Read the input voltage
  value = analogRead(analog_pin);

  // convert voltage to a key number
  k = voltageToKey(value);
  
  // Check to see if current key number is different than last seen 
  if(k != last_key)
  {
    // Update value in last_key
    last_key = k; 
    return true;
  }
  
  return false;
}

/* voltageToKey()
 * Private, internal conversion routine.
 * Given a raw value from the ADC, apply the data from the scale table, 
 * and calculate which key is pressed.
 */
VKey::eKeynum VKey::voltageToKey(int v)
{
  /* See theory/ADC.ods for the nitty-gritty.
   */

  VKey::eKeynum k;
  
  uint16_t bottom, stp, top;
  
  // Read the constants from program memory
  bottom = pgm_read_word_near(&scale_p->offset);
  stp = pgm_read_word_near(&scale_p->step_size);
  top = pgm_read_word_near(&scale_p->top);
  
  if( (v < bottom) || (v > top) )
  {
    // Value is in invalid range - equation doesn't apply
    k = KEY_NONE;
  }
  else
  {
    // Apply the calculation based on the configured constants.
    k = VKey::eKeynum(12 - ((v - bottom)/stp));
  }
  
  return k;
}
