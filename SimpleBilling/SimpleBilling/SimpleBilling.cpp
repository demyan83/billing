// SimpleBilling.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <cctype>

#include "PugiXML\pugixml.hpp"
#include "DateTime.h"
#include "SubscriberInfoProviderFileSys.h"
#include "BilingStrategyFactory.h"

ISubscriberInfoProviderPtr getFSSubscriberInfoProviderPtr(const std::string& filename);
std::map<std::string, IBillingStrategyPtr> populateBillers(const std::string& filename);
std::vector<Call> populateCalls();

int main()
{
	ISubscriberInfoProviderPtr infoProvider = getFSSubscriberInfoProviderPtr("Subscribers.xml");
	std::map<std::string, IBillingStrategyPtr> billers = populateBillers("tariffs.xml");
	std::vector<Call> calls = populateCalls();
	
	for (auto call : calls)
	{
		try
		{
			Subscriber sbcr = infoProvider->getSubscriberInfo(call.getCallerId());
			IBillingStrategyPtr biller = billers[sbcr.tariff];
			double result = biller->calculate(call, sbcr);
			
			std::cout << result << std::endl;
		}
		catch (UnknownCaller& e)
		{
			std::cout << e.what();
		}
	}
	std::string pause;
	std::cin >> pause;

    return 0;
}

std::map<std::string, IBillingStrategyPtr> populateBillers(const std::string& filename)
{
	std::map<std::string, IBillingStrategyPtr> billers;

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename.c_str());
	if (result)
	{
		for (pugi::xml_node planNode : doc.child("Tariffs").children("Plan"))
		{
			std::string planName = planNode.attribute("Name").as_string();
			billers[planName] = getBillingCalculatorByName(planName, planNode);
		}
	}
	
	return std::move(billers);
}

std::vector<Call> populateCalls()
{
	std::vector<Call> calls;
	
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("CallsList.xml");
	if (result)
	{
		for (pugi::xml_node subscNode : doc.child("Calls").children("Call"))
		{
			Call onecall;
			std::string callstart, callend;

			for (pugi::xml_attribute attr : subscNode.attributes())
			{
				std::string atrname(attr.name());
				for (auto &elem : atrname)
					elem = std::tolower(elem);

				if (atrname == "subscriberid")
					onecall.setSubscriberId(attr.as_int());
				else if (atrname == "callednumber")
					onecall.setCalledNumber(attr.as_string());
				else if (atrname == "callstart")
					callstart = attr.as_string();
				else if (atrname == "callend")
					callend = attr.as_string();
			}
			onecall.setCallStartEnd(callstart, callend);
			calls.push_back(onecall);
		}
	}
	return std::move(calls);
}

ISubscriberInfoProviderPtr getFSSubscriberInfoProviderPtr(const std::string& filename)
{
	return std::make_shared<SubscriberInfoProviderFileSys>(filename);
}