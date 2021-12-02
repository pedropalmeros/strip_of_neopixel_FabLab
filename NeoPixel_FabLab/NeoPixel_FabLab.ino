// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define pinS1        6 // On Trinket or Gemma, suggest changing this to 1
#define pinS2        7
#define pinS3        8
#define pinS4        9

// How many NeoPixels are attached to the Arduino?
#define noS1         8 // Popular NeoPixel ring size
#define noS2         8
#define noS3         5
#define noS4         27

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel strip1(noS1, pinS1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(noS2, pinS2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3(noS3, pinS3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip4(noS4, pinS4, NEO_GRB + NEO_KHZ800);


const byte interruptPin = 3;
volatile byte state = LOW;
volatile int count = 0; 
volatile bool current_state = 0;
volatile bool pass_state = 0;
volatile unsigned long lastDebounceTime = 0;
volatile unsigned long debounceDelay = 50;

void allStripColor(Adafruit_NeoPixel &strip, int r, int g,int b);
void fadeRed();
void fadeBlue();
void stripOff();
void rainbow();
void theaterChaseRainbow();



#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.
  strip1.begin();
  strip2.begin();
  strip3.begin();
  strip4.begin();

  
  strip1.show();
  strip2.show();
  strip3.show();
  strip4.show();

  Serial.begin(9600);
  pinMode(interruptPin,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, FALLING);
}

void loop() {
  Serial.print("Cuenta: ");
  Serial.println(count);
  delay(50);

  switch(count){
    case 1:
      fadeRed();
      stripOff();
      allStripShow();
      break;
    case 2:
      fadeBlue();
      stripOff();
      allStripShow();
      break;
    case 3:
      rainbow();
      stripOff();
      allStripShow();
      break;
    case 4:
      theaterChaseRainbow();
      stripOff();
      allStripShow();
      
    default:
      stripOff();
      allStripShow();
  }
}

void allStripColor(Adafruit_NeoPixel &strip, int r, int g,int b){
  for(int i=0; i <= strip.numPixels(); i++){
    strip.setPixelColor(i, r, g, b);
  }
}

void fadeRed(){
  strip1.clear();
  strip2.clear();
  strip3.clear();
  strip4.clear();

  for(int i = 0; i<= 255; i+=5){
  allStripColor(strip1,i, 0, 0);
  allStripColor(strip2,i, 0, 0);
  allStripColor(strip3,i, 0, 0);
  allStripColor(strip4,i, 0, 0);
  Serial.println(count);
  if(count != 1){
    break;
  }

  strip1.show();
  strip2.show();
  strip3.show();
  strip4.show();
  delay(50);
  }
}


void fadeBlue(){
  strip1.clear();
  strip2.clear();
  strip3.clear();
  strip4.clear();

  for(int i = 0; i<= 255; i+=5){
  allStripColor(strip1,0, 0, i);
  allStripColor(strip2,0, 0, i);
  allStripColor(strip3,0, 0, i);
  allStripColor(strip4,0, 0, i);
  Serial.println(count);
  if(count != 2){
    break;
  }

  strip1.show();
  strip2.show();
  strip3.show();
  strip4.show();
  delay(50);
  }
}

void stripOff(){
  allStripColor(strip1,0, 0, 0);
  allStripColor(strip2,0, 0, 0);
  allStripColor(strip3,0, 0, 0);
  allStripColor(strip4,0, 0, 0);
}

void allStripShow(){
  strip1.show();
  strip2.show();
  strip3.show();
  strip4.show();
  
}

void blink() {
  current_state = 1; 
  if (current_state != pass_state) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if(current_state == pass_state){
      count++;
      current_state = 0;
    }
  }
  pass_state = current_state;
  if (count>=5){
    count = 0;
  }

}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow() {
  int i1 = 0;
  int i2 = 0;
  int i3 = 0;
  int i4 = 0;
  for(long firstPixelHue = 0; firstPixelHue < 3*65536; firstPixelHue += 256) {
     if(count != 3){
      break;
    }
    for(int i=0; i<strip4.numPixels(); i++) { // For each pixel in strip...
       if(count != 3){
        break;
      }
      int pixelHue = firstPixelHue + (i * 65536L / strip4.numPixels());
        Serial.println(count);
      
     
      // For Strip1
      i1 = i; 
      /*if(i1 >= strip1.numPixels()){
        i1 = 0;
      }*/
      strip1.setPixelColor(i1, strip1.gamma32(strip1.ColorHSV(pixelHue)));

      // For Strip2
      i2 = i; 
      /*if(i1 >= strip2.numPixels()){
        i2 = 0;
      }*/
      strip2.setPixelColor(i2, strip2.gamma32(strip2.ColorHSV(pixelHue)));

      // For Strip3
      i3 = i; 
      /*if(i3 >= strip3.numPixels()){
        i3 = 0;
      }*/
      strip3.setPixelColor(i3, strip3.gamma32(strip3.ColorHSV(pixelHue)));

      // For Strip4
      i4 = i; 
      /*if(i4 >= strip4.numPixels()){
        i4 = 0;
      }*/
      strip4.setPixelColor(i4, strip4.gamma32(strip4.ColorHSV(pixelHue)));
      
      strip1.show();
      strip2.show();
      strip3.show();
      strip4.show();
      delay(50);  // Pause for a moment
  }
}
}


// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow() {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    if(count != 4){
      break;
    }
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      if(count != 4){
        break;
      }
      strip1.clear();
      strip2.clear();
      strip3.clear();
      strip4.clear();

      
      //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip4.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip4.numPixels();
        uint32_t color1 = strip1.gamma32(strip1.ColorHSV(hue)); // hue -> RGB
        uint32_t color2 = strip2.gamma32(strip2.ColorHSV(hue)); // hue -> RGB
        uint32_t color3 = strip3.gamma32(strip3.ColorHSV(hue)); // hue -> RGB
        uint32_t color4 = strip4.gamma32(strip4.ColorHSV(hue)); // hue -> RGB
        strip1.setPixelColor(c, color1); // Set pixel 'c' to value 'color'
        strip2.setPixelColor(c, color2); // Set pixel 'c' to value 'color'
        strip3.setPixelColor(c, color3); // Set pixel 'c' to value 'color'
        strip4.setPixelColor(c, color4); // Set pixel 'c' to value 'color'

      }
      strip1.show();                // Update strip with new contents
      strip2.show(); 
      strip3.show(); 
      strip4.show(); 
      delay(50);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}
 
  
