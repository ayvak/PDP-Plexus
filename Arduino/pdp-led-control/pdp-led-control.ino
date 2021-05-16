#include <FastLED.h>
#include "SerialTransfer.h"

SerialTransfer myTransfer;

#define PIN_INPUT 6
#define PIN_HIDDEN 7
#define PIN_OUTPUT 8
#define LAYER_SIZE 28
#define NUM_LEDS LAYER_SIZE * LAYER_SIZE

int brightness = 60;

CRGB _input_LED[NUM_LEDS];
CRGB _hidden_LED[NUM_LEDS];
CRGB _output_LED[NUM_LEDS];

#define HIDDEN_ACTUAL_SIZE 12
#define DEBUG 1

#define BATCH_SIZE LAYER_SIZE
#define BATCHES LAYER_SIZE
#define ARRAY_SIZE BATCH_SIZE * BATCHES

// PATTERN

#define ENABLE_PATTERN 1
#define TIMEOUT 20000
const bool    kMatrixSerpentineLayout = true;

// Define arrays
int _input[ARRAY_SIZE], _hidden[ARRAY_SIZE], _output[ARRAY_SIZE];
int flag = -1;
int in_ind = 0, hid_ind = 0, out_ind = 0; // indeices for each layer array


void initLEDMatrix(CRGB leds[NUM_LEDS])
{
  fill_solid( leds, NUM_LEDS, CRGB::Red);
  FastLED.delay(200);

  fill_solid( leds, NUM_LEDS, CRGB::Black);
  FastLED.delay(200);

  fill_solid( leds, NUM_LEDS, CRGB::Blue);
  FastLED.delay(200);

  fill_solid( leds, NUM_LEDS, CRGB::Black);
  FastLED.delay(200);

  fill_solid( leds, NUM_LEDS, CRGB::Green);
  FastLED.delay(200);

  fill_solid( leds, NUM_LEDS, CRGB::Black);
  FastLED.delay(200);
    
  fill_solid( leds, NUM_LEDS, CRGB::Yellow);
  FastLED.delay(200);

  fill_solid( leds, NUM_LEDS, CRGB::Black);
  FastLED.delay(200);
}


void setInputLEDs(int input_layer[NUM_LEDS])
{
  int r, c, ind;
  CRGB row[NUM_LEDS];
  for(int i = 0; i < NUM_LEDS; i++){
    r = i / LAYER_SIZE;
    c = i % LAYER_SIZE;

    row[r * LAYER_SIZE + c] = CHSV( 200, 255, input_layer[i]);
     
    // Assign it to actual hidden layer
    if (!(r % 2)) // even rows
      ind = r * LAYER_SIZE + c;
    else
      ind = (r+1) * LAYER_SIZE - c - 1;
      
    _input_LED[ind] = row[r * LAYER_SIZE + c];
//    _input_LED[i] = ;
  }
}


void setHiddenLEDs(int hidden_layer[NUM_LEDS])
{
  /* 
  input - all pixel values in row-wise in a single array.
  if it is considered as a matrix of size LAYER_SIZE x LAYER_SIZE, the grid
  on the bottom right corner of size HIDDEN_ACTUAL_SIZE x HIDDEN_ACTUAL_SIZE
  has the actual values of the hidden layer. Other areas of the matrix given
  as input has the values which represents colours for each dummy layer 

  colour codes.
    0 - no colour 
    1 - colour for dummy layer 1
    2 - colour for dummy layer 2
    3 - colour for dummy layer 3
    4 - colour for dummy layer 4

  The actual hidden layer area shoudl have a background colour (not black) so
  that is it easily visible

  */

  int r, c, ind;
  CRGB row[NUM_LEDS], colour;
  for(int i = 0; i < NUM_LEDS; i++)
  {
    r = i / LAYER_SIZE;
    c = i % LAYER_SIZE;
    
    // rows of extra/dummy layers
    if ((r < LAYER_SIZE - HIDDEN_ACTUAL_SIZE) || (c < LAYER_SIZE - HIDDEN_ACTUAL_SIZE))
    {
      // assign colours
      switch (hidden_layer[i]) // change to HSV if needed
      {
      case 4:
        colour = CHSV( 250, 255, 100);
        break;

      case 3:
        colour = CHSV( 200, 255, 100);
        break;

      case 2:
        colour = CHSV( 100, 255, 70);
        break;

      case 1:
        colour = CHSV( 150, 255, 100);
        break;

      case 0:
        colour = CRGB::Black; // CHSV( 100, 10, 30); // background colour 
        break;
      
      default:
        break;
      }
      row[r * LAYER_SIZE + c] = colour; 
    }
    else
    {
      // give a base colour - for values less than 100
      hidden_layer[i] = hidden_layer[i] > 255 ? 255 : hidden_layer[i];
      colour = (hidden_layer[i] < 50) ? CHSV( 70, 200, 80) : CHSV( 200, 255, hidden_layer[i]);
//      row[r * LAYER_SIZE + c] = CHSV( 255, 255, hidden_layer[i]);
      row[r * LAYER_SIZE + c] = colour;
    }

    // Assign it to actual hidden layer
    if (!(r % 2)) // even rows
      ind = r * LAYER_SIZE + c;
    else
      ind = (r+1) * LAYER_SIZE - c - 1;
      
    _hidden_LED[ind] = row[r * LAYER_SIZE + c];
  }
//  FastLED.show();
}

void setOutputLEDs(int output_layer[NUM_LEDS])
{
  /* 
  input - all pixel values in row-wise in a single array.
  colours are encoded with integers.
    0 - no colour (or keep a base colour as bg)
    1 - colour for highest probable number 
    2 - colour for second highest probable number
    3 - colour for third highest probable number
  */

  int r, c, ind;
  CRGB row[NUM_LEDS], colour;
  for(int i = 0; i < NUM_LEDS; i++)
  {
    r = i / LAYER_SIZE;
    c = i % LAYER_SIZE;

    switch ((output_layer[i]))
    {
    case 3: 
        colour = CHSV( 200, 255, 200);
        break;

    case 2:
      colour = CHSV( 150, 255, 200);
      break;

    case 1:
      colour = CHSV( 100, 255, 200);
      break;

    case 0:
      colour = CRGB::Black; // CHSV( 100, 20, 50); // background colour
      break;
    
    default:
      break;
    }

    // 
    row[r * LAYER_SIZE + c] = colour; 

    // Assign it to actual output layer
    if (!(r % 2)) // even rows
      ind = r * LAYER_SIZE + c;
    else
      ind = (r+1) * LAYER_SIZE - c - 1;
      
    _output_LED[ind] = row[r * LAYER_SIZE + c];

  }
}

////////////////////////////////////////////////////////////////////////////////////


void get_layer_values()
{
  char c;
  int input_array[BATCH_SIZE];
  if(myTransfer.available())
  {
    int n = 0, index = 0;
    for(uint16_t i=0; i < myTransfer.bytesRead; i++){ //Getting all the numbers in the string received.
      c = myTransfer.packet.rxBuff[i];
//      Serial.println(c);

      switch(c)
      {
        case 'i':
          flag = 0;
          break;
          
        case 'h':
          flag = 1;
          break;
          
        case 'o':
          flag = 2;
          break;

         default:
          break;
      }

      if(flag == -1)
        break;
      
      if(c != '-') // '-' is the separator between the numbers
      {
//        Serial.println(int(c));
        n = n * 10 + c - 48; // 0 -> 48 in ASCII; the numbers are received byte by byte.
      }
      else
      {
        if(flag == 0){
//          Serial.println("adding to input");
          _input[in_ind++] = n;
          if(in_ind >= ARRAY_SIZE)
          {
            in_ind = 0;
            flag = -1;
            setInputLEDs(_input);
            if(DEBUG == 1){
              Serial.println("input added");
              for(int j = 0; j<ARRAY_SIZE; j++){
                Serial.print(_input[j]);
                Serial.print("-");
              }
            }
          }
        }
          
        else if(flag == 1){
//          Serial.println("adding to hidden");
          _hidden[hid_ind++] = n;
          if(hid_ind >= ARRAY_SIZE)
          {
            hid_ind = 0;
            flag = -1;
            if(DEBUG == 1){
              Serial.println("hidden added");
              for(int j = 0; j<ARRAY_SIZE; j++){
                Serial.print(_hidden[j]);
                Serial.print("-");
              }
            }
          }
        }
        
         else if(flag == 2) {
//          Serial.println("adding to output");
          _output[out_ind++] = n;
          if(out_ind >= ARRAY_SIZE)
          {
            out_ind = 0;
            flag = -1;
            if(DEBUG == 1){
              Serial.println("output added");
              for(int j = 0; j<ARRAY_SIZE; j++){
                Serial.print(_output[j]);
                Serial.print("-");
              }
            }
          }
         }
        n = 0;
      }
    }
  }
  
}


void setup() {
  // Initialise serial
  Serial.begin(115200);
  Serial.setTimeout(1);
  myTransfer.begin(Serial);

  //////////////////////////////////////////
  delay(1000);

  // Add LEDs  
  FastLED.addLeds<1, WS2812B, PIN_INPUT, GRB>(_input_LED, NUM_LEDS);
  FastLED.addLeds<1, WS2812B, PIN_HIDDEN, GRB>(_hidden_LED, NUM_LEDS);
  FastLED.addLeds<1, WS2812B, PIN_OUTPUT, GRB>(_output_LED, NUM_LEDS);

  FastLED.setBrightness(brightness);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1500);
  // set_max_power_indicator_LED(13);

  fill_solid(_input_LED, NUM_LEDS, CRGB::Black);
  fill_solid(_hidden_LED, NUM_LEDS, CRGB::Black);
  fill_solid(_output_LED, NUM_LEDS, CRGB::Black);
  FastLED.show();

  initLEDMatrix(_input_LED);
  initLEDMatrix(_hidden_LED);
  initLEDMatrix(_output_LED);

  fill_solid(_input_LED, NUM_LEDS, CRGB::White);
  fill_solid(_hidden_LED, NUM_LEDS, CRGB::White);
  fill_solid(_output_LED, NUM_LEDS, CRGB::White);
  FastLED.delay(500);

  fill_solid(_input_LED, NUM_LEDS, CRGB::Black);
  fill_solid(_hidden_LED, NUM_LEDS, CRGB::Black);
  fill_solid(_output_LED, NUM_LEDS, CRGB::Black);

  Serial.println("setup ends");

}

int check_sum()
{
  /**
   * return sum of input layer
   */

  int sum = 0;
  for(int i=0; i<ARRAY_SIZE; i++)
    sum += _input[i];

  return sum;
}

//////////////////////////////////////////////////////////////////////////
// THE RAINBOW PATTERN

void DrawOneFrame( byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8)
{
  byte lineStartHue = startHue8;
  for( byte y = 0; y < LAYER_SIZE; y++) {
    lineStartHue += yHueDelta8;
    byte pixelHue = lineStartHue;      
    for( byte x = 0; x < LAYER_SIZE; x++) {
      pixelHue += xHueDelta8;
      _input_LED[ XY(x, y)]  = CHSV( pixelHue, 255, 255);
      _hidden_LED[ XY(x, y)]  = CHSV( pixelHue, 255, 255);
      _output_LED[ XY(x, y)]  = CHSV( pixelHue, 255, 255);
    }
  }
}

uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;

  // to incorporate the mess we made. First LED at bottom left (facepalm)
  y = LAYER_SIZE - 1 - y;
  
  if( kMatrixSerpentineLayout == false) {
    i = (y * LAYER_SIZE) + x;
  } else {
    if( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (LAYER_SIZE - 1) - x;
      i = (y * LAYER_SIZE) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * LAYER_SIZE) + x;
    }
  }
  
  return i;
}

void show_rainbow()
{
  uint32_t ms = millis();
  int32_t yHueDelta32 = ((int32_t)cos16( ms * 27 ) * (350 / LAYER_SIZE));
  int32_t xHueDelta32 = ((int32_t)cos16( ms * 39 ) * (310 / LAYER_SIZE));
  DrawOneFrame( ms / 65536, yHueDelta32 / 32768, xHueDelta32 / 32768);
  FastLED.show();
}

//////////////////////////////////////////////////////////////////////////

int prev_time = millis();
int prev_sum = 0;
int new_input = 1; // to check if the input from the user is new

void loop() {

  get_layer_values();

  // check time difference
  int curr_time = millis();
  int td = curr_time - prev_time;
  
Serial.println(td);
  
  int curr_sum = check_sum();  

  if(curr_sum != 0) // if there is an input from the user
  {
    if(curr_sum == prev_sum && td > TIMEOUT) // check if the input hasn't changed for TIMEOUT secs
    {
      // show rainbow pattern
      Serial.println("rainbow");
      show_rainbow();
      new_input = 1;
    }
    else
    {
      if(DEBUG == 1){
        for(int j = 0; j<ARRAY_SIZE; j++){
          Serial.print(_input[j]);
          Serial.print("-");
          Serial.print(_hidden[j]);
          Serial.print("-");
          Serial.println(_output[j]);
        }
      }

      if(new_input)
      {
        // set prev time
        prev_time = curr_time;
        new_input = 0;
      }
      setHiddenLEDs(_hidden);
      setInputLEDs(_input);
      setOutputLEDs(_output);
      
      FastLED.show();
    }
  }
  else if(ENABLE_PATTERN) // no input from user and pattern is enabled
  {
    show_rainbow();
  }
  else // no input from use and no pattern
  {
    // set all LEDs to zero
    fill_solid(_input_LED, NUM_LEDS, CRGB::Black);
    fill_solid(_hidden_LED, NUM_LEDS, CRGB::Black);
    fill_solid(_output_LED, NUM_LEDS, CRGB::Black);
    FastLED.show();
  }
  prev_sum = curr_sum;
}
