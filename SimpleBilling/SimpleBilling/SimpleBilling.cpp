// SimpleBilling.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <chrono>

#include "DateTime.h"

#include "SubscriberInfoProviderFileSys.h"

int main()
{
	DateTime dt_("2018-01-01T12:34:15");
	DateTime dt("2018-03-32T12:34:15");
	DateTime dt1("2018-03-25T12:35:18");
	DateTime dt2("2018-15-25T12:35:18");

	DateTime dt3("2018-15-33T12:35:18");
	DateTime dt4("2018-15-25T28:35:18");
	DateTime dt5("2018-15-25T12:65:18");
	DateTime dt6("2018-15-25T12:35:70");

	int diff = DateTime::getDurationInSeconds(dt1, dt);
	int diffM = DateTime::getDurationInMinutes(dt1, dt);
	
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

