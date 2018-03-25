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

	std::cout << "		Call duration is " << callDurationToPrice << std::endl;

	double cost = 0;
	{
		// unconditional charges
		cost += mFixedConnectionFee;
	}

	if (call.isStartedOnWeekend())
	{
		std::cout << "		Call took place on weekend" << std::endl;
		callDurationToPrice -= mWeekendCallBonusMinutes;
		if (callDurationToPrice < 0)
		{
			std::cout <<"		Weekend bonus is "<< mWeekendCallBonusMinutes << ". Charge connection fee only" << std::endl;
			// it is weekend, and call lasted less than free period
			return cost;
		}
		else
		{
			std::cout << "		Subsctract weekend bonus from total time. Now it is " << callDurationToPrice << std::endl;
		}
	}
	
	bool isInternal = isInternalCall(call.getCalledNumber());
	if (isInternal)
	{
		std::cout << "		Call to internal number" << std::endl;
		// call to the internal network, check if we have bonus minutes to use
		unsigned short bonusMinutes = getBonusMinutes(subscriber_info, call.getCallStart());

		// TODO = may throw, transact it
		//subscriber_info.addUsedMinutes(callDurationToPrice);
		if (bonusMinutes)
		{
			std::cout << "		Subscriber has " << bonusMinutes << " free talking bonus minutes to use" << std::endl;
			if (bonusMinutes >= callDurationToPrice)
			{
				std::cout << "		Bonus minutes used. Charge connection fee only" << std::endl;
				return cost;
			}
			else
			{
				callDurationToPrice -= bonusMinutes;
				std::cout << "		Use bonus minutes. Minutes left to price are " << callDurationToPrice << std::endl;
			}
		}
		else
		{
			std::cout << "		Subscriber has NO free talking bonus minutes to use" << std::endl;
		}
	}
	else
	{
		std::cout << "		Call to external number" << std::endl;
	}

	std::cout << "		Minutes to price " << callDurationToPrice << std::endl;
	// now we know how many minutes must be charged
	cost += callDurationToPrice * (isInternal ? mCostIn : mCostOut);

	return cost;
}