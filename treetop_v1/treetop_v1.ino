/* FUTURE IDEAS
   manual color change
     change colors of all lights at the same time

   auto color change
     colors of all lights change at same time

   manual blink


*/
// Initialize Chainable LEDs
#include <ChainableLED.h>
#define NUM_LEDS  3
ChainableLED leds(7, 8, NUM_LEDS);

// Initialize Status Bar
#include <Grove_LED_Bar.h>
Grove_LED_Bar bar(9, 8, 0);  

// Create Joystick Class
class Joystick {
  public:
    char xState;
    char yState;
};
Joystick *joystick_1;

// Joystick Variables
const int xPin = 0; // the x-axis input pin
const int yPin = 1; // the y-axis input pin

// LED Variables
int ledIndex = 0; // position
int colorIndex = 0;
int LEDrgbVals[3][3] = {{0, 255, 155}, {155, 0, 255}, {255, 155, 0}}; // RGB values, indexed as [ledIndex][colorIndex]
int cycleInitVals[3][3] = {{0, 255, 155}, {155, 0, 255}, {255, 155, 0}}; // initial RGB values for the color cycle mode
int rgbinaryInitVals[3][3] = {{255, 0, 0}, {255, 0, 0}, {255, 0, 0}}; // initial RGB values for the binary counting mode
float brightness = 0.5; // initial brightness
// initialize the 'directions' that each value will shift; 'true' indicates that the number should increase and 'false' indicates that the number should decrease

// Cycle Variales
bool isIncreasing[3][3] = {{true, true, true}, {true, true, true}, {true, true, true}}; // 2D array to hold boolean values which determine if any given RGB value is increasing during the cycle mode
const int cycleIteratorMax = 10;
int cycleIterator = 1;

// RG Binary Variables
int binaryCounter = 0; // variables necessary for the calculation of the binary representation of a given number
float remainder = 0.0;
float binaryPower = 0;
int iterateRGBTimer = 500; // initialize the timer to 500 ms
const int iterateRGBTimerMax = 1000; // timer maximimum is 1000ms

//app modes
int currentMode = 0; //current modes
int totalsModes = 3; //total number of modes
bool initializedForCycle = false; // boolean value to identify if the LEDs have been initialized for the cycle mode
bool initializedForRGBinary = false; // boolean value to identify if the LEDs have been initialized for the counting mode

unsigned long TimerA; 
int tempInt = 0;
float tempFloat = 0.0;

void writeLEDs(ChainableLED leds, int LEDrgbVals[3][3], float brightness){ // write the RGB values to the LEDs themselves
  for (ledIndex = 0; ledIndex < 3; ledIndex++) { // write values to LEDs
    leds.setColorRGB(ledIndex, (int)LEDrgbVals[ledIndex][0]*brightness, (int)LEDrgbVals[ledIndex][1]*brightness, (int)LEDrgbVals[ledIndex][2]*brightness);
  }
}

void initializeLEDValsForCycle(int LEDrgbVals[3][3]){ // initialize the LED values for the color cycle
  for(ledIndex = 0; ledIndex < NUM_LEDS; ledIndex++){
    for(colorIndex = 0; colorIndex < 3; colorIndex++){
      LEDrgbVals[ledIndex][colorIndex] = cycleInitVals[ledIndex][colorIndex];
    }
  }
}

void initializeLEDValsForRGBinary(int LEDrgbVals[3][3]){ // Initialize the LED RGB values for the red-green binary counting
  for(ledIndex = 0; ledIndex < NUM_LEDS; ledIndex++){
    for(colorIndex = 0; colorIndex < 3; colorIndex++){
      LEDrgbVals[ledIndex][colorIndex] = rgbinaryInitVals[ledIndex][colorIndex];
    }
  }
}

void printRGBsToConsole(int LEDrgbVals[3][3]){ // print all the RGB values to the console for debugging
    for (ledIndex = 0; ledIndex < NUM_LEDS; ledIndex ++) { // loop through each LED
      Serial.print(ledIndex);
      Serial.print(": ");
      for (colorIndex = 0; colorIndex < 3; colorIndex++) { // loop through each RGB color in each LED
        Serial.print(LEDrgbVals[ledIndex][colorIndex]);
        Serial.print(" ");
      }
      Serial.println(" ");
    }
    Serial.println(" ");
}


void cycleLEDVals(int LEDrgbVals[4][3], int cycleIterator, bool isIncreasing[4][3]) { // update LED values and write them to the strip
  for (ledIndex = 0; ledIndex < NUM_LEDS; ledIndex ++) { // loop through each LED
    for (colorIndex = 0; colorIndex < 3; colorIndex++) { // loop through each RGB color in each LED
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
}

void iterateRGBinary(int LEDrgbVals[4][3], int binaryCounter){
  Serial.println(binaryCounter);
  remainder = (float)binaryCounter; // initialize the remainder to the initial value of the counter
  for (ledIndex = 0; ledIndex < NUM_LEDS; ledIndex++) {
    binaryPower = pow((float)2, (float)(3-ledIndex)); // calculate two to the power of the index as counted from the left
    if(remainder >= binaryPower){ // if this result is greater than the remainder
      //Serial.print(remainder);
      //Serial.print(" >= ");
      //Serial.println(binaryPower);
      LEDrgbVals[ledIndex][0] = 0; // set the led of that index to green
      LEDrgbVals[ledIndex][1] = 255;
      LEDrgbVals[ledIndex][2] = 0;
      remainder = fmod(remainder, binaryPower); // caulculate the remainder after dividing by that power of two
      //Serial.println(remainder);
      Serial.print(1);
    } else {  // otherwise set the led to red
      LEDrgbVals[ledIndex][0] = 255; 
      LEDrgbVals[ledIndex][1] = 0;
      LEDrgbVals[ledIndex][2] = 0;
      Serial.print(0);
    }
  }
  Serial.println("");
  //printRGBsToConsole(LEDrgbVals);
}

char getXState(int xPin) { // get the state of the x axis
  char xState = 'i';
  int xVal = analogRead(xPin); // read from the x-axis pin
  //Serial.println(xVal);
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
  int yVal = analogRead(yPin); // read from the y-axis pin
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
  //Serial.println(mode);
  delay(300); // when clicked the button sets xVal above 1000 for several cycles; this delay ensures that a single button press doesn't cause the position to shift more than once
  return mode;
}


void setup() {
  Serial.begin(9600); // initialize serial output for debugging
  bar.begin(); // initialize led bar
  leds.init(); // initialize the led strip
  joystick_1 = new Joystick(); // create new instance of the joystick class
}


void loop() {

  // Update the joystick object with its x and y states
  joystick_1->xState = getXState(xPin); // get the x state
  joystick_1->yState = getYState(yPin); // get the y state

  switch (currentMode) {
    case 0:
      //Serial.println("Mode 0 (gradient cycle)");
      initializedForRGBinary = false;
      if(!initializedForCycle){
        initializeLEDValsForCycle(LEDrgbVals);
        initializedForCycle = true;
       }
      cycleLEDVals(LEDrgbVals, cycleIterator, isIncreasing); // update the LEDs with every loop
      break;
    case 1:
      //Serial.println("Mode 1 (RG Binary");
      initializedForCycle = false;
      if(!initializedForRGBinary){
        initializeLEDValsForRGBinary(LEDrgbVals);
        initializedForRGBinary = true;
        TimerA = millis();
      }
      if (millis() - TimerA >= iterateRGBTimer){
        iterateRGBinary(LEDrgbVals, binaryCounter);
        binaryCounter = (binaryCounter + 1) % 16;
        TimerA = millis();
      }
      break;
    case 2:
      //Serial.println("Mode 2");
      initializedForRGBinary = false;
      initializedForCycle = false;
      break;
      break;
  }
  bar.setLevel(currentMode+1);



  // Check on the joystick x axis 
  switch (joystick_1->xState) {
    case 'r':
      Serial.println(joystick_1->xState);
      Serial.println(iterateRGBTimer);
      // if cycleIterator is < cycleIteratorMax, return cycleIterator + 1, otherwise return cycleIterator
      cycleIterator < cycleIteratorMax ? cycleIterator++ : cycleIterator;
      iterateRGBTimer < iterateRGBTimerMax ? iterateRGBTimer++ : iterateRGBTimer;
      break;
    case 'l':      LEDrgbVals[ledIndex][1] = 0;

      Serial.println(joystick_1->xState);
      Serial.println(iterateRGBTimer);
      // if cycleIterator is > 0, return cycleIterator - 1, otherwise return cycleIterator
      cycleIterator > 0 ? cycleIterator-- : cycleIterator;
      iterateRGBTimer > 0 ? iterateRGBTimer = iterateRGBTimer - 10 : iterateRGBTimer;
      break;
    default:
      // if nothing else matches, do the default
      // default is optional
      break;
  }

  // Check on the joystick y axis
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
  //printRGBsToConsole(LEDrgbVals);
  writeLEDs(leds, LEDrgbVals, brightness);
  
}
