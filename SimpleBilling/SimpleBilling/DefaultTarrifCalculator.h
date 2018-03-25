#pragma once

#include <ctime>
#include <set>

#include "IBillingCalculator.h"
#include "PugiXML\pugixml.hpp"
#include <typeinfo>

/*
	Concrete implementation of BillingCalculator
*/


class DefaultTarrifCalculator :
	public IBillingCalculator
{
public:
	// config provides changable parameters to a tariff
	explicit DefaultTarrifCalculator(const pugi::xml_node& config);
	virtual ~DefaultTarrifCalculator() {};

	// calculate call cost
	virtual double calculate(const Call& call, const Subscriber& subscriber);
	virtual std::string name() { return typeid(*this).name(); }

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
	double mFixedConnectionFee = 0;
	double mCostIn = 0;
	double mCostOut = 0;

	unsigned int	mWeekendCallBonusMinutes = 0;
	unsigned short	mChargeUnitInSeconds = 0;
	unsigned short	mInsideBonusMinutes = 0;
	unsigned short	mInsideBonusPeriodDays = 0;

	std::set<std::string> mHomePrefixes;
};

