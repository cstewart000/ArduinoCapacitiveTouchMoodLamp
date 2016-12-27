#include <Adafruit_NeoPixel.h>
#include <CapacitiveSensor.h>


#define capacitiveThreshold 25
#define CAPACITIVE_SAMPLES 100

#define CAPPIN1 11
#define CAPPIN2 12
#define STRIP_PIN 3

#define SHORT_PRESS 100
#define LONG_PRESS 500

#define NUM_MODES 2

CapacitiveSensor   cs = CapacitiveSensor(CAPPIN1,CAPPIN2); 

Adafruit_NeoPixel strip = Adafruit_NeoPixel(36, STRIP_PIN, NEO_GRB + NEO_KHZ800);

long buttonTimer = 0;
long time;
long buttonTime = 0;

long longPressTime = 250;


boolean buttonState = false;
boolean buttonLastState = false;
boolean longPressActive = false;

boolean checkButtonState();
int pressType(int);
void lampStateManager(int);
void updateLamp();

boolean lampOn =false;
int mode = 1;



void setup() {
  
    cs.set_CS_AutocaL_Millis(0xFFFFFFFF);
    Serial.begin(9600);
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'


}

void loop() {
  
  buttonState = checkButtonState();
  
  
  // New press
  if(buttonState && !buttonLastState){
    Serial.println("New press");
    buttonLastState =true; 
    buttonTimer = 0;
    buttonTime = millis();
  }
  
  // continue press
  if(buttonState){
    buttonLastState =true;
    //Serial.println("Continued press");
    buttonTimer = millis() - buttonTime;
    
  }
  
  // release
  if(!buttonState && buttonLastState){
    buttonLastState =false;

    
    int pressOption = pressType(buttonTimer);
    
    lampStateManager(pressOption);
    
    buttonTimer = 0;
  }
  
  updateLamp();
  
  Serial.print("lamp state:");
  Serial.print(lampOn);
  Serial.print("\tlamp mode:");
  Serial.println(mode);
  

}
/*

	if (digitalRead(button) == HIGH) {

		if (buttonActive == false) {

			buttonActive = true;
			buttonTimer = millis();

		}

		if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) {

			longPressActive = true;
			LED1State = !LED1State;
			digitalWrite(LED1, LED1State);

		}

	} else {

		if (buttonActive == true) {

			if (longPressActive == true) {

				longPressActive = false;

			} else {

				LED2State = !LED2State;
				digitalWrite(LED2, LED2State);

			}

			buttonActive = false;

		}

	}
*/


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
    colorWipe(strip.Color(0, 0, 0), 50); // off
  }else{
   
   switch(mode%NUM_MODES){
    case 0:
    colorWipe(strip.Color(222, 40, 179), 50); // 
    break;
    
    case 1:
    colorWipe(strip.Color(224, 169, 114), 50); //
    break;
    
    case 2:
    colorWipe(strip.Color(random(255), random(255), random(255)), 50); //
    break;
    
    
   }
   strip.show();
  }
  
  
  
  
}


void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
}
