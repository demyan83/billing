// SimpleBilling.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "SubscriberInfoProviderFileSys.h"

int main()
{
	SubscriberInfoProviderFileSys fff("");
	try
	{
		fff.getSubscriberInfo(50);
	}
	catch (UnknownCaller& e)
	{
		std::cout << e.what();
	}
	

    return 0;
}

