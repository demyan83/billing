#pragma once
#include <string>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <ios>

/*
	Class provides basic DateTime manupulation functionality
*/

class DateTime
{
public:
	DateTime() : DateTime("2000-01-01T10:00:00"){}
	explicit DateTime(const std::string& date_time);

	bool operator<(const DateTime& rhs)
	{
		return mDateTime_t < rhs.mDateTime_t;
	}
	// check whether date is weekend
	inline bool isWeekend() const
	{
		return (6 == mDateTime.tm_wday) || (0 == mDateTime.tm_wday);
	}
	// calculate time period between two dates. Result expressed in seconds
	// Order of parameters does not matter. Absolute difference is returned
	static double getDurationInSecondsAbs(const DateTime& date_time, const DateTime& date_time1);
	// calculate time period between two dates. Result expressed in minutes. 
	// Order of parameters does not matter. Absolute difference is returned
	static double getDurationInMinutesAbs(const DateTime& date_time, const DateTime& date_time1);
	// calculate time period between two dates. Result expressed in minutes. 
	// Order of parameters does not matter. Absolute difference is returned
	static unsigned short getDurationInDays(const DateTime& date_time, const DateTime& date_time1);
private:
	// Helper to check wtether value is in range
	inline bool checkRange(int value, int min, int max) const
	{
		return ((value >= min) && (value <= max));
	}
	// Helper to check whether date passed from string is a valid date
	bool isValid(const std::tm& dateTime) const;

private:
	std::tm mDateTime;
	std::time_t mDateTime_t;
};

