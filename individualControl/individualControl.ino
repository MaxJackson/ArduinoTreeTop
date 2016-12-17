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
int pos = 0; // position
int redVals[3] = {0, 155, 255};
int greenVals[3] = {255, 0, 155};
int blueVals[3] = {155, 255, 0};
float brightnesses[3] = {0.5, 0.5, 0.5};
bool directions[3][3] = {{true, true, true}, {true, true, true}, {true, true, true}};

int tempInt = 0;
float tempFloat = 0.0;

void setup() {
  Serial.begin(9600); // initialize serial output for debugging
  leds.init();
}



void loop() {
  xVal = analogRead(xPin); // read from the x-axis pin 
  yVal = analogRead(yPin); // read from the y-axis pin
 
 if(xVal > 520 && xVal < 1000){ // if the stick is pulled to the right but not clicked
  if(pos != 3){
    if(redVals[pos] == 255){
      directions[pos][0] = false;
    }
    if(redVals[pos] == 0){
      directions[pos][0] = true;
    }
    if (directions[pos][0]){
      redVals[pos] = redVals[pos] + 1;
    } else {
      redVals[pos] = redVals[pos] - 1;
    }

    if(greenVals[pos] == 255){
      directions[pos][1] = false;
    }
    if(greenVals[pos] == 0){
      directions[pos][1] = true;
    }
    if (directions[pos][1]){
      greenVals[pos] = greenVals[pos] + 1;
    } else {
      greenVals[pos] = greenVals[pos] - 1;
    }

    if(blueVals[pos] == 255){
      directions[pos][2] = false;
    }
    if(redVals[pos] == 0){
      directions[pos][2] = true;
    }
    if (directions[pos][2]){
      blueVals[pos] = blueVals[pos] + 1;
    } else {
      blueVals[pos] = blueVals[pos] - 1;
    }
  }
 } 

 if(xVal < 500){ // if the stick is pulled to the left
  if(pos != 3){
    if(redVals[pos] == 255){
      directions[pos][0] = false;
    }
    if(redVals[pos] == 0){
      directions[pos][0] = true;
    }
    if (directions[pos][0]){
      redVals[pos] = redVals[pos] + 1;
    } else {
      redVals[pos] = redVals[pos] - 1;
    }

    if(greenVals[pos] == 255){
      directions[pos][1] = false;
    }
    if(greenVals[pos] == 0){
      directions[pos][1] = true;
    }
    if (directions[pos][1]){
      greenVals[pos] = greenVals[pos] + 1;
    } else {
      greenVals[pos] = greenVals[pos] - 1;
    }

    if(blueVals[pos] == 255){
      directions[pos][2] = false;
    }
    if(redVals[pos] == 0){
      directions[pos][2] = true;
    }
    if (directions[pos][2]){
      blueVals[pos] = blueVals[pos] + 1;
    } else {
      blueVals[pos] = blueVals[pos] - 1;
    }
  }
 }

 if(yVal > 520){ // if the stick is pulled up
  if(pos != 3){
     if(brightnesses[pos] < 0.99){
      tempFloat = brightnesses[pos];
      brightnesses[pos] = tempFloat + 0.01;
    }
  }
 }

 if(yVal < 500){ // if the stick is pulled down
  if(pos != 3){
    if(brightnesses[pos] > 0.01){
      tempFloat = brightnesses[pos];
      brightnesses[pos] = tempFloat - 0.01;
    }
  }
 }

 if(xVal > 1000){ // if the button is clicked
  pos = (pos + 1) % 4;
  Serial.println(pos);
  delay(100); // when clicked the button sets xVal above 1000 for several cycles; this delay ensures that a single button press doesn't cause the position to shift more than once
 }

 // 

  // Write to LEDs
  for(int i = 0; i < 3; i++){
    leds.setColorRGB(i, (int)redVals[i]*brightnesses[i], (int)greenVals[i]*brightnesses[i], (int)blueVals[i]*brightnesses[i]);
  }
}
