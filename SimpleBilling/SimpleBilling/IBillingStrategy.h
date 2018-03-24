#pragma once
#include <string>
#include <ctime>

#include "ISubscriberInfoProvider.h"

class Call
{
public:
	time_t getCallStart() const
	{
		return mCallStart;
	}

	time_t getCallEnd() const
	{
		return mCallEnd;
	}

	unsigned int getCallDuration() const
	{
		return static_cast<unsigned int>(std::ceil(mCallEnd - mCallStart));
	}
	bool isWeekend() const
	{
		return false;
		//return mCallStart.isWeekend();
	}

	Subscriber::SubscriberID getCallerId() const
	{
		return mSubscriberId;
	}

	std::string getCalledNumber() const
	{
		return mCalledNumber;
	}
private:
	time_t mCallStart;
	time_t mCallEnd;
	std::string mCalledNumber;
	Subscriber::SubscriberID mSubscriberId;
};

class IBillingStrategy
{
public:
	IBillingStrategy();
	virtual ~IBillingStrategy();

	virtual double calculate(const Call& call, const Subscriber& subscriber) = 0;
};

