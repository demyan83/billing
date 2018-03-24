#pragma once
#include <ctime>
#include <string>
#include <sstream>
#include <exception>

struct Subscriber
{
	using SubscriberID = unsigned int;

	void addUsedMinutes(unsigned short minutes)
	{
		minutes_after_last_payment += minutes;
	}
	
	unsigned short minutes_after_last_payment;
	time_t m_last_payment_date;
	std::string phone_number;
	std::string tariff;
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
		Msg_ = tmp.str();
	}
	virtual const char* what() const throw()
	{
		return Msg_.c_str();
	}

private:
	std::string Msg_;
};
