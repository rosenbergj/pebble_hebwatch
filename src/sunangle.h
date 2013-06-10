#ifndef SUNANGLE_H_ADDED
#define SUNANGLE_H_ADDED
#include <stdio.h>
#include "cordic.h"

#define CORDIC_SIZE 30

double fractmod(double x, double y);
int day_of_year(int year, int month, int day);
double julian_day(int year, int month, int day, float utchour);
double julian_century(int year, int month, int day, float hour);
void jc2ymdh(double jc, int tz, int* year, int* month, int* day, float* hour);
double time_of_elevation(double jc, int tz, float longitude, float latitude, float elev_angle, int pm);
/*double elevation_angle(double jc, int tz, float longitude, float latitude);*/


#endif