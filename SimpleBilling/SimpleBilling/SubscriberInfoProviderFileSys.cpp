#include "stdafx.h"
#include "SubscriberInfoProviderFileSys.h"


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

void SubscriberInfoProviderFileSys::loadSubscribersFromFile(const std::string& file)
{

}