#pragma once
#include "IBillingStrategy.h"
#include <ctime>

class DefaultTarrif :
	public IBillingStrategy
{
public:
	DefaultTarrif();
	virtual ~DefaultTarrif() {};

	virtual double calculate(const Call& call, const Subscriber& subscriber);

private:
	inline bool isInternalCall(const std::string& num1, const std::string& num2)
	{
		// compare prefixes
		return num1.compare(0, 3, num2);
	}

	inline unsigned short getBonusMinutes(const Subscriber& subscriber_info, const time_t& start)
	{
		unsigned short bonusMinsLeft = 0;
		if (((subscriber_info.m_last_payment_date - start) < mInsideBonusPeriodDays) & (mInsideBonusMinutes > subscriber_info.minutes_after_last_payment))
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
};

