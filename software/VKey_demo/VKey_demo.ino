/*
 12-10-2015
 Spark Fun Electronics
 Byron Jacquot
 
 This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 
 This demonstrates decoding the input from the VKey voltage keypad.
*/

class VKey
{
  public:
  
    enum eVref
    {
      THREE,
      FIVE,
      MAX
      // FNORD: 3V ref, 5V supply?  Ask MH...
    };
    
    
    
    enum input_pin
    {
      PIN_A0 = 0,
      PIN_A1 = 1,
      PIN_A2 = 2,
      PIN_A3 = 3,
      PIN_A4 = 4,
      PIN_A5 = 5,
      // FNORD conditional compilation for more pins?
    };
    
    enum key
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
  
    VKey(input_pin input,
         eVref reference);
         
    bool CheckForChange(key & k);
    key  ReadCurrentButton();
    
  private:
    VKey(); // default constructor is useless, therefore pidden by making it private
    
    key voltage_to_key(int v);
    
    struct VKeyScale
    {
      int offset;
      int step_size;
      int top;
    };

    static const VKeyScale scales[MAX];
    
    const VKeyScale* scale_p;
    int analog_pin;
   
    int last_key;
};

const VKey::VKeyScale VKey::scales[MAX] =
{
   // min, step, max
   {26,    58,   718},
   {17,    40,   500}
};


VKey::VKey(input_pin input,
         eVref reference)
{
  analog_pin = input;
  last_key = 0;
  
  scale_p = &scales[reference];  
  
  pinMode(input, INPUT);
}

VKey::key VKey::voltage_to_key(int v)
{
  VKey::key k;
  
  if( (v < scale_p->offset) || (v > scale_p->top))
  {
    k = KEY_NONE;
  }
  else
  {
    k = VKey::key(12 - ((v - scale_p->offset)/scale_p->step_size));
  }
  
  return k;
}

bool VKey::CheckForChange(key & k)
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



////////////////////////////////////////////////////////////


VKey keypad(VKey::PIN_A1, VKey::THREE );

void setup()
{
  Serial.begin(9600);
  Serial.println("Trimpot read example");
}

void loop() 
{
#if 0  
  int myValue = 0;
  myValue = analogRead(A0);
  
  Serial.print("The value is: ");
  Serial.print(myValue);
  Serial.print(" ");
  Serial.println( calcButton(myValue) );
#endif

  VKey::key k;

  if(keypad.CheckForChange(k))
  {
    Serial.print("Got key: ");
    Serial.println(int(k));
  }

  delay(50);
}

