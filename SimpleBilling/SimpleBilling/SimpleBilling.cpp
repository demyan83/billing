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
	BillingDemoClass demo("Subscribers.xml", "Tariffs.xml", "CallsList.xml");
	demo.run();
    return 0;
}

