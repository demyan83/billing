#pragma once
#include <string>
#include <ctime>
#include <memory>

#include "DateTime.h"
#include "ISubscriberInfoProvider.h"

class Call
{
public:
	Call() {}
	Call(const Subscriber::SubscriberID& id, 
		const std::string& calledNumber, 
		const std::string& callStart, 
		const std::string& callEnd) : 
		mSubscriberId(id), mCalledNumber(calledNumber), mCallStart(DateTime(callStart)), mCallEnd(DateTime(callEnd))
	{
	}
	void setSubscriberId(int id)
	{
		if (id > 0)
			mSubscriberId = id;
		else
			throw std::exception("Bad subscriber id provided. Must be >0");
	}

	void setCalledNumber(const std::string& number)
	{
		if (number.length() > 3)
			mCalledNumber = number;
		else
			throw std::exception("Bad called number provided. Length must be > 3");
	}
	void setCallStartEnd(const std::string& callStart, const std::string& callEnd)
	{
		DateTime start(callStart);
		DateTime end(callEnd);
		if (start < end)
		{
			mCallEnd = std::move(end);
			mCallStart = std::move(start);
		}
		else
			throw std::exception("Passed StartCall is later then EndCall");
			
	}
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
		return static_cast<unsigned int>(DateTime::getDurationInMinutesAbs(mCallEnd, mCallStart));
	}

	inline bool isStartedOnWeekend() const
	{
		return getCallStart().isWeekend();
	}

private:
	Subscriber::SubscriberID mSubscriberId;
	std::string mCalledNumber;
	DateTime mCallStart;
	DateTime mCallEnd;
};

class IBillingCalculator
{
public:
	IBillingCalculator(){}
	virtual ~IBillingCalculator() {}

	virtual double calculate(const Call& call, const Subscriber& subscriber) = 0;
};

using IBillingStrategyPtr = std::shared_ptr<IBillingCalculator>;

