#pragma once
#include <atomic>
#include <mutex>
#include <queue>
#include <cassert>
#include <variant>
#include <type_traits>

class readerLock;
class writerLock;

class readerWriterMutex
{
	friend readerLock;
	friend writerLock;
	std::atomic<unsigned long long> readCount;
	std::atomic<bool> writing;
	std::queue<writerLock*> writeQue;
	std::mutex m;
public:
	readerWriterMutex() : readCount{ 0 }, writing{ false } {}
};

class readerLock
{
	readerWriterMutex& mut;
public:
	readerLock() = delete;
	readerLock(readerWriterMutex& mutex) : mut{ mutex }
	{
		while (mut.writing);
		++(mut.readCount);
	}
	~readerLock() { --(mut.readCount); }
};

class writerLock
{
	readerWriterMutex& mut;
public:
	writerLock() = delete;
	writerLock(readerWriterMutex& mutex) : mut{ mutex }
	{
		{
			std::lock_guard<std::mutex> loc(mut.m);
			mut.writeQue.emplace(this);
		}
		do
		{
			while (mut.readCount > 0 || mut.writing == true);
		} 
		while (reinterpret_cast<std::uintptr_t>(mut.writeQue.front()) != reinterpret_cast<std::uintptr_t>(this));
		mut.writing = true;
	}
	~writerLock()
	{
		mut.writing = false;
		assert(mut.writeQue.front() == this);
		mut.writeQue.pop();
	}
};

template <typename T>
std::variant<readerLock, writerLock> createReaderWriterLock(T& obj, readerWriterMutex& mut)
{
	//static_assert(!std::is_pointer<T>);
	if constexpr (!std::is_const<T>())
	{
		return std::variant<readerLock, writerLock>(std::in_place_type_t<writerLock>(), mut );
	}
	else
	{
		return std::variant<readerLock, writerLock>(std::in_place_type_t<readerLock>(), mut);
	}
}