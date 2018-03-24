#include "stdafx.h"
#include "DefaultTarrif.h"


DefaultTarrif::DefaultTarrif()
{
}

double DefaultTarrif::calculate(const Call& call, const Subscriber& subscriber_info)
{
	unsigned int callDurationToPrice = call.getCallDuration();

	double cost = 0;
	{
		// unconditional charges
		cost += mFixedConnectionFee;
	}

	if (call.isWeekend())
	{
		// it is weekend, and call lasted less than free period
		callDurationToPrice -= mWeekendCallBonusMinutes;
		if (callDurationToPrice < 0)
			return cost;
	}
	
	bool isInternal = isInternalCall(subscriber_info.phone_number, call.getCalledNumber());
	if (isInternal)
	{
		// call to the internal network, check if we have bonus minutes to use
		unsigned short bonusMinutes = getBonusMinutes(subscriber_info, call.getCallStart());

		// TODO = may throw, transact it
		//subscriber_info.addUsedMinutes(callDurationToPrice);
		if (bonusMinutes)
		{
			if (bonusMinutes >= callDurationToPrice)
			{
				return cost;
			}
			else
			{
				callDurationToPrice -= bonusMinutes;
			}
		}
	}

	// now we know how many minutes must be charged
	cost += callDurationToPrice * (isInternal ? mCostIn : mCostOut);

	return cost;
}