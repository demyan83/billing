#include "stdafx.h"
#include "CppUnitTest.h"

#include <string>
#include <vector>

#include "../SimpleBilling/DateTime.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BillingUnitTests
{		
	TEST_CLASS(TestDateTime)
	{
	public:
		TEST_METHOD(TestGoodInputs)
		{
			DateTime dt("2018-03-25T12:34:15");
			DateTime dt1("2014-08-16T19:34:15");
			DateTime dt2("2016-02-29T19:34:15"); // 29 Feb 2016 - is a leap year
		}

		TEST_METHOD(TestBadInputs)
		{
			std::vector<std::string> dates = {
				"2018-14-25T12:35:18",  // bad month
				"2018-03-35T12:35:18",  // bad day
				"2018-03-25T26:35:18",	// bad hour
				"2018-03-25T12:74:18",	// bad minutes
				"2018-03-25T12:35:64",	// bad seconds
				"2018-02-29T12:35:15",	// 29 Feb 2018 - 2018 is not a leap year
				"20KK-03-25T12:35:18" }; // bad literal

			for (auto date : dates)
			{
				try
				{
					DateTime dt4(date);
					std::wstring message(L"Test should have thrown");
					// Should raise an exception:  
					Assert::Fail(message.c_str(), LINE_INFO());
				}
				catch (std::out_of_range ex)
				{
					continue; // Exception, is just what we expect in case of bad params
				}
			}
			
		}
		TEST_METHOD(TestDurations)
		{
			{
				// 25 sec
				DateTime dt("2018-03-24T12:34:15");
				DateTime dtPlus25("2018-03-24T12:34:40");
				Assert::AreEqual(DateTime::getDurationInSeconds(dt, dtPlus25), 25, 0);
				Assert::AreEqual(DateTime::getDurationInMinutes(dt, dtPlus25), 1, 0);
			}
			{
				// 15 mins + 40 sec
				DateTime dt("2018-03-24T23:55:00");
				DateTime dt2("2018-03-25T00:10:40");
				Assert::AreEqual(DateTime::getDurationInSeconds(dt, dt2), 940, 0);
				Assert::AreEqual(DateTime::getDurationInMinutes(dt, dt2), 16, 0);
			}

			{
				// 1 day + 2 h + 10 m + 40 sec = 
				DateTime dt("2018-03-23T20:10:00");
				DateTime dt2("2018-03-24T22:20:40");
				Assert::AreEqual(DateTime::getDurationInSeconds(dt, dt2), 94240, 0);
				Assert::AreEqual(DateTime::getDurationInMinutes(dt, dt2), 1571, 0);
			}

		}
		TEST_METHOD(TestWeekends)
		{
			// saturday and sunday are weekend
			{
				DateTime sat("2018-03-24T12:34:15");
				Assert::IsTrue(sat.isWeekend());
			}
			{
				DateTime sun("2018-03-25T12:34:15");
				Assert::IsTrue(sun.isWeekend());
			}
			// monday and friday are not weekend
			{
				DateTime mond("2018-03-26T12:34:15");
				Assert::IsFalse(mond.isWeekend());
			}
			{
				DateTime friday("2018-06-15T12:34:15");
				Assert::IsFalse(friday.isWeekend());
			}
		}

	};
}