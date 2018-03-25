#include "stdafx.h"

#include <vector>
#include <iostream>
#include <chrono>
#include <cctype>

#include "DateTime.h"
#include "BillingDemoClass.h"
#include "Factories.h"
#include "SubscriberInfoProviderFileSys.h"

void BillingDemoClass::run()
{
	ISubscriberInfoProviderPtr infoProvider = getFSSubscriberInfoProviderPtr(mSubscribersFileName);
	std::map<std::string, IBillingStrategyPtr> billers = populateBillers(mBillersFileName);
	std::vector<Call> calls = populateCalls(mCallsFileName);

	std::cout << "Start processing calls" << std::endl;
	for (auto call : calls)
	{
		try
		{
			std::cout << "	Start:" << std::endl;

			Subscriber sbcr = infoProvider->getSubscriberInfo(call.getCallerId());
			IBillingStrategyPtr biller = billers[sbcr.tariff];
			double result = biller->calculate(call, sbcr);

			std::cout << "	End: Call cost for \"" << sbcr.subscriberName << "\" has been calculated: " << result << std::endl << std::endl;
		}
		catch (UnknownCaller& e)
		{
			std::cout << e.what();
		}
	}
	std::string pause;
	std::cout << "Finished";
	std::cin >> pause;
}

ISubscriberInfoProviderPtr BillingDemoClass::getFSSubscriberInfoProviderPtr(const std::string& filename)
{
	return SubscriberInfoProviderFactory::getSubscriberInfoProviderByName("FSProvider", filename);
}

std::map<std::string, IBillingStrategyPtr> BillingDemoClass::populateBillers(const std::string& filename)
{
	std::cout << "Loading billers list from " << filename << std::endl;

	std::map<std::string, IBillingStrategyPtr> billers;

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename.c_str());
	if (result)
	{
		for (pugi::xml_node planNode : doc.child("Tariffs").children("Plan"))
		{
			std::string planName = planNode.attribute("Name").as_string();
			std::string plugin = planNode.attribute("Plugin").as_string();
			IBillingStrategyPtr biller = BillingFactory::getBillingCalculatorByName(plugin, planNode);
			billers[planName] = biller;
			std::cout << "    Loaded biller [" << planName << "], plugin is [" << biller->name() << "]" << std::endl;
		}
	}

	return std::move(billers);
}

std::vector<Call> BillingDemoClass::populateCalls(const std::string& filename)
{
	std::cout << "Loading calls info from " << filename << std::endl;

	std::vector<Call> calls;

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename.c_str());
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
	std::cout << "    Loaded " << calls.size() << std::endl;

	return std::move(calls);
}