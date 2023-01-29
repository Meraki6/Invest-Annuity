#pragma once
// Use "int" type (yyyymmdd) to represent Date.
typedef int Date;

// Input a date (yyyymmdd). Return the next date (yyyymmdd).
// No need to skip holidays or weekends
Date __declspec(dllexport) add1Day(Date);

// Input a date (yyyymmdd). Return the previous date (yyyymmdd).
// No need to skip holidays or weekends
Date __declspec(dllexport) minus1Day(Date);

// Input a date (yyyymmdd). 
// Return true if input is a weekend; otherwise, false.
bool __declspec(dllexport) isWeekEnd(Date);

// Input a full path name (C string) of a holiday file.
// Insert the holidays (yyyymmdd) to a data structure which 
// facilitates checking whether a date is a holiday.
bool __declspec(dllexport) loadHoliday(const char*);

// Input a date (yyyymmdd).
// Return true if input date is a holiday in holiday file; 
// otherwise, false.
bool __declspec(dllexport) isHoliday(Date);

// Input a date (yyyymmdd).
// Return true is input date is a business day; otherwise, false.
bool isBusinessDay(Date);

// Input a date (yyyymmdd). Return the monthend date.
// Monthend should not be a weekend or holiday.
Date __declspec(dllexport) monthend(Date);

// Input a date (yyyymmdd). Return the next monthend date.
// Next monthend should not be a weekend or holiday.
Date __declspec(dllexport) nextMonthend(Date);

// Input 2 dates (yyyymmdd). Return the number of dates in between.
// e.g. dateDiff(20210924, 20211024) returns 30.
int __declspec(dllexport) dateDiff(Date, Date);

// Remove all holidays from data structure storing holidays.
bool __declspec(dllexport) removeHoliday();
