#include "stdafx.h"

#include <string>
#include <cctype>

#include "DefaultTarrifCalculator.h"


DefaultTarrifCalculator::DefaultTarrifCalculator(const pugi::xml_node& config)
{
	pugi::xml_node paramsNode = config.child("Parameters");

	for (pugi::xml_attribute attr : paramsNode.attributes())
	{
		std::string atrname(attr.name());
		for (auto &elem : atrname)
			elem = std::tolower(elem);

		if (atrname == "fixedconnectionfee")
			mFixedConnectionFee = attr.as_double();
		else if (atrname == "costin")
			mCostIn = attr.as_double();
		else if (atrname == "costout")
			mCostOut = attr.as_double();
		else if (atrname == "chargeunitseconds")
			mChargeUnitInSeconds = attr.as_int();
		else if (atrname == "weekendcallbonusminutes")
			mWeekendCallBonusMinutes = attr.as_int();
		else if (atrname == "insidebonusminutes")
			mInsideBonusMinutes = attr.as_int();
		else if (atrname == "insidebonusperioddays")
			mInsideBonusPeriodDays = attr.as_int();
	}
	
	for (pugi::xml_node homePrefixeNode : paramsNode.child("HomePrefixes").children("HomePrefix"))
	{
		mHomePrefixes.insert(homePrefixeNode.text().as_string());
	}
}

double DefaultTarrifCalculator::calculate(const Call& call, const Subscriber& subscriber_info)
{
	int callDurationToPrice = call.getCallDuration();

	double cost = 0;
	{
		// unconditional charges
		cost += mFixedConnectionFee;
	}

	if (call.isStartedOnWeekend())
	{
		// it is weekend, and call lasted less than free period
		callDurationToPrice -= mWeekendCallBonusMinutes;
		if (callDurationToPrice < 0)
			return cost;
	}
	
	bool isInternal = isInternalCall(call.getCalledNumber());
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