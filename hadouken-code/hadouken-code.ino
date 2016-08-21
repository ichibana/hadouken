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
    tilt(i);
    delay(50);
  }
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
void tilt(int degrees_yz) {
  int min_light = 3;
  float theta_yz = float(degrees_yz) * PI / 180;

  int boundary = max(min_light, PIXELS * abs(sin(theta_yz)));
  if (theta_yz >= 0 && theta_yz < PI) {
    for (int i = 0 ; i < boundary ; i++) {
      strip.setPixelColor(i, 0, 0, 100);
    }
    for (int i = boundary ; i < PIXELS ; i++) {
      strip.setPixelColor(i, 0, 0, 0);
    }
  }
  else if (theta_yz > PI && theta_yz < 2 * PI) {
    for (int i = (PIXELS - 1) ; i > (PIXELS - boundary) ; i--) {
      strip.setPixelColor(i, 0, 0, 100);
    }
    for (int i = (PIXELS - boundary) ; i >= 0; i--  ) {
      strip.setPixelColor(i, 0, 0, 0);
    }
  }
  strip.show();

}




