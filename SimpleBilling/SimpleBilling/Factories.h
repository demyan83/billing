#pragma once

#include <sstream>

#include "IBillingCalculator.h"

class BillingFactory
{
public:
	static IBillingStrategyPtr getBillingCalculatorByName(const std::string& name, const pugi::xml_node& config);
};

class SubscriberInfoProviderFactory
{
public:
	static ISubscriberInfoProviderPtr getSubscriberInfoProviderByName(const std::string& name, const::std::string& config);
};



