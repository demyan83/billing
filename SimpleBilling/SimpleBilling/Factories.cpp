#include "stdafx.h"

#include "IBillingCalculator.h"
#include "DefaultTarrifCalculator.h"
#include "SubscriberInfoProviderFileSys.h"

#include "Factories.h"

// factory to create instance of a Billing class, by name
// support only 1 at the moment
IBillingStrategyPtr BillingFactory::getBillingCalculatorByName(const std::string& name, const pugi::xml_node& config)
{
	if (name == "Default")
		return std::make_shared<DefaultTarrifCalculator>(config);
	/*
	if (name == "SupperTarrif")
		return std::make_shared<SuperTarrifCalculator>(config);
	*/
	std::stringstream str;
	str << "No factory for the given name " << name;

	throw std::exception(str.str().c_str());
}

// factoru to create instance of SubscriberInfoProvider
// support only 1 at the moment
ISubscriberInfoProviderPtr SubscriberInfoProviderFactory::getSubscriberInfoProviderByName(const std::string& name, const::std::string& config)
{
	if (name == "FSProvider")
		return std::make_shared<SubscriberInfoProviderFileSys>(config);
	/*
	if (name == "SQLProvider")
		return std::make_shared<SubscriberInfoProviderSQL>(config);
	*/
	
	std::stringstream str;
	str << "No factory for the given name " << name;

	throw std::exception(str.str().c_str());
}