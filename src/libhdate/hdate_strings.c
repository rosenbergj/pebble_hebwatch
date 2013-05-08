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
#include <string.h>

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

	static char *parashaot[2][2][62] = {
		{ // begin english
		{ // begin english long
		 N_("none"),		N_("Bereshit"),		N_("Noach"),
		 N_("Lech-Lecha"),	N_("Vayera"),		N_("Chayei Sara"),
		 N_("Toldot"),		N_("Vayetzei"),		N_("Vayishlach"),
		 N_("Vayeshev"),	N_("Miketz"),		N_("Vayigash"),		/* 11 */
		 N_("Vayechi"),		N_("Shemot"),		N_("Vaera"),
		 N_("Bo"),		N_("Beshalach"),	N_("Yitro"),
		 N_("Mishpatim"),	N_("Terumah"),		N_("Tetzaveh"),		/* 20 */
		 N_("Ki Tisa"),		N_("Vayakhel"),		N_("Pekudei"),
		 N_("Vayikra"),		N_("Tzav"),		N_("Shmini"),
		 N_("Tazria"),		N_("Metzora"),		N_("Achrei Mot"),
		 N_("Kedoshim"),	N_("Emor"),		N_("Behar"),		/* 32 */
		 N_("Bechukotai"),	N_("Bamidbar"),		N_("Nasso"),
		 N_("Beha'alotcha"),	N_("Sh'lach"),		N_("Korach"),
		 N_("Chukat"),		N_("Balak"),		N_("Pinchas"),		/* 41 */
		 N_("Matot"),		N_("Masei"),		N_("Devarim"),
		 N_("Vaetchanan"),	N_("Eikev"),		N_("Re'eh"),
		 N_("Shoftim"),		N_("Ki Teitzei"),	N_("Ki Tavo"),		/* 50 */
		 N_("Nitzavim"),	N_("Vayeilech"),	N_("Ha'Azinu"),
		 N_("Vezot Habracha"),	/* 54 */
		 N_("Vayakhel-Pekudei"),N_("Tazria-Metzora"),	N_("Achrei Mot-Kedoshim"),
		 N_("Behar-Bechukotai"),N_("Chukat-Balak"),	N_("Matot-Masei"),
		 N_("Nitzavim-Vayeilech")},
		{ // begin english short
		 N_("none"),		N_("Bereshit"),		N_("Noach"),
		 N_("Lech-Lecha"),	N_("Vayera"),		N_("Chayei Sara"),
		 N_("Toldot"),		N_("Vayetzei"),		N_("Vayishlach"),
		 N_("Vayeshev"),	N_("Miketz"),		N_("Vayigash"),		/* 11 */
		 N_("Vayechi"),		N_("Shemot"),		N_("Vaera"),
		 N_("Bo"),		N_("Beshalach"),	N_("Yitro"),
		 N_("Mishpatim"),	N_("Terumah"),		N_("Tetzaveh"),		/* 20 */
		 N_("Ki Tisa"),		N_("Vayakhel"),		N_("Pekudei"),
		 N_("Vayikra"),		N_("Tzav"),		N_("Shmini"),
		 N_("Tazria"),		N_("Metzora"),		N_("Achrei Mot"),
		 N_("Kedoshim"),	N_("Emor"),		N_("Behar"),		/* 32 */
		 N_("Bechukotai"),	N_("Bamidbar"),		N_("Nasso"),
		 N_("Beha'alotcha"),	N_("Sh'lach"),		N_("Korach"),
		 N_("Chukat"),		N_("Balak"),		N_("Pinchas"),		/* 41 */
		 N_("Matot"),		N_("Masei"),		N_("Devarim"),
		 N_("Vaetchanan"),	N_("Eikev"),		N_("Re'eh"),
		 N_("Shoftim"),		N_("Ki Teitzei"),	N_("Ki Tavo"),		/* 50 */
		 N_("Nitzavim"),	N_("Vayeilech"),	N_("Ha'Azinu"),
		 N_("Vezot Habracha"),	/* 54 */
		 N_("Vayakhel-Pekudei"),N_("Tazria-Metzora"),	N_("Achrei Mot-Kedoshim"),
		 N_("Behar-Bechukotai"),N_("Chukat-Balak"),	N_("Matot-Masei"),
		 N_("Nitzavim-Vayeilech")}
		},
		{ // begin hebrew
		{ // begin hebrew long
		 "none",		"בראשית",		"נח",
		 "לך לך",		"וירא",			"חיי שרה",
		 "תולדות",		"ויצא",			"וישלח",
		 "וישב",		"מקץ",			"ויגש",		/* 11 */
		 "ויחי",		"שמות",			"וארא",
		 "בא",			"בשלח",			"יתרו",
		 "משפטים",		"תרומה",		"תצוה",		/* 20 */
		 "כי תשא",		"ויקהל",		"פקודי",
		 "ויקרא",		"צו",			"שמיני",
		 "תזריע",		"מצורע",		"אחרי מות",
		 "קדושים",		"אמור",			"בהר",		/* 32 */
		 "בחוקתי",		"במדבר",		"נשא",
		 "בהעלתך",		"שלח",			"קרח",
		 "חקת",			"בלק",			"פנחס",		/* 41 */
		 "מטות",		"מסעי",			"דברים",
		 "ואתחנן",		"עקב",			"ראה",
		 "שופטים",		"כי תצא",		"כי תבוא",		/* 50 */
		 "נצבים",		"וילך",			"האזינו",
		 "וזאת הברכה",	/* 54 */
		 "ויקהל-פקודי",	"תזריע-מצורע",	"אחרי מות-קדושים",
		 "בהר-בחוקתי",	"חוקת-בלק",		"מטות מסעי",
		 "נצבים-וילך"},
		{ // begin hebrew short
		 "none",		"בראשית",		"נח",
		 "לך לך",		"וירא",			"חיי שרה",
		 "תולדות",		"ויצא",			"וישלח",
		 "וישב",		"מקץ",			"ויגש",		/* 11 */
		 "ויחי",		"שמות",			"וארא",
		 "בא",			"בשלח",			"יתרו",
		 "משפטים",		"תרומה",		"תצוה",		/* 20 */
		 "כי תשא",		"ויקהל",		"פקודי",
		 "ויקרא",		"צו",			"שמיני",
		 "תזריע",		"מצורע",		"אחרי מות",
		 "קדושים",		"אמור",			"בהר",		/* 32 */
		 "בחוקתי",		"במדבר",		"נשא",
		 "בהעלתך",		"שלח",			"קרח",
		 "חקת",			"בלק",			"פנחס",		/* 41 */
		 "מטות",		"מסעי",			"דברים",
		 "ואתחנן",		"עקב",			"ראה",
		 "שופטים",		"כי תצא",		"כי תבוא",		/* 50 */
		 "נצבים",		"וילך",			"האזינו",
		 "וזאת הברכה",	/* 54 */
		 "ויקהל-פקודי",	"תזריע-מצורע",	"אחרי מות-קדושים",
		 "בהר-בחוקתי",	"חוקת-בלק",		"מטות מסעי",
		 "נצבים-וילך"}
		}
		};

	static char *hebrew_months[2][2][14] = {
		{ // begin english
		{ // begin english long
		 N_("Tishrei"), N_("Cheshvan"), N_("Kislev"), N_("Tevet"),
		 N_("Sh'vat"), N_("Adar"), N_("Nisan"), N_("Iyyar"),
		 N_("Sivan"), N_("Tammuz"), N_("Av"), N_("Elul"), N_("Adar I"),
		 N_("Adar II")},
		{ // begin english short
		 N_("Tishrei"), N_("Cheshvan"), N_("Kislev"), N_("Tevet"),
		 N_("Sh'vat"), N_("Adar"), N_("Nisan"), N_("Iyyar"),
		 N_("Sivan"), N_("Tammuz"), N_("Av"), N_("Elul"), N_("Adar I"),
		 N_("Adar II")}
		},
		{ // begin hebrew
		{ // begin hebrew long
		 "תשרי", "חשון", "כסלו", "טבת", "שבט", "אדר", "ניסן", "אייר",
		  "סיון", "תמוז", "אב", "אלול", "אדר א", "אדר ב" },
		{ // begin hebrew short
		 "תשרי", "חשון", "כסלו", "טבת", "שבט", "אדר", "ניסן", "אייר",
		  "סיון", "תמוז", "אב", "אלול", "אדר א", "אדר ב" }}
		};

	static char *gregorian_months[2][12] = {
		{N_("January"), N_("February"), N_("March"),
		 N_("April"), N_("May"), N_("June"),
		 N_("July"), N_("August"), N_("September"),
		 N_("October"), N_("November"), N_("December")},
		{N_("Jan"), N_("Feb"), N_("Mar"), N_("Apr"), N_("May"),
		 N_("Jun"), N_("Jul"), N_("Aug"), N_("Sep"), N_("Oct"),
		 N_("Nov"), N_("Dec")},
	};

	static char *holidays[2][2][37] = {
		{ // begin english
		{ // begin english long
		 N_("Rosh Hashana I"),	N_("Rosh Hashana II"),
		 N_("Tzom Gedaliah"),	N_("Yom Kippur"),
		 N_("Sukkot"),			N_("Hol hamoed Sukkot"),
		 N_("Hoshana raba"),	N_("Simchat Torah"),
		 N_("Chanukah"),		N_("Asara B'Tevet"),
		 N_("Tu B'Shvat"),		N_("Ta'anit Esther"),
		 N_("Purim"),			N_("Shushan Purim"),
		 N_("Pesach"),			N_("Hol hamoed Pesach"),
		 N_("Yom HaAtzma'ut"),	N_("Lag B'Omer"),
		 N_("Erev Shavuot"),	N_("Shavuot"),
		 N_("Tzom Tammuz"),		N_("Tish'a B'Av"),
		 N_("Tu B'Av"),			N_("Yom HaShoah"),
		 N_("Yom HaZikaron"),	N_("Yom Yerushalayim"),
		 N_("Shmini Atzeret"),	N_("Pesach VII"),
		 N_("Pesach VIII"),		N_("Shavuot II"),
		 N_("Sukkot II"),		N_("Pesach II"),
		 N_("Family Day"),		N_("Memorial day for fallen whose place of burial is unknown"), 
		 N_("Yitzhak Rabin memorial day"), N_("Zeev Zhabotinsky day"),
		 N_("Erev Yom Kippur")},
		{ // begin english short
		 N_("Rosh Hashana I"),	N_("Rosh Hashana II"),
		 N_("Tzom Gedaliah"),	N_("Yom Kippur"),
		 N_("Sukkot"),			N_("Hol hamoed Sukkot"),
		 N_("Hoshana raba"),	N_("Simchat Torah"),
		 N_("Chanukah"),		N_("Asara B'Tevet"),	/* 10 */
		 N_("Tu B'Shvat"),		N_("Ta'anit Esther"),
		 N_("Purim"),			N_("Shushan Purim"),
		 N_("Pesach"),			N_("Hol hamoed Pesach"),
		 N_("Yom HaAtzma'ut"),	N_("Lag B'Omer"),
		 N_("Erev Shavuot"),	N_("Shavuot"),			/* 20 */
		 N_("Tzom Tammuz"),		N_("Tish'a B'Av"),
		 N_("Tu B'Av"),			N_("Yom HaShoah"),
		 N_("Yom HaZikaron"),	N_("Yom Yerushalayim"),
		 N_("Shmini Atzeret"),	N_("Pesach VII"),
		 N_("Pesach VIII"),		N_("Shavuot II"),   /* 30 */
		 N_("Sukkot II"),		N_("Pesach II"),	 
		 N_("Family Day"),		N_("Memorial day for fallen whose place of burial is unknown"), 
		 N_("Rabin memorial day"),	 N_("Zhabotinsky day"),
		 N_("Erev Yom Kippur")}
		},
		{ // begin hebrew
		{ // begin hebrew long
		 "א ר\"ה",		 "ב' ר\"ה",
		 "צום גדליה",		 "יוה\"כ",
		 "סוכות",		 "חוה\"מ סוכות",
		 "הוש\"ר",		 "שמח\"ת",
		 "חנוכה",		 "י' בטבת",	/* 10 */
		 "ט\"ו בשבט",		 "תענית אסתר",
		 "פורים",		 "שושן פורים",
		 "פסח",			 "חוה\"מ פסח",
		 "יום העצמאות",		 "ל\"ג בעומר",
		 "ערב שבועות",		 "שבועות",	/* 20 */
		 "צום תמוז",		 "ט' באב",
		 "ט\"ו באב",		 "יום השואה",
		 "יום הזכרון",		 "יום י-ם",
		 "שמיני עצרת",		 "ז' פסח",
		 "אחרון של פסח",	 "ב' שבועות",   /* 30 */
		 "ב' סוכות",		 "ב' פסח",	 
		 "יום המשפחה",		 "יום זכרון...", 
		 "יום הזכרון ליצחק רבין","יום ז\'בוטינסקי",
		 "עיוה\"כ"},
		{ // begin hebrew short
		 "א' ראש השנה",		"ב' ראש השנה",
		 "צום גדליה",		"יום הכפורים",
		 "סוכות",		"חול המועד סוכות",
		 "הושענא רבה",		"שמחת תורה",
		 "חנוכה",		"צום עשרה בטבת",/* 10 */
		 "ט\"ו בשבט",		"תענית אסתר",
		 "פורים",		"שושן פורים",
		 "פסח",			"חול המועד פסח",
		 "יום העצמאות",		"ל\"ג בעומר",
		 "ערב שבועות",		"שבועות",	/* 20 */
		 "צום שבעה עשר בתמוז",	"תשעה באב",
		 "ט\"ו באב",		"יום השואה",
		 "יום הזכרון",		"יום ירושלים",
		 "שמיני עצרת",		"שביעי פסח",
		 "אחרון של פסח",	"שני של שבועות",/* 30 */
		 "שני של סוכות",	"שני של פסח",
		 "יום המשפחה",		"יום זכרון...", 
		 "יום הזכרון ליצחק רבין","יום ז\'בוטינסקי",
		 "עיוה\"כ"}	}
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
	case HDATE_STRING_PARASHA: if (index >= 1 && index <= 61)
				return _(parashaot[hebrew_form][short_form][index]);
				break;
	case HDATE_STRING_HMONTH:
				if (index >= 1 && index <= 14)
				return _(hebrew_months[hebrew_form][short_form][index - 1]);
				break;
	case HDATE_STRING_GMONTH:
				if (index >= 1 && index <= 12)
				return _(gregorian_months[short_form][index - 1]);
				break;
	case HDATE_STRING_HOLIDAY: if (index >= 1 && index <= 37)
				return _(holidays[hebrew_form][short_form][index - 1]);
				break;
	} // end of switch(type_of_string)

	return NULL;
}
