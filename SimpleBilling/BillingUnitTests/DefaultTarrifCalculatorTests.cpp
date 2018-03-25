#include "stdafx.h"
#include "CppUnitTest.h"

#include <string>
#include <vector>

#include "PugiXML\pugixml.hpp"
#include "PugiXML\pugixml.cpp"
#include "Factories.h"
#include "Factories.cpp"
#include "DefaultTarrifCalculator.cpp"
#include "SubscriberInfoProviderFileSys.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BillingUnitTests
{
	TEST_CLASS(TestDefCalc)
	{
	public:

		TEST_METHOD(TestBillerInstantitation)
		{
			const char* node = 
				"<Parameters FixedConnectionFee = '0.33' ChargeUnitSeconds = '60' InsideBonusMinutes = '30'  InsideBonusPeriodDays = '30' CostIn = '0.50' CostOut = '0.95' weekendCallBonusMinutes = '5'>\
				<HomePrefixes>\
				<HomePrefix>050</HomePrefix>\
				<HomePrefix>066</HomePrefix>\
				<HomePrefix>095</HomePrefix>\
				<HomePrefix>099</HomePrefix>\
				</HomePrefixes>\
				</Parameters>\"";

			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_string(node);
			const pugi::xml_node& config = doc.child("Parameters");
				

			IBillingStrategyPtr biller = BillingFactory::getBillingCalculatorByName("Default", doc);
			
			Assert::IsTrue(bool(biller));
		}
		
		// check calculation is right
		TEST_METHOD(TestCalculate)
		{
			const char* node =
				"<Parameters FixedConnectionFee = '0.33' ChargeUnitSeconds = '60' InsideBonusMinutes = '30'  \
				InsideBonusPeriodDays = '30' CostIn = '0.50' CostOut = '1' weekendCallBonusMinutes = '5'>\
				<HomePrefixes>\
				<HomePrefix>050</HomePrefix>\
				<HomePrefix>066</HomePrefix>\
				<HomePrefix>095</HomePrefix>\
				<HomePrefix>099</HomePrefix>\
				</HomePrefixes>\
				</Parameters>\"";

			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_string(node);
			const pugi::xml_node& config = doc.child("Parameters");


			IBillingStrategyPtr biller = BillingFactory::getBillingCalculatorByName("Default", doc);
			Subscriber scr;
			scr.phone_number ="4444444";
			scr.minutes_after_last_payment = 5000;
			scr.last_payment_date = DateTime("2018-01-25T12:34:15");
			scr.tariff = "Default";

			Assert::IsTrue(bool(biller));
			Call call;
			call.setCalledNumber("888888");	// external call
			call.setCallStartEnd("2018-03-29T12:34:15", "2018-03-29T12:34:25"); // 10 seconds => 1 minute
			call.setSubscriberId(15);
			double res = biller->calculate(call, scr);

			Assert::AreEqual(res, 1.33, 2);
		}
		
	};
}