// statsConsuleTestApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>
#include <complex>
#include "StatAlgorthims.h"

int main()
{
    int a = 1;
    int b = 1;
    int* ptrA = &a;
    int* ptrB = &b;
    std::cout << (ptrA == ptrB) <<'\n';
    std::cout << "Hello World!\n";
    std::vector<double> vect{3, 8, 10, 17, 24, 27};//{1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8};
    std::vector<int> vecti{11,12,13,14,15,16,17,18};
    std::istream_iterator<double> cin_it(std::cin);
    std::istream_iterator<double> eof;
    std:: cout << stats::compute_mean(cin_it, eof) << '\n';
    std:: cout << stats::compute_mean(vect.cbegin(), vect.cend()) << '\n';
    std:: cout << stats::compute_standard_deviation(vect.cbegin(), vect.cend()) << '\n';
    std:: cout << stats::compute_skewness(vect.cbegin(), vect.cend()) << '\n';
    //std:: cout << stats::compute_kurtosis(vect.cbegin(), vect.cend()) << '\n';
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
