#include <Wire.h>
#include <Adafruit_VEML7700.h>
#include <Adafruit_NeoPixel.h>

#define SDA_PIN 8
#define SCL_PIN 9
#define LED_PIN 3
#define NUM_LEDS 18

#define LUX_OFF_THRESHOLD 30.0
#define LUX_MAX 600.0

#define MIN_PERCENT 25.0
#define MAX_PERCENT 80.0

#define SAMPLE_INTERVAL 200
#define CONTROL_INTERVAL 2000
#define OFF_DELAY 3000
#define MAX_STEP 8   // etwas schneller anpassen

Adafruit_VEML7700 veml = Adafruit_VEML7700();
Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

float luxBuffer[25];
int luxIndex = 0;
bool bufferFilled = false;

float avgLux = 0;
int brightness = 0;

unsigned long lastSample = 0;
unsigned long lastControl = 0;
unsigned long belowThresholdSince = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  if (!veml.begin()) {
    Serial.println("VEML7700 nicht gefunden!");
    while (1);
  }

  pixels.begin();
  pixels.clear();
  pixels.show();

  Serial.println("System gestartet");
}

void loop() {

  unsigned long now = millis();

  // ===== MESSUNG =====
  if (now - lastSample >= SAMPLE_INTERVAL) {
    lastSample = now;

    float lux = veml.readLux();

    luxBuffer[luxIndex++] = lux;
    if (luxIndex >= 25) {
      luxIndex = 0;
      bufferFilled = true;
    }

    int count = bufferFilled ? 25 : luxIndex;
    float sum = 0;
    for (int i = 0; i < count; i++) sum += luxBuffer[i];
    avgLux = sum / count;

    Serial.print("Lux avg: ");
    Serial.print(avgLux, 2);
    Serial.print(" | Brightness: ");
    Serial.println(brightness);
  }

// ===== REGELUNG =====
if (now - lastControl >= CONTROL_INTERVAL) {
  lastControl = now;

  int targetBrightness = 0;

  if (avgLux <= LUX_OFF_THRESHOLD) {

    if (belowThresholdSince == 0)
      belowThresholdSince = now;

    if (now - belowThresholdSince >= OFF_DELAY)
      targetBrightness = 0;

  } else {

    belowThresholdSince = 0;

    if (avgLux > LUX_MAX)
      avgLux = LUX_MAX;

    float percent = MAX_PERCENT -
      ((avgLux - LUX_OFF_THRESHOLD) /
      (LUX_MAX - LUX_OFF_THRESHOLD)) *
      (MAX_PERCENT - MIN_PERCENT);

    targetBrightness = (percent / 100.0) * 255.0;
  }

  // weiche Anpassung
  if (targetBrightness > brightness)
    brightness += min(MAX_STEP, targetBrightness - brightness);
  else
    brightness -= min(MAX_STEP, brightness - targetBrightness);

  updateLEDs();
}
}

void updateLEDs() {
  for (int i = 0; i < NUM_LEDS; i++) {
    pixels.setPixelColor(i, pixels.Color(brightness, brightness, brightness));
  }
  pixels.show();
}