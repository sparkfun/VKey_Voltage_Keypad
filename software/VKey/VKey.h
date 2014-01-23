/*
FNORD
*/
#ifndef __VKEY_H__
#define __VKEY_H__

#include <Arduino.h>

class VKey
{
  public:
  
    enum eVref
    {
      THREE, // 3.3V power supply and Vref
      FIVE,  // 5V  power supply and Vref
      MAX    // used for range checking only
    };
    
    enum eKeynum
    {
      // Keys map directly to regular integers
      KEY_NONE = 0,
      KEY_1,
      KEY_2,
      KEY_3,
      KEY_4,
      KEY_5,
      KEY_6,
      KEY_7,
      KEY_8,
      KEY_9,
      KEY_10,
      KEY_11,
      KEY_12
    };
    
    //static const 
  
    VKey(uint8_t pin,
         eVref reference);
         
    bool CheckKeys(eKeynum & k);
    
  private:
    VKey(); // default constructor won't construct a valid object,
            // therefore it's hidden by making it private
    
    eKeynum voltage_to_key(int v);
    
    struct VKeyScale
    {
      prog_uint16_t offset;
      prog_uint16_t step_size;
      prog_uint16_t top;
    };

    static const PROGMEM VKeyScale scales[MAX];
    
    const PROGMEM  VKeyScale* scale_p;
    int analog_pin;
   
    eKeynum last_key;
};

#endif