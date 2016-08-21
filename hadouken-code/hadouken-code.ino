#include <Adafruit_NeoPixel.h>

#define NEOPIN 4
#define MODEPIN 0

static byte const PIXELS = 14;
static byte const MIDPIXEL = PIXELS / 2;
static byte mode;

double bz[PIXELS];

Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, NEOPIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  strip.begin();
  strip.show(); // Initialize all PIXELS to 'off'
  bz_mod(0.45);
  pulse();
}

void loop()
{
  safety(3);
}

void bz_mod(double decay_rate) {
  for (int i = 0 ; i < PIXELS ; i++) {
    bz[i] = pow(decay_rate, i);
  }
}

//Pulse a "bead" of light from beginning to end of strip and back again.
void pulse() {
  int hold = 90;
  int buildup = hold / PIXELS;
  for (int j = 0; j < PIXELS; j++) {
    for (int k = 0; k < PIXELS ; k++) {
      strip.setPixelColor(k, 100 * bz[abs(k - j)], 0, 0);
    }
    strip.show();
    delay(buildup + j * buildup);
  }
  delay(hold);
  for (int j = PIXELS; j >= -PIXELS; j--) {
    for (int k = 0; k < PIXELS ; k++) {
      strip.setPixelColor(k, 100 * bz[min(abs(k - j), (PIXELS - 1))], 0, 0);
    }
    strip.show();
    delay(buildup + min(abs(j), PIXELS) * buildup);
  }
}

//Safety mode
void safety(int n) {
  int max_brightness = 70;
  int delay_time = 10;
  while (1) {
    for (int i = 1; i <= 50 ; i++) {
      for (int j = 1; j < PIXELS ; j += PIXELS / n) {
        strip.setPixelColor(j, max_brightness / i, 0, 0);
      }
      strip.show();
      delay(delay_time);
    }
    for (int i = 50; i >= 1 ; i--) {
      for (int j = 1; j < PIXELS ; j += PIXELS / n) {
        strip.setPixelColor(j, max_brightness / i, 0, 0);
      }
      strip.show();
      delay(delay_time);
    }
  }
}








