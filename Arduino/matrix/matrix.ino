#include <FastLED.h>
#include "SerialTransfer.h"

SerialTransfer myTransfer;

#define PIN_INPUT           6
#define PIN_HIDDEN          7
#define PIN_OUTPUT          8
#define NETWORK_LAYER_SIZE  10 // size of each layer in the network. All 3 layers have the same size
#define SCALE_FACTOR        2 // multiplication factor - for specifying the size of the physical layer
#define LAYER_SIZE          NETWORK_LAYER_SIZE * SCALE_FACTOR // size of physical layer
#define NUM_LEDS            LAYER_SIZE * LAYER_SIZE
#define PIXELS              NETWORK_LAYER_SIZE * NETWORK_LAYER_SIZE

int brightness = 60;

CRGB _input_LED[NUM_LEDS];
CRGB _hidden_LED[NUM_LEDS];
CRGB _output_LED[NUM_LEDS];

#define HIDDEN_ACTUAL_SIZE 12

int down_size_10x10[PIXELS] = {0,   0,   0, 200,   0,   0,   0,   0,   0,   0,   0,   0,   0, 200,   0,   0,   0,   0,   0,   0,   0,   0,   0, 200,   0,   0,   0,   0,   0,   0,   0,   0,   0, 200,   0,   0,   0,   0,   0,   0,   0,   0,   0, 200,   0,   0,   0,   0,   0,   0,   0,   0,   0, 200,   0,   0,   0,   0,   0,   0, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200,   0,   0,   0, 200,   0,   0,   0,   0,   0,   0,   0,   0,   0, 200,   0,   0,   0,   0,   0,   0,   0, 0,   0, 200,   0,   0,   0,   0,   0,   0};

//int hidden_layer_20x20[PIXELS] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 3, 3, 3, 3, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 4, 4, 4, 4, 0, 0, 0, 8, 127, 255, 129, 0, 0, 0, 0, 0, 3, 3, 3, 3, 4, 4, 4, 4, 0, 0, 0, 11, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 3, 33, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 2, 16, 0, 12, 129, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 13, 137, 242, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 23, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 83, 74, 34, 66, 47, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 192, 62, 52, 100, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//int output_layer_20x20[PIXELS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//int input_layer_28x28[PIXELS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 33, 62, 58, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35, 246, 245, 121, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 89, 232, 254, 133, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 42, 217, 254, 193, 37, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 90, 250, 242, 114, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 27, 180, 254, 216, 12, 0, 0, 0, 0, 0, 0, 0, 0, 12, 112, 128, 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 65, 247, 230, 42, 0, 0, 0, 0, 0, 0, 0, 0, 0, 107, 254, 229, 83, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 22, 175, 253, 217, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, 234, 249, 71, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 70, 248, 233, 45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 95, 250, 240, 62, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 158, 255, 216, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, 233, 248, 68, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 108, 252, 247, 160, 108, 101, 100, 99, 100, 100, 99, 98, 100, 131, 190, 241, 255, 218, 8, 0, 0, 0, 0, 0, 0, 0, 0, 11, 205, 240, 247, 253, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 216, 3, 0, 0, 0, 0, 0, 0, 0, 0, 17, 235, 255, 209, 173, 190, 193, 193, 191, 192, 192, 190, 190, 191, 183, 141, 103, 80, 37, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 12, 14, 14, 14, 14, 14, 14, 14, 15, 14, 17, 126, 253, 236, 18, 0, 0, 0, 0, 0, 0, 0, 0, 49, 251, 254, 136, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 151, 255, 255, 97, 0, 0, 0, 0, 0, 0, 0, 0, 142, 255, 255, 154, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 154, 255, 255, 163, 0, 0, 0, 0, 0, 0, 0, 0, 176, 255, 255, 154, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 154, 255, 230, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 162, 253, 143, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 31, 62, 34, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//int hidden_layer_28x28[PIXELS] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 4, 4, 4, 4, 0, 0, 0, 0, 52, 192, 167, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 4, 4, 4, 4, 0, 0, 1, 107, 254, 119, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 4, 4, 4, 4, 0, 8, 164, 246, 0, 0, 0, 0, 20, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 56, 137, 0, 0, 0, 0, 0, 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 12, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 18, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 19, 12, 0, 0, 0
//};
//int output_layer_28x28[PIXELS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//};

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

void setInputLEDs(int input_layer[PIXELS])
{
  Serial.println("input layer fix");
  int r, c, ind, _r, _c;
  CRGB val;
  CRGB row[NUM_LEDS];
  for(int i = 0; i < NUM_LEDS; i++){
    r = i / LAYER_SIZE;
    c = i % LAYER_SIZE;

    // corresponding row and column in the network layer
    _r = r / SCALE_FACTOR;
    _c = c / SCALE_FACTOR;

     row[r * LAYER_SIZE + c] = CHSV( 200, 255, input_layer[_r * NETWORK_LAYER_SIZE + _c]);
//    val = CHSV( 200, 255, input_layer[_r * NETWORK_LAYER_SIZE + _c]);
     
    // Assign it to actual hidden layer
    if (!(r % 2)) // even rows
      ind = r * LAYER_SIZE + c;
    else
      ind = (r+1) * LAYER_SIZE - c - 1;
      
//    _input_LED[ind] = val; // 
    _input_LED[ind] = row[r * LAYER_SIZE + c];
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

void loop() {
  // Update matrix values

  // input layer
  setInputLEDs(down_size_10x10);

  // hidden layer - Adjust colours
//  setHiddenLEDs(down_size_10x10);

  // output layer - Adjust colours
//  setOutputLEDs(down_size_10x10);

  // show everything
  FastLED.show();
}
