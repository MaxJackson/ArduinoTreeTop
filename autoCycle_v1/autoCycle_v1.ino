/* FUTURE IDEAS
   manual color change
     change colors of all lights at the same time

   auto color change
     colors of all lights change at same time

   manual blink


*/

// Create Joystick Class
class Joystick {
  public:
    char xState;
    char yState;
};
Joystick *joystick_1;


// Initialize Chainable LEDs
#include <ChainableLED.h>
#define NUM_LEDS  3
ChainableLED leds(7, 8, NUM_LEDS);

// Joystick Variables
const int xPin = 0; // the x-axis input pin
const int yPin = 1; // the y-axis input pin
int xVal = 0; // the x-axis analog value (250 min, 510 resting, 770 max)
int yVal = 0; // the y-axis analog value (250 min, 510 resting, 770 max)

// LED Variables
int ledIndex = 0; // position
int colorIndex = 0;
int LEDrgbVals[3][3] = {{0, 255, 155}, {155, 0, 255}, {255, 155, 0}}; // RGB values, indexed as [ledIndex][colorIndex]
float brightness = 0.5;
// initialize the 'directions' that each value will shift; 'true' indicates that the number should increase and 'false' indicates that the number should decrease
bool isIncreasing[3][3] = {{true, true, true}, {true, true, true}, {true, true, true}};
int cycleIterator = 1;

//app modes
int currentMode = 0; //current modes
int totalsModes = 3; //total number of modes

int tempInt = 0;
float tempFloat = 0.0;

void updateLEDs(ChainableLED leds, int LEDrgbVals[3][3], float brightness, int cycleIterator) { // update LED values and write them to the strip

  for (ledIndex = 0; ledIndex < 3; ledIndex ++) { // loop through each LED
    for (colorIndex = 0; colorIndex < 3; colorIndex++) { // loop through each RGB color in each LED
      // Serial.println("updateLEDs");
      // Serial.println(LEDrgbVals[ledIndex][colorIndex]);
      if (LEDrgbVals[ledIndex][colorIndex] >= 255) { // if the value has reached the maximum
        LEDrgbVals[ledIndex][colorIndex] = 255; // make sure it doesn't exceed the maximum
        isIncreasing[ledIndex][colorIndex] = false; // tell the value to decrease
      }

      if (LEDrgbVals[ledIndex][colorIndex] <= 0) { // if the value has reached the minumum
        LEDrgbVals[ledIndex][colorIndex] = 0; // make sure it stays above the minimum
        isIncreasing[ledIndex][colorIndex] = true; // tell the value to increase
      }

      if (isIncreasing[ledIndex][colorIndex]) { // increase the value if the direction for the value is set to true
        LEDrgbVals[ledIndex][colorIndex] = LEDrgbVals[ledIndex][colorIndex] + cycleIterator;
      } else { // decrease the value if the direction for the value is set to false
        LEDrgbVals[ledIndex][colorIndex] = LEDrgbVals[ledIndex][colorIndex] - cycleIterator;
      }

    }
  }
  for (ledIndex = 0; ledIndex < 3; ledIndex++) { // write values to LEDs
    leds.setColorRGB(ledIndex, (int)LEDrgbVals[0][ledIndex]*brightness, (int)LEDrgbVals[1][ledIndex]*brightness, (int)LEDrgbVals[2][ledIndex]*brightness);
  }
}

char getXState(int xPin) { // get the state of the x axis
  char xState = 'i';
  xVal = analogRead(xPin); // read from the x-axis pin
  if (xVal > 1000) {
    xState = 'c'; // set the state to 'click'
    currentMode = cycleMode(currentMode);
  } else if (xVal > 520 && xVal < 1000) {
    xState = 'r'; // set the state to 'right'
  } else if (xVal < 500) {
    xState = 'l'; // set the state to 'left'
  } else {
    xState = 'i'; // set the state to 'idle'
  }
  return xState;
}

char getYState(int yPin) {
  char yState = 'i';
  yVal = analogRead(yPin); // read from the x-axis pin
  //Serial.println(yVal);
  if (yVal > 520) {
    yState = 'u'; // set the state to 'up'
  } else if (yVal < 500) {
    yState = 'd'; // set the state to 'down'
  }
  else {
    yState = 'i'; // set the state to 'idle'
  }
  return yState;
}

char cycleMode(int mode) { // get the state of the x axis
  mode = (mode + 1) % 3;
  Serial.println(mode);
  delay(300); // when clicked the button sets xVal above 1000 for several cycles; this delay ensures that a single button press doesn't cause the position to shift more than once
  return mode;
}


void setup() {
  Serial.begin(9600); // initialize serial output for debugging
  leds.init(); // initialize the led strip
  joystick_1 = new Joystick();
}


void loop() {

  // Update the joystick object with its x and y states
  joystick_1->xState = getXState(xPin); // get the x state
  joystick_1->yState = getYState(yPin); // get the y state

  switch (currentMode) {
    case 0:
      Serial.println("Mode 0");
      break;
    case 1:
      Serial.println("Mode 1");
      break;
    case 2:
      Serial.println("Mode 2");
      break;
      break;
  }



  switch (joystick_1->xState) {
    case 'r':
      Serial.println(joystick_1->xState);
      Serial.println(cycleIterator);
      // if cycleIterator is < 10, return cycleIterator + 1, otherwise return cycleIterator
      cycleIterator < 10 ? cycleIterator++ : cycleIterator;
      break;
    case 'l':
      Serial.println(joystick_1->xState);
      Serial.println(cycleIterator);
      // if cycleIterator is > 0, return cycleIterator - 1, otherwise return cycleIterator
      cycleIterator > 0 ? cycleIterator-- : cycleIterator;
      break;
    default:
      // if nothing else matches, do the default
      // default is optional
      break;
  }

  switch (joystick_1->yState) {
    case 'u':
      Serial.println(joystick_1->yState);
      Serial.println(brightness);
      // if brightness is < 0.99, return brightness + 0.01, otherwise return brightness
      brightness < 0.99 ? brightness = brightness + 0.01 : brightness;
      break;
    case 'd':
      Serial.println(joystick_1->yState);
      Serial.println(brightness);
      // if brightness is > 0.01, return brightness - 0.01, otherwise return brightness
      brightness > 0.01 ? brightness = brightness - 0.01 : brightness;
      break;
    default:
      // if nothing else matches, do the default
      // default is optional
      break;
  }

  updateLEDs(leds, LEDrgbVals, brightness, cycleIterator); // update the LEDs with every loop
}
