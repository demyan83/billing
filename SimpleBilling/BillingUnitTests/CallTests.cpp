#include "stdafx.h"
#include "CppUnitTest.h"

#include "Call.h"
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BillingUnitTests
{

	TEST_CLASS(TestCall)
	{
	public:
		TEST_METHOD(TestGoodInputsCall)
		{
			Call one;
			std::string num("123456");
			unsigned int id = 150;
			unsigned int dur = 1;

			one.setCalledNumber(num);
			one.setCallStartEnd("2018-03-25T12:34:15", "2018-03-25T12:34:25");
			one.setSubscriberId(id);
			Assert::AreEqual(one.getCallerId(), id);
			Assert::AreEqual(one.getCalledNumber(), num);
			Assert::AreEqual(one.getCallDuration(), dur);
		}

		TEST_METHOD(TestBadInputsCall)
		{
			std::wstring message(L"Test should have thrown");

			Call one;
			std::string num("123456");
			unsigned int id = 150;
			unsigned int dur = 10;

			try
			{
				one.setCalledNumber("11");
				// Should raise an exception:  
				Assert::Fail(message.c_str(), LINE_INFO());
			}
			catch (std::exception&)
			{
				// Exception, is just what we expect in case of bad params
			}

			try
			{
				one.setSubscriberId(0);
				// Should raise an exception:  
				Assert::Fail(message.c_str(), LINE_INFO());
			}
			catch (std::exception&)
			{
				// Exception, is just what we expect in case of bad params
			}

			try
			{
				one.setCallStartEnd("2018-04-25T12:34:15", "2018-03-25T12:34:25");
				Assert::Fail(message.c_str(), LINE_INFO());
			}
			catch (std::exception&)
			{
				// Exception, is just what we expect in case of bad params
			}
		}
	};
}