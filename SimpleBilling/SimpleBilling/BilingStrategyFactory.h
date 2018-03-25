#pragma once
#include "IBillingCalculator.h"
#include "DefaultTarrifCalculator.h"

IBillingStrategyPtr getBillingCalculatorByName(const std::string& name, const pugi::xml_node& config)
{
	if (name == "Default")
		return std::make_shared<DefaultTarrifCalculator>(config);

	throw std::exception("No factory for given name");
}