/*
Controlling the SN74HC595 Shift Register
Author: Paul Ybarra
Date; June 1, 2019

*/

/*
 * SRCLR clears the input registers
 * SRCLK controls input registers
 * OE controls output display
 * RCLK controls output register
 * 
 * Toggling RCLK gets next value stored in input register
 * Toggling SRCLK moves shifts data by one bit
 *   Anything in SER is put in 1st bit
 *   Whatever was stored in 1st bit goes to 2nd bit
 *   ... and so on
 *   
 * SRCLR clears the input registers when it goes low
 *   On the next RCLK pulse the output reflects this state
 * 
 * Process to update output
 * 
 * 1) Set RCLK state low
 * 2) Update SER 
 * 3) Toggle SRCLK (low to high)
 * 3) Set RCLK state high (which makes update visible) 
 * 
 * To clear registers (does not depend on SRCLK)
 * 
 * 1) Set RCLK state low
 * 2) toggle SRCLR (low to high)
 * 3) Set RCLK state high (which makes output blank) 
 */

int SRCLR = 3;
int SER   = 4;
int RCLK  = 5;
int SRCLK = 6;
int OE    = 9;

byte leds = 0;

void setup() 
{

  // Set Port D for output - 1 = Output 0 = Input

  DDRD |=  (1 << SRCLK) | (1 << RCLK) | (1 << SER) | (1 << SRCLR) | (1 << OE);

  // Set the outputs to High
  
  PORTD |= (1 << SRCLK) | (1 << RCLK) | (1 << SER) | (1 << SRCLR);
  PORTD &= ~(1 << OE);
 
}

void loop() 
{

 bitFade(5);
 onOffFlash(100);
 bitGrow(25);
 // splitFlash dur  = 50+ is best
 splitFlash(50);
 // chirp dur = 30 - 50 ms is best
 chirp(30);
 for (int i =0 ; i <= 5; i++)
  {
    leftRightFlash(25);
  }
 for (int i =0 ; i <= 5; i++)
  {
     bumpingBits(50);
  }
  binaryCounter(20);
}

void updateShiftRegister(int order)
{

   // send 8 bits of data to input register then show output
   // To reverse order change MSBFIRST to LSBFIRST
   
   PORTD &=  ~(1 << RCLK);
   shiftOut(SER, SRCLK, order, leds);
   PORTD |=  (1 << RCLK);
}

void binaryCounter(int dur)
{

  // Clear input regester and blank output
  
  PORTD &=  ~(1 << RCLK);
  PORTD &= ~(1 << SRCLR);
  PORTD |=  (1 << SRCLR);
  PORTD |=  (1 << RCLK);
  
  leds = 0;
  updateShiftRegister(MSBFIRST);
  delay(dur);
    
  for (int i = 0; i < 256; i++)
  {
    leds = i;
    updateShiftRegister(MSBFIRST);
    delay(dur);
  }
}

void leftRightFlash(int dur)
{

  // Clear input regester and blank output
  
  PORTD &=  ~(1 << RCLK);
  PORTD &= ~(1 << SRCLR);
  PORTD |=  (1 << SRCLR);
  PORTD |=  (1 << RCLK);
  
  leds = 0;
  updateShiftRegister(MSBFIRST);
  delay(dur);
    
  for (int i = 0; i < 8; i++)
  {
    leds = (1 << i);
    updateShiftRegister(MSBFIRST);
    delay(dur);
  }

  leds = 0;
  updateShiftRegister(LSBFIRST);
  delay(dur);
  
  for (int i = 0; i < 8; i++)
  {
    leds = (1 << i);
    updateShiftRegister(LSBFIRST);
    delay(dur);
  }

  
}

void chirp(int dur)
{

  // Clear input regester and blank output
  
  PORTD &=  ~(1 << RCLK);
  PORTD &= ~(1 << SRCLR);
  PORTD |=  (1 << SRCLR);
  PORTD |=  (1 << RCLK);
      
  for (int i = 1; i < dur; i++)
  {
    leds = 0;
    updateShiftRegister(MSBFIRST);
    delay(i);
    for (int k = 0; k < 8; k++)
    {
      leds = (1 << k);
      updateShiftRegister(MSBFIRST);
      delay(i);
    }
  }
}

void onOffFlash(int dur)
{

  // Clear input regester and blank output
  
  PORTD &=  ~(1 << RCLK);
  PORTD &= ~(1 << SRCLR);
  PORTD |=  (1 << SRCLR);
  PORTD |=  (1 << RCLK);

  for (int i = 0; i < 10; i++)
  {
    leds = 0;
    updateShiftRegister(MSBFIRST);
    delay(dur);
    leds = 255;
    updateShiftRegister(MSBFIRST);
    delay(dur); 
  }
}


void splitFlash(int dur)
{

  // Clear input register and blank output
  
  PORTD &=  ~(1 << RCLK);
  PORTD &= ~(1 << SRCLR);
  PORTD |=  (1 << SRCLR);
  PORTD |=  (1 << RCLK);
 
  for (int i = 0; i < 5; i++)
  {
    leds = 0;
    for (int k = 0; k <= 3; k++)
    {
       leds |= (1 << (4 + k) ) | (1 << (3 - k) );
       updateShiftRegister(MSBFIRST);
       delay(dur);   
    }
       
    for (int k = 3; k >= 0; k--)
    {
       leds &= ~((1 << (4 + k) ) | (1 << (3 - k) ));
       updateShiftRegister(MSBFIRST);
       delay(dur);   
    }
  }
  
}

void bitGrow(int dur)
{

  // Clear input register and blank output
  
  PORTD &=  ~(1 << RCLK);
  PORTD &= ~(1 << SRCLR);
  PORTD |=  (1 << SRCLR);
  PORTD |=  (1 << RCLK);
 
  for (int k = 0; k < 5; k++)
  {
    leds = 0;
    for (int i = 0; i < 8; i++)
    {
       leds |= (1 << i);
       updateShiftRegister(MSBFIRST);
       delay(dur);   
    }
       
    for (int i = 7; i >= 0; i--)
    {
       leds &= ~(1 << i);
       updateShiftRegister(MSBFIRST);
       delay(dur);   
    }
  }
  
}

void bitFade(int dur)
{

  // Clear input register and blank output
  
  PORTD &=  ~(1 << RCLK);
  PORTD &= ~(1 << SRCLR);
  PORTD |=  (1 << SRCLR);
  PORTD |=  (1 << RCLK);

  leds = 255;
  updateShiftRegister(MSBFIRST);
  delay(dur);  
  
  for (int k = 0; k < 3; k++)
  {            
    for (int fadeValue = 0 ; fadeValue <= 255; fadeValue++) 
      {
        // sets the value (range from 0 to 255):
        analogWrite(OE, fadeValue);
        // wait for 30 milliseconds to see the dimming effect
        delay(dur);
      }
 
     for (int fadeValue = 255 ; fadeValue >=0; fadeValue--) 
      {
        // sets the value (range from 0 to 255):
        analogWrite(OE, fadeValue);
        // wait for 30 milliseconds to see the dimming effect
        delay(dur);
      }
  } 
  PORTD &= ~(1 << OE);
}

void bumpingBits(int dur)
{

  // Clear input regester and blank output
  
  PORTD &=  ~(1 << RCLK);
  PORTD &= ~(1 << SRCLR);
  PORTD |=  (1 << SRCLR);
  PORTD |=  (1 << RCLK);
  
  leds = 0;
  updateShiftRegister(MSBFIRST);
  delay(dur);
    
  for (int i = 0; i < 8; i++)
  {
    leds = (1 << i) | (1 << (7-i));
    updateShiftRegister(MSBFIRST);
    delay(dur);
  }
  
}
