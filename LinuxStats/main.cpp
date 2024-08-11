#include "StatAlgorthims.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <cstdio>

int main(int argc, char* argv[])
{
    std::vector<long double> dataSet;
    for(auto i{1}; i < argc; ++i)
    {
        char* endptr;
        errno = 0; // Reset errno before the call
        auto value = std::strtold(argv[i], &endptr);
        if (errno == ERANGE || *endptr != '\0')
        {
            std::cerr << "invalid data arugment\n";
            return 1;
        }
        dataSet.emplace_back(value);
    }
    std::cout <<"mean: " << stats::compute_mean(dataSet.begin(), dataSet.end()) 
        <<" standard deviation: " << stats::compute_standard_deviation(dataSet.begin(), dataSet.end())
        <<" variance: " <<stats::compute_variance(dataSet.begin(), dataSet.end()) << '\n';
    std::cout << "standard deviation pop: " << stats::compute_standard_deviation_population(dataSet.begin(), dataSet.end())
        << " variance pop: " << stats::compute_variance_population(dataSet.begin(), dataSet.end()) << '\n';
    printf("hello from %s!\n", "LinuxStats");
    return 0;
}