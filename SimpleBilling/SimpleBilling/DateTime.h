#pragma once
#include <string>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <ios>

class DateTime
{
public:
	DateTime(const std::string& date_time) : mDateTime{0}
	{
		std::tm dateTime_t;
		int res = sscanf_s(date_time.c_str(), "%d-%d-%dT%d:%d:%d",
			&dateTime_t.tm_year, &dateTime_t.tm_mon, &dateTime_t.tm_mday, &dateTime_t.tm_hour, &dateTime_t.tm_min, &dateTime_t.tm_sec);

		if((res != 6) || !isValid(dateTime_t))
			throw std::out_of_range("Bad formatted string passed in DateTime or doesn't represent datetime");
		
		
		mDateTime = dateTime_t;
		mDateTime.tm_mon -= 1;		// months since January - [0, 11]
		mDateTime.tm_year -= 1900;	// years since 1900
		// sets tm_wday
		mDateTime_t = mktime(&mDateTime);
	}

	inline bool isWeekend() const
	{
		return (6 == mDateTime.tm_wday) || (0 == mDateTime.tm_wday);
	}

	static double getDurationInMinutes(const DateTime& date_time, const DateTime& date_time1)
	{
		double res = getDurationInSeconds(date_time, date_time1);

		return std::ceil(res / 60);
		//return (res + 59) / 60;
	}
	static double getDurationInSeconds(const DateTime& date_time, const DateTime& date_time1)
	{
		return std::abs(difftime(date_time.mDateTime_t, date_time1.mDateTime_t));
	}

private:
	inline bool checkRange(int value, int min, int max)
	{
		return ((value >= min) && (value <= max));
	}

	bool isValid(const std::tm& dateTime)
	{
		// holds number of days in each month, idx is a month num
		static int calendar[13] = { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

		bool res = (checkRange(dateTime.tm_sec, 0, 60)
			&& checkRange(dateTime.tm_min, 0, 59)
			&& checkRange(dateTime.tm_hour, 0, 23)
			&& checkRange(dateTime.tm_mon, 1, 12)
			&& checkRange(dateTime.tm_mday, 1, calendar[dateTime.tm_mon]) // check exact number of dates in a month
			);
		
		// special case for the leap year
		// above checked that day is less or equal to 29, for non-leap year it mast be less then 29
		if (res && (2 == dateTime.tm_mon) && (dateTime.tm_year % 4 != 0))
			res = dateTime.tm_mday < 29;

		 return res;
	}
private:
	std::tm mDateTime;
	std::time_t mDateTime_t;
};

