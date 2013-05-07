#include "cmath.h"

double c_abs(double x) {
  if (x<0) {
    return -x;
  }
  return x;
}

double c_fmod(double x, double y) {
  if(y < 0) {
    y = -y;
    x = -x;
  }
  if(x < 0) {
    x = x + (-y * ((int)(x / y) - 1));
  }
  return x - (int)(x / y) * y;
}

double deg2rad(double degrees) {
    /* Convert degrees to radians. */
    /* Input in [-180,360]. */
    if(degrees > 180) {
        degrees = degrees - 360;
    }
    return (degrees * C_PI / 180);
}
