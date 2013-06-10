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
int steps_taken(double value, double origin, double destination, int total_steps) {
    double distance_covered = value - origin;
    double total_distance = destination - origin;
    double fract_covered = distance_covered / total_distance;
    return (int)(fract_covered * total_steps);
}

char *itoap(int num)
{
static char buff[20] = {};
int i = 0, temp_num = num, length = 0;
char *string = buff;
if(num >= 0) {
// count how many characters in the number
while(temp_num) {
temp_num /= 10;
length++;
}
// assign the number to the buffer starting at the end of the 
// number and going to the begining since we are doing the
// integer to character conversion on the last number in the
// sequence
for(i = 0; i < length; i++) {
buff[(length-1)-i] = '0' + (num % 10);
num /= 10;
}
buff[i] = '\0'; // can't forget the null byte to properly end our string
}
else
return "Unsupported Number";
return string;
}