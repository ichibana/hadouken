#include <Adafruit_NeoPixel.h>

#define NEOPIN 4
#define MODEPIN 0

static byte const PIXELS = 14;
static byte const MIDPIXEL = PIXELS / 2;

double bz[PIXELS];

Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, NEOPIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  strip.begin();
  strip.show(); // Initialize all PIXELS to 'off'
  bz_mod(0.45);
  pulse();
  for (int i = 0 ; i < 360 ; i++) {
    if (i == 230) {
      for (int j = 90 ; j < 130 ; j++) {
        tilt(i, j);
      }
      for (int j = 130; j > 90; j--) {
        tilt(i, j);
      }
    }
    else {
      tilt(i, 90);
    }
  }
  delay(15);
}


void loop()
{

}

void bz_mod(double decay_rate) {
  bz[0] = 1;
  for (int i = 1 ; i < PIXELS ; i++) {
    bz[i] = decay_rate;
    for (int j = 0 ; j < i; j++) {
      bz[i] *= decay_rate;
    }
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

//Tilt mode
void tilt(int degrees_yz, int degrees_xz) {
  bz_mod(0.85);
  uint32_t c;
  int min_light = 3;
  float theta_yz = float(degrees_yz) * PI / 180;

  int boundary = max(min_light, PIXELS * abs(sin(theta_yz)));
  for (int i = 0 ; i < boundary ; i++) {
    c = Wheel(degrees_xz, 100 * bz[boundary - i]);
    strip.setPixelColor(i, c);
  }
  for (int i = boundary ; i < PIXELS ; i++) {
    strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show();
}

uint32_t Wheel(int WheelPos, double brightness) {
  WheelPos = 255 - WheelPos;
  brightness = brightness / 255;
  if (WheelPos < 85) {
    return strip.Color((255 - WheelPos * 3) * brightness, 0, (WheelPos * 3) * brightness);
  }
  else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, (WheelPos * 3) * brightness, (255 - WheelPos * 3) * brightness);
  }
  else {
    WheelPos -= 170;
    return strip.Color((WheelPos * 3) * brightness, (255 - WheelPos * 3) * brightness, 0);
  }
}


