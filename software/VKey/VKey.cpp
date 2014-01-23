//
#include "VKey.h"

const PROGMEM VKey::VKeyScale VKey::scales[MAX] =
{
   // min, step, max
   {26,    58,   718},
   {17,    40,   500}
};


VKey::VKey(uint8_t pin,
         eVref reference)
{
  analog_pin = pin;
  last_key = KEY_NONE;
  
  scale_p = &scales[reference];  
  
  pinMode(pin, INPUT);
}


VKey::eKeynum VKey::voltage_to_key(int v)
{
  VKey::eKeynum k;
  
  uint16_t bottom, stp, top;
  
  bottom = pgm_read_word_near(&scale_p->offset);
  stp = pgm_read_word_near(&scale_p->step_size);
  top = pgm_read_word_near(&scale_p->top);
  
  if( (v < bottom) || (v > top) )
  {
    k = KEY_NONE;
  }
  else
  {
    k = VKey::eKeynum(12 - ((v - bottom)/stp));
  }
  
  return k;
}


bool VKey::CheckKeys(eKeynum & k)
{
  int value;
  
  value = analogRead(analog_pin);

  k = voltage_to_key(value);
  
  if(k != last_key)
  {
    last_key = k; 
    return true;
  }
  
  return false;
}
