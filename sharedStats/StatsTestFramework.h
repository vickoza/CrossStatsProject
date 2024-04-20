#pragma once
#include <functional>
#include <concepts>
#include <string>
#include <tuple>
#include <optional>
#include <vector>
#include <memory>
#include <StatAlgorthims.h>

class StateObject
{
	virtual ~StateObject() {}
};

enum class TestState
{
	pass,
	testFail,
	testBadRun
};

template<typename T>
	requires std::derived_from<StateObject, T>
class StatsTestHarness 
{
protected:
	std::vector < std::tuple<std::function<std::unique_ptr<T>()>, std::function<bool(T&)>, std::optional<std::function<void(T&)>>, unsigned long long, double long, std::string>> tests;
public:
	virtual ~StatsTestHarness() {}
	void addTest(
		std::function<std::unique_ptr<T>()> generatorFunction,
		std::function<bool(T&)> testFunction,
		std::optional<std::function<void(T&)>> cleanupFunction,
		unsigned long long testRuns,
		double long passRate,
		std::string& testName) final
	{
		tests.emplace_back(generatorFunction, testFunction, cleanupFunction, testRuns, passRate, testName);
	}

	virtual std::vector<std::pair<std::string, TestState>> run()
	{
		std::vector<std::pair<std::string, TestState>> results;
		for (const auto& test : tests)
		{
			auto passCount = decltype(std::get<3>(test))0;
			for(auto i = decltype(std::get<3>(test))0; i < std::get<3>(test); ++i)
			{
				try
				{
					auto testObj = std::get<0>(test)();
					auto resultState = std::get<1>(test)(testObj);
					if (std::get<2>(test).has_value())
					{
						std::get<2>(test)(testObj)
					}
					if (resultState)
					{
						++passCount;
					}
				}
				catch (...)
				{
					results.emplace_back(std::get<5>(test), TestState::testBadRun);
					break;
				}
			}
			auto passScore = decltype(std::get<4>(test))(passCount) / std::get<3>(test);
			if(std::get<4>(test) > 0.0)
			{
				if (passScore >= std::get<4>(test))
				{
					results.emplace_back(std::get<5>(test), TestState::pass);
				}
				else
				{
					results.emplace_back(std::get<5>(test), TestState::testFail);
				}
			}
			else if (std::get<4>(test) < 0.0)
			{
				if (passScore <= -(std::get<4>(test)))
				{
					results.emplace_back(std::get<5>(test), TestState::pass);
				}
				else
				{
					results.emplace_back(std::get<5>(test), TestState::testFail);
				}
			}
			else
			{
				results.emplace_back(std::get<5>(test), TestState::testBadRun);
			}
		}
	}
};