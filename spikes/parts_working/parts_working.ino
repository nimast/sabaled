#include <TimerOne.h>
#include <Adafruit_NeoPixel.h>
#include <Sabalib.h>

#define PIN 6

#define NUM_OF_STRIPS 1
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
Adafruit_NeoPixel strip(50, PIN, NEO_GRB + NEO_KHZ400);

//Adafruit_NeoPixel* strips[NUM_OF_STRIPS];
#define NUM_OF_SECTIONS 1

Section section = Section(0,10,&strip);
BogusPart part = BogusPart(&section, NUM_OF_SECTIONS);

const int led = LED_BUILTIN;  // the pin with a LED

void setup(void)
{  
  Serial.begin(9600);
  section.begin();
  section.show();
  
  //strip.begin();
  //strip.show();
  pinMode(led, OUTPUT);
  Timer1.initialize(50000);
  Timer1.attachInterrupt(blinkLED); // blinkLED to run every 0.5 second
  
}


// The interrupt will blink the LED, and keep
// track of how many times it has blinked.
int ledState = LOW;
volatile unsigned long blinkCount = 0; // use volatile for shared variables

void blinkLED(void)
{
  part.tick();
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
  //noInterrupts();
  //blinkCopy = blinkCount;
  //interrupts();

  //Serial.print("blinkCount = ");
  //Serial.println(blinkCopy);
  
//  uint8_t* pixels = strip.getPixels();
// 	Serial.println('Pixels array:');
// 	for(int i=0;i<strip.numPixels();i++) {
// 		Serial.print(pixels[i]);
// 	}
// 
  delay(200);
}

