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

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include "hdate.h"
#include "support.h"

/**
 @brief   Return string values for hdate information
 @return  a pointer to a string containing the information. In the cases
          integers and omer, the strings will NOT be static, and the
          caller must free() them after use.
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

// TODO - Number days of chol hamoed, and maybe have an entry for shabbat chol hamoed
// DONE - (I hope) change short to be = 1 long = 0, and switch order of data structures
//        this way user app opt.short = 0/FALSE will work as a parameter to pass here

// These definitions are in hdate.h
//
// HDATE_STRING_INT     0
// HDATE_STRING_DOW     1
// HDATE_STRING_PARASHA 2
// HDATE_STRING_HMONTH  3
// HDATE_STRING_GMONTH  4
// HDATE_STRING_HOLIDAY 5
// HDATE_STRING_OMER    6
// HDATE STRING_SHORT   1
// HDATE_STRING_LONG    0
// HDATE_STRING_HEBREW  1
// HDATE_STRING_LOCAL   0
char* hdate_string( int const type_of_string, int const index, int const input_short_form, int const input_hebrew_form)
{
	int short_form = 0;
	int hebrew_form = 0;

	#define H_CHAR_WIDTH 2
	static char *digits[3][10] = {
		{" ", "א", "ב", "ג", "ד", "ה", "ו", "ז", "ח", "ט"},
		{"ט", "י", "כ", "ל", "מ", "נ", "ס", "ע", "פ", "צ"},
		{" ", "ק", "ר", "ש", "ת"}
	};

	static char *days[2][2][7] = {
		{ // begin english
		{ // begin english long
		N_("Sunday"), N_("Monday"), N_("Tuesday"), N_("Wednesday"),
		 N_("Thursday"), N_("Friday"), N_("Saturday")},
		{ // begin english short
		 N_("Sun"), N_("Mon"), N_("Tue"), N_("Wed"), N_("Thu"),
		 N_("Fri"), N_("Sat")}
		},
		{ // begin hebrew
		{ // begin hebrew long
		"ראשון", "שני", "שלישי", "רביעי", "חמישי", "שישי", "שבת"},
		{ // begin hebrew short
		"א", "ב", "ג", "ד", "ה", "ו", "ש"}
		}
		};

	static char *hebrew_months[2][1][14] = {
		{ // begin english
		{ // begin english long
		 N_("Tishrei"), N_("Cheshvan"), N_("Kislev"), N_("Tevet"),
		 N_("Sh'vat"), N_("Adar"), N_("Nisan"), N_("Iyyar"),
		 N_("Sivan"), N_("Tammuz"), N_("Av"), N_("Elul"), N_("Adar I"),
		 N_("Adar II")}
		},
		{ // begin hebrew
		{ // begin hebrew long
		 "תשרי", "חשון", "כסלו", "טבת", "שבט", "אדר", "ניסן", "אייר",
		  "סיון", "תמוז", "אב", "אלול", "אדר א", "אדר ב" }}
		};

	static char *gregorian_months[1][12] = {
		{N_("January"), N_("February"), N_("March"),
		 N_("April"), N_("May"), N_("June"),
		 N_("July"), N_("August"), N_("September"),
		 N_("October"), N_("November"), N_("December")},
	};


#ifdef ENABLE_NLS
	bindtextdomain (PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (PACKAGE, "UTF-8");
#endif

	// validate parameters
	if (input_short_form != 0) short_form = 1;
	if (input_hebrew_form != 0) hebrew_form = 1;

	switch (type_of_string)
	{
	case HDATE_STRING_DOW: if (index >= 1 && index <= 7)
				return _(days[hebrew_form][short_form][index - 1]);
				break;
	case HDATE_STRING_HMONTH:
				if (index >= 1 && index <= 14)
				return _(hebrew_months[hebrew_form][0][index - 1]);
				break;
	case HDATE_STRING_GMONTH:
				if (index >= 1 && index <= 12)
				return _(gregorian_months[0][index - 1]);
				break;
	} // end of switch(type_of_string)

	return NULL;
}
