/*  libhdate - Hebrew calendar library: http://libhdate.sourceforge.net
 *
 *  Copyright (C) 2011-2012 Boruch Baum  <boruch-baum@users.sourceforge.net>
 *                2004-2007 Yaacov Zamir <kzamir@walla.co.il>
 *                1984-2003 Amos Shapir
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/** @file hdate.h
    @brief libhdate C language header.
    
    libhdate - Hebrew calendar library, the C language header file.
*/

#ifndef __HDATE_H__
#define __HDATE_H__

/** @def HDATE_DIASPORA_FLAG
  @brief use diaspora dates and holydays flag
*/
#define HDATE_DIASPORA_FLAG -1
	
/** @def HDATE_ISRAEL_FLAG
  @brief use israel dates and holydays flag
*/
#define HDATE_ISRAEL_FLAG 0
	
/** @def HDATE_SHORT_FLAG
  @brief use short strings flag
*/
#define HDATE_SHORT_FLAG -1
	
/** @def HDATE_LONG_FLAG
  @brief use long strings flag
*/
#define HDATE_LONG_FLAG 0

/** @def HEBREW_NUMBER_BUFFER_SIZE
  @brief for hdate_get_int_string_ and hdate_get_int_wstring
  @note
  How large should the buffer be? Hebrew year 10,999 would
  be י'תתקצ"ט, eight characters, each two bytes, plus an
  end-of-string delimiter, equals 17. This could effectively
  yield a range extending to Hebrew year 11,899, י"א תתצ"ט,
  due to the extra ק needed for the '900' century. However,
  for readability, I would want a an extra space at that
  point between the millenium and the century...
*/
#define HEBREW_NUMBER_BUFFER_SIZE 17
#define HEBREW_WNUMBER_BUFFER_SIZE 9


/** @struct hdate_struct
  @brief libhdate Hebrew date struct
*/
typedef struct
{
	/** The number of day in the hebrew month (1..31). */
	int hd_day;
	/** The number of the hebrew month 1..14 (1 - tishre, 13 - adar 1, 14 - adar 2). */
	int hd_mon;
	/** The number of the hebrew year. */
	int hd_year;
	/** The number of the day in the month. (1..31) */
	int gd_day;
	/** The number of the month 1..12 (1 - jan). */
	int gd_mon;
	/** The number of the year. */
	int gd_year;
	/** The day of the week 1..7 (1 - sunday). */
	int hd_dw;
	/** The length of the year in days. */
	int hd_size_of_year;
	/** The week day of Hebrew new year. */
	int hd_new_year_dw;
	/** The number type of year. */
	int hd_year_type;
	/** The Julian day number */
	int hd_jd;
	/** The number of days passed since 1 tishrey */
	int hd_days;
	/** The number of weeks passed since 1 tishrey */
	int hd_weeks;
} hdate_struct;


/*************************************************************/
/*************************************************************/

/**
 @brief Days since Tishrey 3744
 
 @author Amos Shapir 1984 (rev. 1985, 1992) Yaacov Zamir 2003-2005 
 
 @param hebrew_year The Hebrew year
 @return Number of days since 3,1,3744
*/
int
hdate_days_from_3744 (int hebrew_year);

/**
 @brief Converting from the Julian day to the Gregorian date
 
 @author Yaacov Zamir (Algorithm, Henry F. Fliegel and Thomas C. Van Flandern ,1968)

 @param jd Julian day
 @param day return Day of month 1..31
 @param month return Month 1..12
 @param year return Year in 4 digits e.g. 2001
 */
void
hdate_jd_to_gdate (int jd, int *day, int *month, int *year);

/**
 @brief Converting from the Julian day to the Hebrew day
 
 @author Yaacov Zamir 2005

 @param jd Julian day
 @param day return Day of month 1..31
 @param month return Month 1..14 (13 - Adar 1, 14 - Adar 2)
 @param year return Year in 4 digits e.g. 2001
 @param jd_tishrey1 return the julian number of 1 Tishrey this year
 @param jd_tishrey1_next_year return the julian number of 1 Tishrey next year
 */
void
hdate_jd_to_hdate (int jd, int *day, int *month, int *year, int *jd_tishrey1, int *jd_tishrey1_next_year);

/*************************************************************/
/*************************************************************/

/**
 @brief   Return string values for hdate information
 @return  a pointer to a string containing the information. In the cases
          integers and omer, the strings will NOT be static, and the
          caller must free() them after use. Returns a null pointer
          upon failure.
 @param type_of_string 	0 = integer, 1 = day of week, 2 = parshaot,
						3 = hmonth, 4 = gmonth, 5 = holiday, 6 = omer
 @param index			integer		( 0 < n < 11000)
						day of week ( 0 < n <  8 )
						parshaot	( 0 , n < 62 )
						hmonth		( 0 < n < 15 )
						gmonth		( 0 < n < 13 )
						holiday		( 0 < n < 37 )
						omer		( 0 < n < 50 )
 @param short_form   0 = short format
 @param hebrew_form  0 = not hebrew (native/embedded)
*/
char* hdate_string( int type_of_string, int index, int short_form, int hebrew_form);

/** @def HDATE_STRING_INT
  @brief for function hdate_string: identifies string type: integer
*/
#define HDATE_STRING_INT     0

/** @def HDATE_STRING_DOW
  @brief for function hdate_string: identifies string type: day of week 
*/
#define HDATE_STRING_DOW       1

/** @def HDATE_STRING_PARASHA
  @brief for function hdate_string: identifies string type: parasha
*/
#define HDATE_STRING_PARASHA   2

/** @def HDATE_STRING_HMONTH
  @brief for function hdate_string: identifies string type: hebrew_month
*/
#define HDATE_STRING_HMONTH    3

/** @def HDATE_STRING_GMONTH
  @brief for function hdate_string: identifies string type: gregorian_month
*/
#define HDATE_STRING_GMONTH    4

/** @def HDATE_STRING_HOLIDAY
  @brief for function hdate_string: identifies string type: holiday
*/
#define HDATE_STRING_HOLIDAY   5

/** @def HDATE_STRING_HOLIDAY
  @brief for function hdate_string: identifies string type: holiday
*/
#define HDATE_STRING_OMER      6

/** @def HDATE_STRING_SHORT
  @brief for function hdate_string: use short form, if one exists
*/
#define HDATE_STRING_SHORT   1

/** @def HDATE_STRING_LONG
  @brief for function hdate_string: use long form
*/
#define HDATE_STRING_LONG    0

/** @def HDATE_STRING_HEBREW
  @brief for function hdate_string: use embedded hebrew string
*/
#define HDATE_STRING_HEBREW  1

/** @def HDATE_STRING_LOCAL
  @brief for function hdate_string: use local locale string
*/
#define HDATE_STRING_LOCAL   0

#endif
