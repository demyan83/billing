



typedef in CallerID_t;
struct Subscriber
{
	void addUsedMinutes(int minutes)
	{
		minutesUsedAfterLastPayment += minutes;
	}
	boost::date_time lastPaymentDate;
	int minutesUsedAfterLastPayment;
	std::string phoneNumber;
	std::string Tariff;
}

// represent Iface of retrieving data from the subscribers data source (sql, cache, file..)
class ISubscriberInfoProvider
{
	virtual Subscriber getSubscriberInfo(CallerID_t id) = 0;
}

class FSProvider : public ISubscriberInfoProvider
{
public:
	explicit FSProvider(const std::string& filename)
	{
		parse();
	}
	virtual Subscriber getSubscriberInfo(CallerID_t id)
	{
		// find
		// throw if couldn't find
		return m_subscribers[id];
	}
private:	
	void parse()
	{
		//throw if couldn't open or parse
	}
	// hashmap may be better
	std::map<int, Subscriber> m_subscribers;
}

class DBProvider : public ISubscriberInfoProvider
{
public:
	explicit DBProvider(const std::string& connectionstring)
	{
		connect();
		retrieveInfo();
	}
	virtual Subscriber getSubscriberInfo(CallerID_t id)
	{
		// find
		// throw if couldn't find
		return m_subscribers[id];
	}
private:
	void connect(const std::string& connectionstring)
	{
		//throw if couldn't connect
	}
	void retrieveInfo()
	{
		// fill in m_subscribers
	}
	// hashmap may be better
	std::map<int, Subscriber> m_subscribers;
}

std::map<int, Subscriber> Subscribers;

struct Call
{
	int getCallDuration()
	{
		return std::ceil(end - start);
	}
	bool isWeekend()
	{
		return start.isWeekend();
	}
	
	CallerID_t getCallerId() const
	{
		return subscriberId;
	}
	
private:
	boost::date_time start;
	boost::date_time end;
	string number;
	CallerID_t subscriberId;
}
	
class IBillingStrategy
{
	virtual int calculate(const Call& call, ISubscriberInfoProvider* provider) = 0;
}

/*
< Calculating call cost, given date and time for its start and end, number called, and subscriber account information.
 Following payment plan is valid:
o Fixed connection fee of $0.33 that is added to any call cost.
o The minute fee is charged at the beginning of each minute, so if call duration is 1:03, two minutes cost should be paid.
o Each subscriber has 30 minutes of free talking inside his operator network, valid for 30 days since the date when last credit was added.
o After free minutes expire, calls inside home operator network are charged $0.50 per minute.
o When calling numbers outside of the home network, the minute cost is $0.95.
o On weekends, first five minutes of every call are free.
 Home network of the operator is defined as a set of phone numbers starting with one of the given prefixes: 050, 066, 095, and 099.

*/

class DefaultBillingStrategy : public IBillingStrategy
{
public:
	virtual int calculate(const Call& call, ISubscriberInfoProvider* provider)
	{
		Subscriber sbccr = provider->getSubscriberInfo(CALLER_ID);
		
		const int callDurationToPrice = call.getCallDuration();
		
		double cost = 0;
		{
			// unconditional charges
			cost += FixedConnectionFee;
		}
		
		if(call.start.isWeekend())
		{
			// it is weekend, and call lasted less than free period
			callDurationToPrice-= weekendCallBonusMinutes;
			if(callDurationToPrice < 0)
				return cost;
		}
		

		bool isInternal = isInternalCall(sbccr.phoneNumber, call.phoneNumber);
		if(isInternal)
		{
			// call to the internal network, check if we have bonus minutes to use
			int bonusMinutes = getBonusMinutes(sbccr, call.start);
			
			// TODO = may throw, transact it
			sbccr.addUsedMinutes(callDurationToPrice);
			if(bonusMinutes > 0)
			{
				if(bonusMinutes >= callDurationToPrice)
				{
					return cost;
				}
				else
				{
					callDurationToPrice -= bonusMinutes;
				}
			}
		}
		
		// now we know how many minutes must be charged
		cost += callDurationToPrice * (isInternal? CostIn : CostOut);
		
		return cost;
	}
	
private:
	inline bool isInternalCall(const std::string& ownNumber, const std::string& calledNumber)
	{
		// compare prefixes
		return ownNumber.compare(calledNumber, 0, 3);
	}
	
	inline int getBonusMinutes(const Subscriber& sbscr, const boost::date_time& start)
	{
		int bonusMinsLeft = 0;
		 if((sbccr.lastPaymentDate - start) < InsideBonusPeriodDays) &&	(InsideBonusMinutes > sbccr.minutesUsedAfterLastPayment))
		{
			bonusMinsLeft = InsideBonusMinutes - sbccr.minutesUsedAfterLastPayment;
		}
		return 	bonusMinsLeft;
	}
	
private:
	double FixedConnectionFee=0.33;
	int	ChargeUnitSeconds=60;
	int InsideBonusMinutes=30;
	int InsideBonusPeriodDays=30;
	double CostIn=0.50;
	double CostOut=0.95;
	int weekendCallBonusMinutes=5;
}

class BillingCalculator
{
	public:
	static void calcCalls(const std::vector<Call>& calls)
	{
		ISubscriberInfoProvider* prov = FSProvider("clients.dat");
		for(auto call : calls)
		{
			IBillingStrategy* str = BilingFactory::get(prov->getSubscriberInfo(call.getCallerId()).Tariff);
			str->calculate(call);
			
		}
	}
}

		//// read date time from string
		//std::istringstream ss(date_time);
		//try
		//{
		//	//ss >> std::get_time(&mDateTime, "%Y-%m-%dT%H:%M:%S");
		//}
		//catch (...)
		//{
		//	// whatever may happen on parsing, we want only 1 type of exception out
		//	throw std::out_of_range("1-String passed in DateTime doesn't represent datetime");
		//}

		//if (!isValid(mDateTime))
		//{
		//	throw std::out_of_range("2-String passed in DateTime doesn't represent datetime");
		//}