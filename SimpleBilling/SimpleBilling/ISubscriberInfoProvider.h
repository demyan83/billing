#pragma once
#include <ctime>
#include <string>
#include <sstream>
#include <exception>

#include "DateTime.h"

struct Subscriber
{
	using SubscriberID = unsigned int;
	using TariffID = std::string;

	void addUsedMinutes(unsigned short minutes)
	{
		minutes_after_last_payment += minutes;
	}
	
	unsigned short	minutes_after_last_payment;
	DateTime		last_payment_date;
	std::string		phone_number;
	TariffID		tariff;
};

class ISubscriberInfoProvider
{
public:
	ISubscriberInfoProvider();
	virtual ~ISubscriberInfoProvider();

	virtual Subscriber getSubscriberInfo(const Subscriber::SubscriberID& id) = 0;
};


class UnknownCaller : public std::exception
{
public:
	explicit UnknownCaller(const Subscriber::SubscriberID& id_passed)
	{
		std::ostringstream tmp;
		tmp << "Subscriber with the given Id [" << id_passed << "] is not registered";
		msg_ = tmp.str();
	}
	virtual const char* what() const throw()
	{
		return msg_.c_str();
	}

private:
	std::string msg_;
};
