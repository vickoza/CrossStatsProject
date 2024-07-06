// ReaderWriterLockTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <random>
#include <thread>
//#include "StatsTestFramework.h"
#include "readerwriter.h"

auto getRandomValue()
{
    static std::random_device rd;  // a seed source for the random number engine
    static std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    static std::uniform_int_distribution<> distrib(1, 10);

    return distrib(gen);
}

void readInt(const int& i)
{
    for (auto j{ 0 }; j < 1'000'000; ++j)
    {
        //++i;
    }
}

void writeInt(int& i)
{
    for (auto j{0}; j < 1'000'000; ++j)
    {
        i += 1;
        std::this_thread::sleep_for(std::chrono::nanoseconds(getRandomValue()));
        i += 1;
    }
}

void readIntLock(const int& i, readerWriterMutex& mut)
{
    for (auto j{ 0 }; j < 1'000'000; ++j)
    {
        auto lock = createReaderWriterLock(i, mut);
        //++i;
    }
}

void writeIntLock(int& i, readerWriterMutex& mut)
{
    for (auto j{ 0 }; j < 1'000'000; ++j)
    {
        auto lock = createReaderWriterLock(i, mut);
        i += 1;
        std::this_thread::sleep_for(std::chrono::nanoseconds(getRandomValue()));
        i += 1;
    }
}

int main()
{

    auto i{ 0 };
    {
        std::jthread jt0{ writeInt, std::ref(i) };
        std::jthread jt1{ readInt, std::ref(i) };
        std::jthread jt2{ readInt, std::ref(i) };
        std::jthread jt3{ writeInt, std::ref(i) };
        std::jthread jt4{ readInt, std::ref(i) };
        std::jthread jt5{ readInt, std::ref(i) };
    }
    auto j{ 0 };
    auto mut = readerWriterMutex{};
    {
        std::jthread jt0{ writeIntLock, std::ref(j), std::ref(mut)};
        std::jthread jt1{ readIntLock, std::ref(j), std::ref(mut) };
        std::jthread jt2{ readIntLock, std::ref(j), std::ref(mut) };
        std::jthread jt3{ writeIntLock, std::ref(j), std::ref(mut) };
        std::jthread jt4{ readIntLock, std::ref(j), std::ref(mut) };
        std::jthread jt5{ readIntLock, std::ref(j), std::ref(mut) };
    }
    std::cout << i << "<=>" << j << '\n';
    std::cout << "Hello World!\n";
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
