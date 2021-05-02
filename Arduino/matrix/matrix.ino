#include <FastLED.h>

#define PIN_INPUT 6
#define PIN_HIDDEN 7
#define PIN_OUTPUT 8
#define LAYER_SIZE 20
#define NUM_LEDS LAYER_SIZE * LAYER_SIZE

int brightness = 60;

CRGB _input[NUM_LEDS];
CRGB _hidden[NUM_LEDS];
CRGB _output[NUM_LEDS];

#define HIDDEN_ACTUAL_SIZE 12

int hidden_layer_20x20[NUM_LEDS] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 3, 3, 3, 3, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 4, 4, 4, 4, 0, 0, 0, 8, 127, 255, 129, 0, 0, 0, 0, 0, 3, 3, 3, 3, 4, 4, 4, 4, 0, 0, 0, 11, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 3, 33, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 2, 16, 0, 12, 129, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 13, 137, 242, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 23, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 83, 74, 34, 66, 47, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 192, 62, 52, 100, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

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

void setHiddenLEDs(int hidden_layer[NUM_LEDS])
{
  int r, c, ind;
  CRGB row[NUM_LEDS], colour;
  for(int i = 0; i < NUM_LEDS; i++)
  {
    r = i / LAYER_SIZE;
    c = i % LAYER_SIZE;

    Serial.print(r);
    Serial.print("\t");
    Serial.print(c);
    Serial.print("\t");
    
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
        colour = CHSV( 150, 255, 100);
        break;

      case 1:
        colour = CHSV( 100, 255, 100);
        break;

      case 0:
        colour = CRGB::Black;
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
      
    Serial.println(ind);
    _hidden[ind] = row[r * LAYER_SIZE + c];
  }
//  FastLED.show();
}

void setup() {
//  Serial.begin(9600);
  delay(1000);

  // Add LEDs  
  FastLED.addLeds<1, WS2812B, PIN_INPUT, GRB>(_input, NUM_LEDS);
  FastLED.addLeds<1, WS2812B, PIN_HIDDEN, GRB>(_hidden, NUM_LEDS);
  FastLED.addLeds<1, WS2812B, PIN_OUTPUT, GRB>(_output, NUM_LEDS);

  FastLED.setBrightness(brightness);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1500);
  // set_max_power_indicator_LED(13);

  fill_solid(_input, NUM_LEDS, CRGB::Black);
  fill_solid(_hidden, NUM_LEDS, CRGB::Black);
  fill_solid(_output, NUM_LEDS, CRGB::Black);
  FastLED.show();

  initLEDMatrix(_input);
  initLEDMatrix(_hidden);
  initLEDMatrix(_output);

  fill_solid(_input, NUM_LEDS, CRGB::White);
  fill_solid(_hidden, NUM_LEDS, CRGB::White);
  fill_solid(_output, NUM_LEDS, CRGB::White);
  FastLED.delay(1000);

  fill_solid(_input, NUM_LEDS, CRGB::Black);
  fill_solid(_hidden, NUM_LEDS, CRGB::Black);
  fill_solid(_output, NUM_LEDS, CRGB::Black);
//  FastLED.delay(1000);


//  setHiddenLEDs(hidden_layer_20x20);
//  FastLED.show();
}

void loop() {
  // Update matrix values

  // input layer

  // hidden layer - Adjust colours
  setHiddenLEDs(hidden_layer_20x20);

  // output layer - Adjust colours

  // show everything
  FastLED.delay(50);
}
