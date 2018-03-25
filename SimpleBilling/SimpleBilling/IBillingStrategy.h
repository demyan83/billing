#pragma once
#include <string>
#include <ctime>

#include "DateTime.h"
#include "ISubscriberInfoProvider.h"

class Call
{
public:
	DateTime getCallStart() const
	{
		return mCallStart;
	}

	DateTime getCallEnd() const
	{
		return mCallEnd;
	}

	Subscriber::SubscriberID getCallerId() const
	{
		return mSubscriberId;
	}

	std::string getCalledNumber() const
	{
		return mCalledNumber;
	}

	unsigned int getCallDuration() const
	{
		return static_cast<unsigned int>(DateTime::getDurationInMinutes(mCallEnd, mCallStart));
	}

	inline bool isStartedOnWeekend() const
	{
		return getCallStart().isWeekend();
	}

private:
	DateTime mCallStart;
	DateTime mCallEnd;
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

