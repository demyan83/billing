#pragma once
#include <map>
#include <string>

#include "ISubscriberInfoProvider.h"


class SubscriberInfoProviderFileSys :
	public ISubscriberInfoProvider
{
public:
	explicit SubscriberInfoProviderFileSys(const std::string& file);
	virtual ~SubscriberInfoProviderFileSys() {}

	virtual Subscriber getSubscriberInfo(const Subscriber::SubscriberID& id);

private:
	void loadSubscribersFromFile(const std::string& file);

private:
	std::string mSourceFile;
	std::map<Subscriber::SubscriberID, Subscriber> mSubscribers;
};

