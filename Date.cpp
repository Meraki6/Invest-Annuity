#include "Date.h"
#include <set>
#include <fstream>

using namespace std;

enum DayOfWeek {SUN, MON, TUE, WED, THU, FRI, SAT};
set<int> holidays;

Date add1Day(Date dt) {
	int year = dt / 10000;
	int month = dt / 100 % 100;
	int day = dt % 100;
	int days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // corresponding days in each month
	days[1] = (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) ? 29 : 28; // change days in February by deciding leap year
	if (day < days[month - 1]) // day before end of the month
		return dt + 1;
	else // day in the end of month
		if (month != 12) // 1st day of next month in current year
			return year * 10000 + (month + 1) * 100 + 1;
		else // beginning of next year
			return (year + 1) * 10000 + 101;
}

Date minus1Day(Date dt) {
	// TODO
	int year = dt / 10000;
	int month = dt / 100 % 100;
	int day = dt % 100;
	int days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	days[1] = (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) ? 29 : 28;
	if (day > 1) // day after 1st day of month
		return dt - 1;
	else // day in 1st day of month
		if (month != 1) // last day in last month of current year
			return year * 10000 + (month - 1) * 100 + days[month - 2];
		else // last day of last year
			return (year - 1) * 10000 + 1231;
}

bool isWeekEnd(Date dt) {
	// TODO
	// dt is the weekend if the difference between dt and Saturday (20221015) or Sunday (20221016) can be divided by 7 with module 0
	if (dateDiff(dt, 20221015) % 7 == 0 || dateDiff(dt, 20221016) % 7 == 0)
		return true;
	return false;
}

bool isHoliday(Date dt) {
	// TODO
	// dt is holiday if it can be found in holidays set
	// if not found, holidays.find(dt) returns holidays.end()
	if (holidays.find(dt) != holidays.end())
		return true;
	return false;
}

bool isBusinessDay(Date dt) {
	// TODO
	// dt is business day if dt is not weekend and also not holiday
	if (!isWeekEnd(dt) && (!isHoliday(dt)))
		return true;
	return false;
}

int dateDiff(Date d1, Date d2) {  // d1 < d2
	// TODO
	if (d2 < d1) return dateDiff(d2, d1); // swap d1 and d2 if d1 > d2
	if (d1 == d2) return 0;
	// case of d1 < d2
	int year1 = d1 / 10000, month1 = d1 / 100 % 100, day1 = d1 % 100;
	int year2 = d2 / 10000, month2 = d2 / 100 % 100, day2 = d2 % 100;

	// first count number of days in between two years, that is from Jan 1st in year1 to Dec 31st in year just before year2 (year2 - 1), considering leaps
	// then add days from Jan 1st in year2 to d2
	// finally minus days from Jan 1st in year1 to d1

	// count number of days in between two years [year1, year2]
	int leaps = 0; // number of leap years from year1 to year2 including year1 and year2
	int yearIter = year1; // leap year iterator
	while (yearIter <= year2) {
		bool isleap = (yearIter % 400 == 0 || (yearIter % 4 == 0 && yearIter % 100 != 0));
		bool y1leap = (yearIter == year1 && (month1 > 2)); // not count leaps for year1 if month1 > 2 since minusing days in non-leap year does not affect remaining days for d1
		bool y2leap = (yearIter == year2 && (month2 <= 2)); // not count leaps for year2 if month2 <= 2 since adding days does not need days in Feb if month2 <= 2
		leaps += isleap ? (y1leap || y2leap ? 0 : 1) : 0;
		++yearIter;
	}
	int result = (year2 - year1) * 365 + leaps;

	// count remaining days
	const int cumdays[12] = { 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 }; // cumulative days in corresponding months for non-leap year
	result += month2 == 1 ? day2 : cumdays[month2 - 2] + day2; // add number of days before d2 after beginng of year2
	result -= month1 == 1 ? day1 : cumdays[month1 - 2] + day1; // minus number of days before d1

	return result;
}

Date monthend(Date dt) {
	// TODO
	int year = dt / 10000;
	int month = dt / 100 % 100;
	int day = dt % 100;
	int days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	days[1] = (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) ? 29 : 28;
	int result = year * 10000 + month * 100 + days[month - 1]; // normal case
	// keep minusing 1 if result is holiday or weekend
	while (isHoliday(result) || isWeekEnd(result)) {
		if (result % 100 == 1) // --result does not work if day of result is 1st day
			result = minus1Day(result);
		else
			--result;
	}
	return result;
}

Date nextMonthend(Date dt) {
	// TODO
	if (dt / 100 % 100 == 12) // month is 12
		return monthend(dt + 10000 - 11 * 100); // dt + 10000 returns next year while - 11 * 100 returns Jan
	return monthend(dt + 100); // dt + 100 returns any days in next month
}

bool loadHoliday(const char* filename) {
	ifstream holFile(filename);
	if (!holFile) return false;
	
	Date dt;
	while (holFile >> dt) {
		holidays.insert(dt);
	}

	holFile.close();
	return true;
}

bool removeHoliday() {
	holidays.clear();
	return true;
}