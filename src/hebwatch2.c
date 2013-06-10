//#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cordic.h"
#include "sunangle.h"
#include "cmath.h"
#include "hdate.h"
#include "hebwatch2.h"

#define CHECK_ARCTIC_ERROR 1

int arctic; /* 0 normally, 1 if sun doesn't set, -1 if sun doesn't rise */
double jc_sunset_y;
double jc_dawn;
double jc_sunrise;
double jc_sunset;
double jc_tzeit;
double jc_sunrise_t;
double hour_count_start;
double hour_count_end;

void init_sunset_y(double jc, int tz, float longitude, float latitude) {
    double iteration1;
    iteration1 = (arctic == 0) ? time_of_elevation(jc - (1.0/36525), tz, longitude, latitude, -.833, 1) : 0;
    jc_sunset_y = (arctic == 0) ? time_of_elevation(iteration1 - (1.0/36525), tz, longitude, latitude, -.833, 1) : 0;
}

void init_dawn(double jc, int tz, float longitude, float latitude) {
    double iteration1;
    iteration1 = (arctic == 0) ? time_of_elevation(jc, tz, longitude, latitude, DAWN_ANGLE, 0) : 0;
    jc_dawn = (arctic == 0) ? time_of_elevation(iteration1, tz, longitude, latitude, DAWN_ANGLE, 0) : 0;
}

void init_sunrise(double jc, int tz, float longitude, float latitude) {
    double iteration1;
    iteration1 = (arctic == 0) ? time_of_elevation(jc, tz, longitude, latitude, -.833, 0) : 0;
    jc_sunrise = (arctic == 0) ? time_of_elevation(iteration1, tz, longitude, latitude, -.833, 0) : 0;
}

void init_sunset(double jc, int tz, float longitude, float latitude) {
    double iteration1;
    iteration1 = (arctic == 0) ? time_of_elevation(jc, tz, longitude, latitude, -.833, 1) : 0;
    jc_sunset = (arctic == 0) ? time_of_elevation(iteration1, tz, longitude, latitude, -.833, 1) : 0;
}

void init_tzeit(double jc, int tz, float longitude, float latitude) {
    double iteration1;
    iteration1 = (arctic == 0) ? time_of_elevation(jc, tz, longitude, latitude, TZEIT_ANGLE, 1) : 0;
    jc_tzeit = (arctic == 0) ? time_of_elevation(iteration1, tz, longitude, latitude, TZEIT_ANGLE, 1) : 0;
}

void init_sunrise_t(double jc, int tz, float longitude, float latitude) {
    double iteration1;
    iteration1 = (arctic == 0) ? time_of_elevation(jc + (1.0/36525), tz, longitude, latitude, -.833, 0) : 0;
    jc_sunrise_t = (arctic == 0) ? time_of_elevation(iteration1 + (1.0/36525), tz, longitude, latitude, -.833, 0) : 0;
}

/* Initializes the global jc_* caches for the specified jc.
    If force == 0, caches will be left alone if they're on the correct days.
    If force == 1, caches will all be recalculated.
    Return value indicates how many were recalculated.
*/
int init_sunrises_sunsets(double jc, int tz, float longitude, float latitude, int force) {
    int num_changes;
    int y_now;
    int m_now;
    int d_now;
    float h_now;
    int y_target;
    int m_target;
    int d_target;
    float h_target;
    if(arctic != 0) {
        arctic = 0;
        force = 1;
    }
    jc2ymdh(jc, tz, &y_now, &m_now, &d_now, &h_now);
    num_changes = 0;

    jc2ymdh(jc_sunset_y + (1.0/36525), tz, &y_target, &m_target, &d_target, &h_target);
    if((force == 1) || (d_now != d_target)) {
        init_sunset_y(jc, tz, longitude, latitude);
        num_changes++;
    }

    jc2ymdh(jc_dawn, tz, &y_target, &m_target, &d_target, &h_target);
    if((force == 1) || (d_now != d_target)) {
        init_dawn(jc, tz, longitude, latitude);
        num_changes++;
    }

    jc2ymdh(jc_sunrise, tz, &y_target, &m_target, &d_target, &h_target);
    if((force == 1) || (d_now != d_target)) {
        init_sunrise(jc, tz, longitude, latitude);
        num_changes++;
    }

    jc2ymdh(jc_sunset, tz, &y_target, &m_target, &d_target, &h_target);
    if((force == 1) || (d_now != d_target)) {
        init_sunset(jc, tz, longitude, latitude);
        num_changes++;
    }

    jc2ymdh(jc_tzeit, tz, &y_target, &m_target, &d_target, &h_target);
    if((force == 1) || (d_now != d_target)) {
        init_tzeit(jc, tz, longitude, latitude);
        num_changes++;
    }

    jc2ymdh(jc_sunrise_t - (1.0/36525), tz, &y_target, &m_target, &d_target, &h_target);
    if((force == 1) || (d_now != d_target)) {
        init_sunrise_t(jc, tz, longitude, latitude);
        num_changes++;
    }
    return num_changes;
}

/* Return an integer indicating what "part" of the day a specified time is.
        0: nighttime, pre-dawn
        1: between dawn and sunrise
        2: daytime
        3: between sunset and nightfall
        4: after nightfall
*/
int part_of_day(double jc) {
    if(arctic == 1) {
        return 2;
    } else if(arctic == -1) {
        return 0;
    } else if(jc < jc_dawn) {
        return 0;
    } else if(jc < jc_sunrise) {
        return 1;
    } else if(jc < jc_sunset) {
        return 2;
    } else if(jc < jc_tzeit) {
        return 3;
    } else {
        return 4;
    }
}

/* Initialize the variables that determine the halachic hour */
void init_hour_count_start_end(double jc) {
    int daypart = part_of_day(jc);
    if(daypart < 2) {
        hour_count_start = jc_sunset_y;
        hour_count_end = jc_sunrise;
    } else if(daypart == 2) {
        hour_count_start = jc_sunrise;
        hour_count_end = jc_sunset;
    } else {
        hour_count_start = jc_sunset;
        hour_count_end = jc_sunrise_t;
    }
}

void hour_chelek_string(double jc, double startjc, double endjc, char* time_string) {
    int chelek = steps_taken(jc, startjc, endjc, 12960);
    int heb_hours = chelek / 1080;
    chelek -= (heb_hours * 1080);
    if(heb_hours < 10) {
        strcat(time_string, "0");
    }
    char *temp = itoap(heb_hours);
    strcat(time_string, temp);
    if(chelek < 10) {
        strcat(time_string, "0");
    }
    temp = itoap(chelek);
    strcat(time_string, temp);  
}

void h_m_chelek_string(double jc, double startjc, double endjc, char* time_string) {
    int chelek = steps_taken(jc, startjc, endjc, 12960);
    int heb_hours = chelek / 1080;
    chelek -= (heb_hours * 1080);
    int heb_minutes = chelek / 18;
    chelek -= (heb_minutes * 18);
    strcpy(time_string, "");
    if(heb_hours < 10) {
        strcat(time_string, "0");
    }
    char *temp = itoap(heb_hours);
    strcat(time_string, temp);
    if(heb_minutes < 10) {
        strcat(time_string, "0");
    }
    temp = itoap(heb_minutes);
    strcat(time_string, temp);
    if(chelek < 10) {
        strcat(time_string, "0");
    }
    temp = itoap(chelek);
    strcat(time_string, temp);  
}

void heb_date_string(double jc, float tz, int after_sunset, char* date_string) {
    int year;
    int month;
    int day;
    float hour;
    jc2ymdh(jc, tz, &year, &month, &day, &hour);
    int jd_for_hday = (int)(jc*36525) + 2451546;
    /* adjust for the fact that jc's day turns over at noon UTC, where we want to work from midnight local time */
    if((hour - tz) > 12) jd_for_hday--;
    /* adjust for time zones on the "wrong" side of the date line */
    if((hour - tz) > 36) jd_for_hday--;
    if((hour - tz) < -12) jd_for_hday++;
    /* adjust for the date turning over at sunset */
    if(after_sunset > 0) jd_for_hday++;

    int hday;
    int hmonth;
    int hyear;
    int tishrei_l;
    int tishrei_n;
    hdate_jd_to_hdate(jd_for_hday, &hday, &hmonth, &hyear, &tishrei_l, &tishrei_n);
    char *temp = itoap(hday);
    strcpy(date_string, temp);
    strcat(date_string, hdate_string(3, hmonth, 0, 0));
    temp = itoap(hyear);
    strcat(date_string, temp);
}
