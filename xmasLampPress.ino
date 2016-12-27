/*
    Arduino Capacitive Touch Mood Lamp

    A simple program for a Mood Lamp with a single imput, a capacitive touch surface, 
    which detects different press types (ie short, long) to change the behaviour. 
    The program uses the AdaFroit NeoPixel library to drive RGB LEDs.

    The circuit:

      - D3 : Connected to the signal line of the LED strip.
      - D11 : Connected to a 10M resistor and to the touch surface.
      - D12 : Connected to touch surface. 

      - Power where it should be :P

      Created: 24/12/2016
      By: Cameron Stewart, cstewart000@gmail.com


      https://github.com/cstewart000/ArduinoCapacitiveTouchMoodLamp
*/


// Libraries
#include <Adafruit_NeoPixel.h>
#include <CapacitiveSensor.h>

// Hardware definitions
#define CAPPIN1 11
#define CAPPIN2 12
#define STRIP_PIN 3
#define NUM_PIXELS 36

// Capacitive touch sensor configuration
#define capacitiveThreshold 25
#define CAPACITIVE_SAMPLES 100

// Press min duration definition 
#define SHORT_PRESS 100
#define LONG_PRESS 500

// Number of different mood routines to be defined
#define NUM_MODES 2

// Initialisaiton of library objects
CapacitiveSensor   cs = CapacitiveSensor(CAPPIN1,CAPPIN2); 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, STRIP_PIN, NEO_GRB + NEO_KHZ800);


// Global state variables
long buttonTimer = 0;
long time;
long buttonTime = 0;
long longPressTime = 250;

boolean buttonState = false;
boolean buttonLastState = false;
boolean longPressActive = false;
boolean lampOn =false;

int mode = 1;

// Method declarations
boolean checkButtonState();
int pressType(int);
void lampStateManager(int);
void updateLamp();


void setup() {
  
    cs.set_CS_AutocaL_Millis(0xFFFFFFFF);
    Serial.begin(9600);
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  
  // TODO: Check button state in interrupt to stop code blocking
  buttonState = checkButtonState();
  
  // New press
  if(buttonState && !buttonLastState){
    
    Serial.println("New press");
    buttonLastState =true; 
    buttonTimer = 0;
    buttonTime = millis();
  
  }
  
  // Continue press
  if(buttonState){

    Serial.println("Continued press");
    buttonLastState = true;
    buttonTimer = millis() - buttonTime; // record time that the button has been depressed.
    
  }
  
  // release
  if(!buttonState && buttonLastState){
    
    Serial.println("Released press");
    buttonLastState = false;
    int pressOption = pressType(buttonTimer); // determine the type of press from depression time
    lampStateManager(pressOption); // Act based on press type.
    
    buttonTimer = 0;
  }
  
  // Update the lamp based on changed state variables.
  updateLamp();
  
  // Debug logging
  Serial.print("lamp state:");
  Serial.print(lampOn);
  Serial.print("\tlamp mode:");
  Serial.println(mode);
  
}


boolean checkButtonState(){

  long capacitiveSwitchValue = cs.capacitiveSensor(CAPACITIVE_SAMPLES);

  if(capacitiveSwitchValue>capacitiveThreshold){
    return true;
  }else{
    return false;
  }
}

int pressType(int buttonTimer){
  
  if(buttonTimer>LONG_PRESS){
    return 1;
  }else if(buttonTimer>SHORT_PRESS){
    return 2;

    // Add more branching options here for more other behaviours

    // TODO: Held press - scroll over the colour wheel.

    // TODO: Double tap - step forward
    
    // TODO: Triple tap - step back

  }else
  return 0;    
  
}

void lampStateManager(int pressOption){
  
  switch(pressOption){
    
    //long press toggles on/off
    case 1:
      lampOn =!lampOn;
    break;
    
    // short press increments the mode
    case 2:
      mode =mode++;
    break;
    default:
    
    break;
    
  }
    
}


void updateLamp(){
  
  if(!lampOn){
    colourStripSet(strip.Color(0, 0, 0)); // off
  }else{
   
   switch(mode%NUM_MODES){
    case 0:
    colourStripSet(strip.Color(222, 40, 179)); // 
    break;
    
    case 1:
    colourStripSet(strip.Color(224, 169, 114)); //
    break;
    
    case 2:
    colourStripSet(strip.Color(random(255), random(255), random(255))); //
    break;
    
    // TODO: Add case statements for new behaviours
    
   }
   strip.show();
  }
}

// Sets strip to a new colour
void colourStripSet(uint32_t colour {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, colour);
  }
  strip.show();
}
