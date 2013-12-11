/*
 12-10-2015
 Spark Fun Electronics
 Byron Jacquot
 
 This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 
 This demonstrates decoding the input from the VKey voltage keypad.
*/

int A0 = 1;

byte calcButton(int val)
{
  byte btn;
  
#if 1
  // 3.3V calculation
  if( (val < 16) || (val > 480))
    return 0;
  
  return( 12 - ((val - 16)/38));

  // After experimentation, 38.5 is a likely ideal divisor...mul by 2 and use 77?

#else
  // 5V calculation
  if( (val < 17) || (val > 500))
    return 0;
  
  return( 12 - ((val - 17)/40));
#endif  
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Trimpot read example");

  pinMode(A0, INPUT);
}

void loop() 
{
  int myValue = 0;
  myValue = analogRead(A0);
  
  Serial.print("The value is: ");
  Serial.print(myValue);
  Serial.print(" ");
  Serial.println( calcButton(myValue) );

  delay(250);
}

