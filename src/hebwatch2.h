#ifndef HEBWATCH2_H_ADDED
#define HEBWATCH2_H_ADDED
#include <stdlib.h>
#include <string.h>
#include "cordic.h"
#include "sunangle.h"
#include "cmath.h"
#include "hdate.h"
#include "config.h"

#define CHECK_ARCTIC_ERROR 1

extern int arctic; /* 0 normally, 1 if sun doesn't set, -1 if sun doesn't rise */
extern double jc_sunset_y;
extern double jc_dawn;
extern double jc_sunrise;
extern double jc_sunset;
extern double jc_tzeit;
extern double jc_sunrise_t;
extern double hour_count_start;
extern double hour_count_end;


void init_sunset_y(double jc, int tz, float longitude, float latitude);
void init_dawn(double jc, int tz, float longitude, float latitude);
void init_sunrise(double jc, int tz, float longitude, float latitude);
void init_sunset(double jc, int tz, float longitude, float latitude);
void init_tzeit(double jc, int tz, float longitude, float latitude);
void init_sunrise_t(double jc, int tz, float longitude, float latitude);

/* Initializes the global jc_* caches for the specified jc.
    If force == 0, caches will be left alone if they're on the correct days.
    If force == 1, caches will all be recalculated.
    Return value indicates how many were recalculated.
*/
int init_sunrises_sunsets(double jc, int tz, float longitude, float latitude, int force);

/* Return an integer indicating what "part" of the day a specified time is.
        0: nighttime, pre-dawn
        1: between dawn and sunrise
        2: daytime
        3: between sunset and nightfall
        4: after nightfall
*/
int part_of_day(double jc);

void init_hour_count_start_end(double jc);
void hour_chelek_string(double jc, double startjc, double endjc, char* time_string);
void h_m_chelek_string(double jc, double startjc, double endjc, char* time_string);
void heb_date_string(double jc, float tz, int after_sunset, char* date_string);
#endif