#pragma once
#include <cmath>
#include <vector>
#include <iterator>
#include <concepts>
#include <functional>

namespace stats
{
	template<typename iter, typename T>
	concept mean_computable = requires(T a, T b, iter it, std::size_t i)
	{
		{*it}->std::convertible_to <T>;
		{++it}->std::convertible_to <iter&>;
		{a + b}->std::convertible_to<T>;
		{a += b}->std::convertible_to<T&>;
		{a / i}->std::convertible_to<T>;
	};

	template<typename T>
	concept square_computable = requires(T a, T b)
	{
		{a * b}->std::convertible_to<T>;
		{a * 0}->std::convertible_to<T>;
		{a - b}->std::convertible_to<T>;
	};

	template<typename T, typename Fun>
	concept square_root_computable = requires(T a, Fun fun)
	{
		{fun(a)}->std::convertible_to<T>;
	};

	template<typename T>
	concept type_divisible = requires(T a, T b)
	{
		{a / b}->std::convertible_to<T>;
	};

	template<typename IterT>
	constexpr typename std::iterator_traits<IterT>::value_type compute_mean(IterT start, IterT end) requires mean_computable<IterT, typename std::iterator_traits<IterT>::value_type>
	{
		auto sum = *start * 0.0;
		std::size_t count = 0;
		while (start != end)
		{
			sum += *start;
			++count;
			++start;
		}
		return sum / count;
	}

	template<typename IterT, typename Fun>
	constexpr typename std::iterator_traits<IterT>::value_type compute_standard_deviation(IterT start, IterT end, Fun func) requires mean_computable<IterT, typename std::iterator_traits<IterT>::value_type> && square_computable<typename std::iterator_traits<IterT>::value_type> && square_root_computable <typename std::iterator_traits<IterT>::value_type,Fun>
	{
		auto begin = start;
		auto sum = *start * 0.0;
		std::vector<typename std::iterator_traits<IterT>::value_type> temp;
		while (start != end)
		{
			temp.emplace_back(*start);
			sum += *start;
			++start;
		}
		auto mean = sum / temp.size();
		auto variance = *begin * 0.0;
		for (const auto& val : temp)
		{
			variance +=(val-mean)*(val-mean);
		}
		return func(variance/(temp.size()-1));
	}

	template<typename IterT>
	constexpr typename std::iterator_traits<IterT>::value_type compute_standard_deviation(IterT start, IterT end) requires mean_computable<IterT, typename std::iterator_traits<IterT>::value_type> && square_computable<typename std::iterator_traits<IterT>::value_type> && std::floating_point<typename std::iterator_traits<IterT>::value_type>
	{
		return compute_standard_deviation(start, end, [](auto val){return std::sqrt(val);});
	}

	template<typename IterT>
	constexpr typename std::iterator_traits<IterT>::value_type compute_variance(IterT start, IterT end) requires mean_computable<IterT, typename std::iterator_traits<IterT>::value_type> && square_computable<typename std::iterator_traits<IterT>::value_type>
	{
		auto begin = start;
		auto sum = *start * 0.0;
		std::vector<typename std::iterator_traits<IterT>::value_type> temp;
		while (start != end)
		{
			temp.emplace_back(*start);
			sum += *start;
			++start;
		}
		auto mean = sum / temp.size();
		auto variance = *begin * 0.0;
		for (const auto& val : temp)
		{
			variance +=(val-mean)*(val-mean);
		}
		return variance/(temp.size()-1);
	}

	template<typename IterT, typename Fun>
	constexpr typename std::iterator_traits<IterT>::value_type compute_standard_deviation_population(IterT start, IterT end, Fun func) requires mean_computable<IterT, typename std::iterator_traits<IterT>::value_type>&& square_computable<typename std::iterator_traits<IterT>::value_type>&& square_root_computable <typename std::iterator_traits<IterT>::value_type, Fun>
	{
		auto begin = start;
		auto sum = *start * 0.0;
		std::vector<typename std::iterator_traits<IterT>::value_type> temp;
		while (start != end)
		{
			temp.emplace_back(*start);
			sum += *start;
			++start;
		}
		auto mean = sum / temp.size();
		auto variance = *begin * 0.0;
		for (const auto& val : temp)
		{
			variance += (val - mean) * (val - mean);
		}
		return func(variance / (temp.size()));
	}

	template<typename IterT>
	constexpr typename std::iterator_traits<IterT>::value_type compute_standard_deviation_population(IterT start, IterT end) requires mean_computable<IterT, typename std::iterator_traits<IterT>::value_type>&& square_computable<typename std::iterator_traits<IterT>::value_type>&& std::floating_point<typename std::iterator_traits<IterT>::value_type>
	{
		return compute_standard_deviation_population(start, end, [](auto val) {return std::sqrt(val); });
	}

	template<typename IterT>
	constexpr typename std::iterator_traits<IterT>::value_type compute_variance_population(IterT start, IterT end) requires mean_computable<IterT, typename std::iterator_traits<IterT>::value_type>&& square_computable<typename std::iterator_traits<IterT>::value_type>
	{
		auto begin = start;
		auto sum = *start * 0.0;
		std::vector<typename std::iterator_traits<IterT>::value_type> temp;
		while (start != end)
		{
			temp.emplace_back(*start);
			sum += *start;
			++start;
		}
		auto mean = sum / temp.size();
		auto variance = *begin * 0.0;
		for (const auto& val : temp)
		{
			variance += (val - mean) * (val - mean);
		}
		return variance / (temp.size());
	}

	template<typename IterT, typename Fun>
	constexpr typename std::iterator_traits<IterT>::value_type compute_skewness(IterT start, IterT end, Fun func) requires mean_computable<IterT, typename std::iterator_traits<IterT>::value_type> && square_computable<typename std::iterator_traits<IterT>::value_type> && square_root_computable <typename std::iterator_traits<IterT>::value_type,Fun>
	{
		auto begin = start;
		auto sum = *start * 0.0;
		std::vector<typename std::iterator_traits<IterT>::value_type> temp;
		while (start != end)
		{
			temp.emplace_back(*start);
			sum += *start;
			++start;
		}
		auto mean = sum / temp.size();
		auto variance = *begin * 0.0;
		auto sumDiffCubed = *begin * 0.0;
		for (const auto& val : temp)
		{
			auto temp = (val-mean)*(val-mean);
			variance += temp;
			sumDiffCubed += temp * (val-mean);
		}
		auto standardDeviation = func(variance/(temp.size()-1));
		return sumDiffCubed / ( standardDeviation * standardDeviation * standardDeviation *  (temp.size()-1));
	}

	template<typename IterT>
	constexpr typename std::iterator_traits<IterT>::value_type compute_skewness(IterT start, IterT end) requires mean_computable<IterT, typename std::iterator_traits<IterT>::value_type> && square_computable<typename std::iterator_traits<IterT>::value_type> && std::floating_point<typename std::iterator_traits<IterT>::value_type>
	{
		return compute_skewness(start, end, [](auto val){return std::sqrt(val);});
	}

	/*template<typename IterT>
	constexpr typename std::iterator_traits<IterT>::value_type compute_kurtosis(IterT start, IterT end) requires mean_computable<IterT, typename std::iterator_traits<IterT>::value_type> && square_computable<typename std::iterator_traits<IterT>::value_type> 
	{
		auto begin = start;
		auto sum = *start * 0.0;
		std::vector<typename std::iterator_traits<IterT>::value_type> temp;
		while (start != end)
		{
			temp.emplace_back(*start);
			sum += *start;
			++start;
		}
		auto mean = sum / temp.size();
		auto varianceNumerator = *begin * 0.0;
		auto sumDiff4th = *begin * 0.0;
		for (const auto& val : temp)
		{
			auto temp = (val-mean)*(val-mean);
			varianceNumerator += temp;
			sumDiff4th += temp * temp;
		}
		return (( sumDiff4th * (temp.size()-1) ) / ( varianceNumerator * varianceNumerator )) - 3.0;
	}*/
}

