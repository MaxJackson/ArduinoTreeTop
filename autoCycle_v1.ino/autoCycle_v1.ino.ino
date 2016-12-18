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
  // Serial.println(xState); // print the states to console for debugging
  // Serial.println(yState);
  // Serial.println('\n');
 
  if(xState == 'r'){ // if the stick is pulled to the right
    if(cycleIterator < 10){ // if the cycle iterator is less than its maximum value of 10,
      cycleIterator = cycleIterator + 1; // ...increase the cycle iterator by one, speeding up the cycle
    }
  }
  if(xState == 'l'){ // if the stick is pulled to hte left
    if(cycleIterator > 0){ // if the cycle iterator is more than its minimum value of 0
     cycleIterator = cycleIterator - 1; // decrease the cycle iterator by one, slowing down the cycle
    }
  }
 

 if(yState == 'u'){ // if the stick is pulled up
  if(brightness < 0.99){ // if the brightness is less than the maximum
    brightness = brightness + 0.01; // increase the brightness
  }
 }

 if(yState == 'd'){ // if the stick is pulled down
    if(brightness > 0.01){ // if the brightness is more than the minimum
      brightness = brightness - 0.01; // decrease the brightness
    }
  }

   updateLEDs(leds, LEDrgbVals, brightness, cycleIterator); // update the LEDs with every loop
}
