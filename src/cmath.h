#ifndef CMATH_H_ADDED
#define CMATH_H_ADDED

#define C_PI 3.1415926535897932384626433832795028841971693993751058
double c_abs(double x);
double c_fmod(double x, double y);
double deg2rad(double degrees);
int steps_taken(double value, double origin, double destination, int total_steps);
char *itoap(int num);
#endif