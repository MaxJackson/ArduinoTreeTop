/* FUTURE IDEAS
 * manual color change
 *   change colors of all lights at the same time
 * 
 * auto color change
 *   colors of all lights change at same time
 * 
 * manual blink
 *  
 * 
 */

// Initialize Chainable LEDs
#include <ChainableLED.h>
#define NUM_LEDS  3
ChainableLED leds(7, 8, NUM_LEDS);

// Joystick Variables
const int xPin = 0; // the x-axis input pin   
const int yPin = 1; // the y-axis input pin
int xVal = 0; // the x-axis analog value (250 min, 510 resting, 770 max)
int yVal = 0; // the y-axis analog value (250 min, 510 resting, 770 max)
char xState = 'i';
char yState = 'i';
char clickState = 'i';

// LED Variables
int ledIndex = 0; // position
int colorIndex = 0;
int LEDrgbVals[3][3] = {{0, 255, 155}, {155, 0, 255}, {255, 155, 0}}; // RGB values, indexed as [ledIndex][colorIndex]
float brightness = 0.5;
// initialize the 'directions' that each value will shift; 'true' indicates that the number should increase and 'false' indicates that the number should decrease
bool isIncreasing[3][3] = {{true, true, true}, {true, true, true}, {true, true, true}};
int cycleIterator = 1;

int tempInt = 0;
float tempFloat = 0.0;

void updateLEDs(ChainableLED leds, int LEDrgbVals[3][3], float brightness, int cycleIterator){ // update LED values and write them to the strip
  
  for(ledIndex = 0; ledIndex < 3; ledIndex ++){ // loop through each LED
    for(colorIndex = 0; colorIndex < 3; colorIndex++){ // loop through each RGB color in each LED
      // Serial.println("updateLEDs");
      // Serial.println(LEDrgbVals[ledIndex][colorIndex]);
      if(LEDrgbVals[ledIndex][colorIndex] >= 255){ // if the value has reached the maximum 
        LEDrgbVals[ledIndex][colorIndex] = 255; // make sure it doesn't exceed the maximum
        isIncreasing[ledIndex][colorIndex] = false; // tell the value to decrease
      }

      if(LEDrgbVals[ledIndex][colorIndex] <= 0){ // if the value has reached the minumum
       LEDrgbVals[ledIndex][colorIndex] = 0; // make sure it stays above the minimum
       isIncreasing[ledIndex][colorIndex] = true; // tell the value to increase
      }

      if (isIncreasing[ledIndex][colorIndex]){ // increase the value if the direction for the value is set to true
       LEDrgbVals[ledIndex][colorIndex] = LEDrgbVals[ledIndex][colorIndex] + cycleIterator;
      } else { // decrease the value if the direction for the value is set to false
       LEDrgbVals[ledIndex][colorIndex] = LEDrgbVals[ledIndex][colorIndex] - cycleIterator;
      }

    }
  }
  for(ledIndex = 0; ledIndex < 3; ledIndex++){ // write values to LEDs
    leds.setColorRGB(ledIndex, (int)LEDrgbVals[0][ledIndex]*brightness, (int)LEDrgbVals[1][ledIndex]*brightness, (int)LEDrgbVals[2][ledIndex]*brightness);
  }
}

char getXState(int xPin){ // get the state of the x axis
  xVal = analogRead(xPin); // read from the x-axis pin 
  if(xVal > 1000) {
    xState = 'c'; // set the state to 'click'
  } else if(xVal > 520 && xVal < 1000){
    xState = 'r'; // set the state to 'right'
  } else if(xVal < 500) {
    xState = 'l'; // set the state to 'left'
  } else {
    xState = 'i'; // set the state to 'idle'
  }
  return xState;
}

char getYState(int yPin){
  yVal = analogRead(yPin); // read from the x-axis pin 
  //Serial.println(yVal);
  if(yVal > 520){
    yState = 'u'; // set the state to 'up'
  } else if(yVal < 500) {
    yState = 'd'; // set the state to 'down'
  }
  else {
    yState = 'i'; // set the state to 'idle'
  }
  return yState;
}




void setup() {
  Serial.begin(9600); // initialize serial output for debugging
  leds.init(); // initialize the led strip
}


void loop() {

  xState = getXState(xPin); // get the x state
  yState = getYState(yPin); // get the y state

  switch (xState) {
    case 'r':
      Serial.println(xState);
      Serial.println(cycleIterator);
      // if cycleIterator is < 10, return cycleIterator + 1, otherwise return cycleIterator
      cycleIterator < 10 ? cycleIterator++ : cycleIterator; 
      break;
    case 'l':
      Serial.println(xState);
      Serial.println(cycleIterator);
      // if cycleIterator is > 0, return cycleIterator - 1, otherwise return cycleIterator
      cycleIterator > 0 ? cycleIterator-- : cycleIterator; 
      break;
    default: 
      // if nothing else matches, do the default
      // default is optional
    break;
  }

  switch (yState) {
    case 'u':
      Serial.println(yState);
      Serial.println(brightness);
      // if brightness is < 0.99, return brightness + 0.01, otherwise return brightness
      brightness < 0.99 ? brightness=brightness+0.01 : brightness; 
      break;
    case 'd':
      Serial.println(yState);
      Serial.println(brightness);
      // if brightness is > 0.01, return brightness - 0.01, otherwise return brightness
      brightness > 0.01 ? brightness=brightness-0.01 : brightness; 
      break;
    default: 
      // if nothing else matches, do the default
      // default is optional
    break;
  }

   updateLEDs(leds, LEDrgbVals, brightness, cycleIterator); // update the LEDs with every loop
}
