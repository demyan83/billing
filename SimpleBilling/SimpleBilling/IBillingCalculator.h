#pragma once
#include <string>
#include <memory>

#include "ISubscriberInfoProvider.h"
#include "DateTime.h"
#include "Call.h"

/*
	Base interface of an Billing Calculator
*/

class IBillingCalculator
{
public:
	IBillingCalculator(){}
	virtual ~IBillingCalculator() {}

	virtual double calculate(const Call& call, const Subscriber& subscriber) = 0;
	virtual std::string name() = 0;
};

using IBillingStrategyPtr = std::shared_ptr<IBillingCalculator>;

