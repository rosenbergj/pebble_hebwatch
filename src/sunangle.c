#include <stdio.h>
#include "cordic.h"
#include "sunangle.h"
#include "cmath.h"

int day_of_year(int year, int month, int day) {
	int n1 = (int)(275 * month / 9);
	int n2 = (int)((month + 9) / 12);
	int n3 = 1 + (int)((2 + year - 4*(int)(year/4)) / 3);
	int n4 = 1 - (int)((98 + year - 100*(int)(year/100)) / 99);
	int n5 = 1 - (int)((398 + year - 400*(int)(year/400)) / 399);
	return n1 - (n2 * (n3+n4-n5)) + day - 30;
}

double julian_day(int year, int month, int day, float utchour) {
	int lasty = year - 1;
	int year_to_days = (lasty * 365) + (int)(lasty/4) - (int)(lasty/100) + (int)(lasty/400);
	return 1721424.5 + year_to_days + day_of_year(year, month, day) + ((utchour) / 24);
}

double julian_century(int year, int month, int day, float hour) {
	/* 36525-day intervals since noon GMT, Jan 1, 2000. Negative OK. */
	return (julian_day(year, month, day, hour) - 2451545) / 36525;
}

void jc2ymdh(double jc, int tz, int* year, int* month, int* day, float* hour) {
	double jd = jc * 36525;
	jd += (12.0+tz)/24;

	int J = (int)jd + 2451545;
	int f = J + 1401;
	f += (int)(((int)((4*J+274277)/146097)*3)/4) - 38;
	int e = 4*f + 3;
	int g = (int)((e % 1461)/4);
	int h = 5*g + 2;
	*day = (int)((h%153)/5) + 1;
	*month = ((int)(h/153)+2)%12 + 1;
	*year = (int)(e/1461) - 4716 + (int)((14 - *month)/12);

	double jh = c_fmod(jd,1);
	*hour = jh * 24;
}

static double gml(double jc) {
	/* Geometric mean ecliptic longitude of the sun, in degrees */
	return c_fmod(((0.0003032*jc) + 36000.76983)*jc + 280.46646, 360);
}

static double gma(double jc) {
	/* Geometric mean anomaly of the sun, in degrees */
	return c_fmod((((-0.00000048*jc) - 0.0001559)*jc + 35999.05030)*jc + 357.52910, 360);
}

static double eq_ctr(double jc) {
	/* Solar equation of the center, 3 terms */
	/* Conversion necessary because gma is in degrees but trig functs are in rads */
	double mean_anom = gma(jc);
	double mean_anom1 = deg2rad(mean_anom); /*sin*/
	double mean_anom2 = deg2rad(c_fmod(2*mean_anom, 360));
	double mean_anom3 = deg2rad(c_fmod(3*mean_anom, 360));
	double dummy;
	double meansin1;
	double meansin2;
	double meansin3;
	cossin_cordic(mean_anom1, CORDIC_SIZE, &dummy, &meansin1);
	cossin_cordic(mean_anom2, CORDIC_SIZE, &dummy, &meansin2);
	cossin_cordic(mean_anom3, CORDIC_SIZE, &dummy, &meansin3);

	double quadterm = meansin1 * (1.914602 - jc*(0.004817+0.000014*jc));
	double linterm = meansin2 * (0.019993-0.000101*jc);
	double constterm = meansin3 * 0.000289;
	return quadterm + linterm + constterm;
}

static double app_long(double jc) {
	/* apparent ecliptic longitude of the sun, in degrees */
	/* first we calculate true ecliptic longitude */
	double true_long = gml(jc) + eq_ctr(jc);
	double angle = 125.04 - (1934.136 * jc);
	angle = deg2rad(c_fmod(angle, 360));
	double dummy;
	double anglesin;
	cossin_cordic(angle, CORDIC_SIZE, &dummy, &anglesin);
	return true_long - 0.00569 - (0.00478*anglesin);
}

static double obliq_ecliptic(double jc) {
	/* the cubic term seemed like overkill... */
	double mean_o_e = 23 + (26.0/60) + (21.448/3600) - (jc*0.013004167) - (0.0000164*jc*jc);

	double degangle = c_fmod(125.04 - (1934.136*jc), 360);
	double angle = deg2rad(c_fmod(125.04 - (1934.136*jc), 360));
	double cosine;
	double dummy;
	cossin_cordic(angle, CORDIC_SIZE, &cosine, &dummy);
	return cosine*0.00256 + mean_o_e;
}

static double sun_declin(double jc) {
	double longitude = deg2rad(c_fmod(app_long(jc), 360));
	double obliq = deg2rad(c_fmod(obliq_ecliptic(jc), 360));
	double dummy;
	double longsin;
	double obliqsin;
	cossin_cordic(longitude, CORDIC_SIZE, &dummy, &longsin);
	cossin_cordic(obliq, CORDIC_SIZE, &dummy, &obliqsin);
	double sinsundecl = longsin * obliqsin;
	return arcsin_cordic(sinsundecl, CORDIC_SIZE) * 180 / C_PI;
}

static double eq_time(double jc) {
	/* units are minutes, believe it or not */
	double halfobl = deg2rad(c_fmod(obliq_ecliptic(jc)/2, 360));
	double tanhalfobl = tan_cordic(halfobl, CORDIC_SIZE);
	double y = tanhalfobl*tanhalfobl;

	double eccent = 0.016708634 - jc*(0.000042037+0.0000001267*jc);

	double gmadeg = gma(jc);
	double gmldeg = gml(jc);
	double gma1 = deg2rad(c_fmod(gmadeg, 360));
	double gml2 = deg2rad(c_fmod(2*gmldeg, 360));
	double dummy;
	double singma1;
	cossin_cordic(gma1, CORDIC_SIZE, &dummy, &singma1);
	double cosgml2;
	double singml2;
	cossin_cordic(gml2, CORDIC_SIZE, &cosgml2, &singml2);

	double t1 = y*singml2;
	double t2 = -2*eccent*singma1;

	/* below are 2nd order terms */
	double gma2 = deg2rad(c_fmod(2*gmadeg, 360));
	double gml4 = deg2rad(c_fmod(4*gmldeg, 360));
	double singma2;
	cossin_cordic(gma2, CORDIC_SIZE, &dummy, &singma2);
	double singml4;
	cossin_cordic(gml4, CORDIC_SIZE, &dummy, &singml4);

	double t3 = 4*eccent*y*singma1*cosgml2;
	double t4 = -.5*y*y*singml4;
	double t5 = -1.25*eccent*eccent*singma2;
	return 4*180/C_PI*(t1+t2+t3+t4+t5);
}

static double solar_noon(double jc, int tz, float longitude) {
	/* units are hours from midnight, local time */
	return 12 - longitude/15 - eq_time(jc)/60 + tz;
}

double time_of_elevation(double jc, int tz, float longitude, float latitude, float elev_angle, int pm) {
	/* fixed-point function that, for a given location, takes a date (as julian century) and
		 returns a time (as julian century) on that date (local time) of a specified solar elevation angle.
	   Call with am/pm local time and an angle of -0.833 for sunrise/set. */

	/* TODO: not actually fixed-point if it returns a time between clock midnight and solar midnight.
		 Maybe fix: if jc is after solar_noon+12, get and use tomorrow's solar noon instead (always, or just for am searches?)
		 			if jc is before solar_noon-12, use yesterday's solar noon (always or just pm?) */
	double latcos;
	double latsin;
	double declcos;
	double declsin;
	double elevcos;
	double elevsin;
	cossin_cordic(deg2rad(c_fmod(latitude, 360)), CORDIC_SIZE, &latcos, &latsin);
	cossin_cordic(deg2rad(c_fmod(sun_declin(jc), 360)), CORDIC_SIZE, &declcos, &declsin);
	cossin_cordic(deg2rad(c_fmod(elev_angle, 360)), CORDIC_SIZE, &elevcos, &elevsin);
	double hacos = (elevsin - (latsin*declsin))/(latcos*declcos);
	/* TODO: add check here that 0 < hacos < pi; if not, angle is never reached that day */
	double target_ha = 90 - (arcsin_cordic(hacos, 30)*180/C_PI);

	int sign = pm*2 - 1;
	/* units of solar noon and target_time are hours from midnight */
	double target_time = solar_noon(jc, tz, longitude) + sign*target_ha/15;

	int year;
	int month;
	int day;
	float hour;
	jc2ymdh(jc, tz, &year, &month, &day, &hour);
	return julian_century(year, month, day, target_time-tz);
}

static double hour_angle(double jc, int tz, double longitude) {
	int year;
	int month;
	int day;
	float hour;
	jc2ymdh(jc, tz, &year, &month, &day, &hour);
	float true_solar_time = c_fmod(60*(hour-tz) + eq_time(jc) + 4*longitude, 1440);
	float quarter_tst = true_solar_time / 4;
	if(quarter_tst < 0) {
		return quarter_tst + 180;
	}
	return quarter_tst - 180;
}

double elevation_angle(double jc, int tz, float longitude, float latitude) {
	/* Returns the elevation angle now.  Possibly less interesting than
		the time of a specified elevation, such as sunset. */
	double latcos;
	double latsin;
	cossin_cordic(deg2rad(c_fmod(latitude, 360)), CORDIC_SIZE, &latcos, &latsin);
	double declcos;
	double declsin;
	cossin_cordic(deg2rad(c_fmod(sun_declin(jc), 360)), CORDIC_SIZE, &declcos, &declsin);
	double hacos;
	double hasin;
	cossin_cordic(deg2rad(c_fmod(hour_angle(jc, tz, longitude), 360)), CORDIC_SIZE, &hacos, &hasin);
	double elevsin = latsin*declsin + latcos*declcos*hacos;
	int sign = 1;
	if(elevsin < 0) {
		sign = -1;
		elevsin *= -1;
	}
    return arcsin_cordic(elevsin, CORDIC_SIZE)*180*sign/C_PI;
}
