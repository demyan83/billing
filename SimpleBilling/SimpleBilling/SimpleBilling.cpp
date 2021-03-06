// SimpleBilling.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <cctype>

#include "BillingDemoClass.h"


int main()
{
	std::string subs, tariffs, calls;

	std::cout << "Please enter Subscribers source(or Enter for Subscribers.xml): ";
	std::getline(std::cin, subs);
	if (subs.empty())
	{
		subs = "Subscribers.xml";
		std::cout << subs << std::endl;
	}
		
	std::cout << "Please enter Tariffs source(or Enter for Tariffs.xml): ";
	std::getline(std::cin, tariffs);
	if (tariffs.empty())
	{
		tariffs = "Tariffs.xml";
		std::cout << tariffs<< std::endl;
	}
		
	std::cout << "Please enter Calls source(or Enter for CallsList.xml): ";
	std::getline(std::cin, calls);
	if (calls.empty())
	{
		calls = "CallsList.xml";
		std::cout << calls << std::endl;
	}
		
	BillingDemoClass demo(subs, tariffs, calls);
	demo.run();
    return 0;
}

