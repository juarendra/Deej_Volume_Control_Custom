//#include <FastLED.h>

#define NUM_LEDS 5
#define LED_PIN  PB10

//CRGB leds[NUM_LEDS];
const int NUM_SLIDERS = 5;
const int analogInputs[NUM_SLIDERS] = {PA0, PA1, PA2, PA3, PA4}; // analog Inputs for sliders/ potentiometers
const int digitalInputs[NUM_SLIDERS] = {PA5, PA6, PA7, PB0, PB1}; // digital Inputs for mute buttons


int analogSliderValues[NUM_SLIDERS];
bool mutedSliders[NUM_SLIDERS];
int mutedTimeout[NUM_SLIDERS];

void setup() {

  //FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  //FastLED.setBrightness(255);
  
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
    pinMode(digitalInputs[i], INPUT_PULLUP);
  }

  Serial.begin(9600);
}

void loop() {

  updateSliderValues();
  sendSliderValues(); // Actually send data (all the time)
  //LEDMuteState();
  // printSliderValues(); // For debug
  delay(10);
}

void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    analogSliderValues[i] = analogRead(analogInputs[i]);
    if (digitalRead(digitalInputs[i]) == HIGH) {
      if (mutedTimeout[i] < 20) {
        mutedTimeout[i]++;
      }
    } else if (mutedTimeout[i] > 10) {
      mutedSliders[i] = !mutedSliders[i];
      mutedTimeout[i] = 0;
    }
  }
}

void sendSliderValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    int val = 0;
    if (!mutedSliders[i]) {
      val = (int)analogSliderValues[i];
    }
    builtString += String(val);

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }
  }

  Serial.println(builtString);
}

void printSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    String printedString = String("Slider #") + String(i + 1) + String(": ") + String(analogSliderValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_SLIDERS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}
// void LEDMuteState() {

//   for (int i = 0; i < NUM_SLIDERS; i++) {
//     if (analogSliderValues[i]<= 1 || !mutedSliders[i] == 0) {
//       //leds[i] = CRGB::Red;
//       //FastLED.show();
//     }
//     else  {
//       //leds[i] = CRGB::Green;
//       //FastLED.show();
//     }
    
    
//   }
// }
