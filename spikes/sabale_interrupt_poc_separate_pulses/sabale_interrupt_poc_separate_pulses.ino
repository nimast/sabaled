#include <TimerOne.h>
#include <Adafruit_NeoPixel.h>

#define PIN 6

// This example uses the timer interrupt to blink an LED
// and also demonstrates how to share a variable between
// the interrupt and the main program.

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(50, PIN, NEO_GRB + NEO_KHZ400);


const int led = LED_BUILTIN;  // the pin with a LED

void setup(void)
{
  strip.begin();
  strip.show();
  pinMode(led, OUTPUT);
  Timer1.initialize(50000);
  Timer1.attachInterrupt(blinkLED); // blinkLED to run every 0.01 second
  Serial.begin(9600);
}


// The interrupt will blink the LED, and keep
// track of how many times it has blinked.
int ledState = LOW;
volatile unsigned long blinkCount = 0; // use volatile for shared variables


int pulse_step = 10;
int pulse_max = 60;
int *pulse_index = 0;
int *pulse_current_level = 0;

int *pulse2_index = 0;
int *pulse2_current_level = 0;

void make_pulse_step(int *local_index, int *local_level)
{
  if (*local_level < pulse_max) 
  {
      *local_level += pulse_step;  
  }
  
  // go up
  int temp_level = *local_level;
  int temp_index = *local_index;
  while(temp_level > 0)
  {
     strip.setPixelColor(temp_index, strip.Color(temp_level, 0, 0));
     temp_index++;
     temp_level = *local_level - ((temp_index - *local_index) * pulse_step);
  }  
  
  if(*local_level == pulse_max) 
  {
      temp_index = *local_index - 1;
      temp_level = *local_level - ((*local_index - temp_index) * pulse_step);
      while(temp_index >= 0 && temp_level >= 0)
      {
         strip.setPixelColor(temp_index, strip.Color(temp_level, 0, 0));
         temp_index--;
         temp_level = *local_level - ((*local_index - temp_index) * pulse_step);
      }
  }
  
  strip.show();
  
  // Only when we reach max level, start advancing.
  // This creates the effect of the pulse taking shape at the beginning of the strip instead of starting in the middle of it.
  if (*local_level == pulse_max) 
  {
      *local_index++;
  }
   
  if (*local_index == strip.numPixels())
  {
    // Reaches the end of the strip, end the pulse gracefuly in a manner
    // similar to the one in which we begin i.e decade the pulse slowly into the 
    // end of the strip instead of turning off multiple leds at once.
    while(*local_level > 0)
    {
        temp_index = *local_index - 1;
        temp_level = *local_level - ((*local_index - temp_index) * pulse_step);
        while(temp_index >= 0 && temp_level >= 0)
        {
           strip.setPixelColor(temp_index, strip.Color(temp_level, 0, 0));
           temp_index--;
           temp_level = *local_level - ((*local_index - temp_index) * pulse_step);
        }
  
        *local_level -= pulse_step;      
    }
      *local_level = 0;
      *local_index = 0;
  }
}

void blinkLED(void)
{
  make_pulse_step(pulse_index, pulse_current_level);
  
   
   
   
}


// The main program will print the blink count
// to the Arduino Serial Monitor
void loop(void)
{
  unsigned long blinkCopy;  // holds a copy of the blinkCount

  // to read a variable which the interrupt code writes, we
  // must temporarily disable interrupts, to be sure it will
  // not change while we are reading.  To minimize the time
  // with interrupts off, just quickly make a copy, and then
  // use the copy while allowing the interrupt to keep working.
  noInterrupts();
  blinkCopy = blinkCount;
  interrupts();

  delay(100);
}

