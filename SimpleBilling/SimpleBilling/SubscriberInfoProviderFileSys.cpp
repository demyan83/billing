#include "stdafx.h"
#include <cctype>

#include "SubscriberInfoProviderFileSys.h"
#include "PugiXML\pugixml.hpp"

SubscriberInfoProviderFileSys::SubscriberInfoProviderFileSys(const std::string& file) : mSourceFileName(file)
{
	loadSubscribersFromFile(mSourceFileName);
}

Subscriber SubscriberInfoProviderFileSys::getSubscriberInfo(const Subscriber::SubscriberID& id)
{
	using MapIt = std::map<Subscriber::SubscriberID, Subscriber>::const_iterator;

	MapIt lookup = mSubscribers.find(id);
	if (lookup == mSubscribers.end())
	{
		throw UnknownCaller(id);
	}
	return lookup->second;
	
}

void SubscriberInfoProviderFileSys::loadSubscribersFromFile(const std::string& filename)
{
	std::cout << "Loading subscribers list from " << filename << std::endl;

	std::map<Subscriber::SubscriberID, Subscriber> subscribers;

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename.c_str());
	if (result)
	{
		for (pugi::xml_node subscNode : doc.child("Subscribers").children("Subscriber"))
		{
			int id = 0;  
			Subscriber sbscr;

			for (pugi::xml_attribute attr : subscNode.attributes())
			{
				std::string atrname(attr.name());
				for (auto &elem : atrname)
					elem = std::tolower(elem);

				if (atrname == "id")
					id = attr.as_int();
				else if (atrname == "lastpaymentdate")
					sbscr.last_payment_date = DateTime(attr.as_string());
				else if (atrname == "tarifname")
					sbscr.tariff = attr.as_string();
				else if (atrname == "phonenumber")
					sbscr.phone_number = attr.as_string();
				else if (atrname == "minutesused")
					sbscr.minutes_after_last_payment = attr.as_int();
				else if (atrname == "subscribername")
					sbscr.subscriberName = attr.as_string();
				
			}
			// add element into the storage
			if (id)
			{
				if (subscribers.find(id) != subscribers.end())
					throw std::exception("Duplicate entry in subscribers list");

				subscribers[id] = sbscr;
			}
				
		}
		mSubscribers.swap(subscribers);
		std::cout << "    Loaded " << mSubscribers.size() << " subscribers" << std::endl;
	}
}