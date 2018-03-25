#pragma once
#include <vector>
#include <map>

#include "PugiXML\pugixml.hpp"
#include "ISubscriberInfoProvider.h"
#include "Factories.h"
#include "Call.h"

class BillingDemoClass
{
public:
	BillingDemoClass(const std::string& subscribers, const std::string& billers, const std::string& calls)
		: mSubscribersFileName(subscribers), mBillersFileName(billers), mCallsFileName(calls) 
	{}
	~BillingDemoClass() {};

	void run();

private:
	ISubscriberInfoProviderPtr getFSSubscriberInfoProviderPtr(const std::string& filename);
	std::map<std::string, IBillingStrategyPtr> populateBillers(const std::string& filename);
	std::vector<Call> populateCalls(const std::string& filename);

private:
	std::string mSubscribersFileName;
	std::string	mBillersFileName;
	std::string	mCallsFileName;
	
};

