#pragma once

#include <ctime>
#include <set>

#include "IBillingCalculator.h"
#include "PugiXML\pugixml.hpp"

class DefaultTarrifCalculator :
	public IBillingCalculator
{
public:
	explicit DefaultTarrifCalculator(const pugi::xml_node& config);
	virtual ~DefaultTarrifCalculator() {};

	virtual double calculate(const Call& call, const Subscriber& subscriber);

private:
	inline bool isInternalCall(const std::string& numCalled)
	{
		// compare prefixes
		std::string prefix(numCalled, 0, 3);
		return mHomePrefixes.find(prefix) != mHomePrefixes.end();
	}

	inline unsigned short getBonusMinutes(const Subscriber& subscriber_info, const DateTime& start)
	{
		unsigned short bonusMinsLeft = 0;
		if (((DateTime::getDurationInDays(subscriber_info.last_payment_date, start)) < mInsideBonusPeriodDays) & (mInsideBonusMinutes > subscriber_info.minutes_after_last_payment))
		{
			bonusMinsLeft = mInsideBonusMinutes - subscriber_info.minutes_after_last_payment;
		}
		return bonusMinsLeft;
	}

private:
	double mFixedConnectionFee;
	double mCostIn;
	double mCostOut;

	unsigned int	mWeekendCallBonusMinutes;
	unsigned short	mChargeUnitInSeconds;
	unsigned short	mInsideBonusMinutes;
	unsigned short	mInsideBonusPeriodDays;

	std::set<std::string> mHomePrefixes;
};

