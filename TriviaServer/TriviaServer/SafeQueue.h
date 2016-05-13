#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

template <class T>
class SafeQueue
{
public:
	SafeQueue();
	void enqueue(const T&);
	T dequeue();

private:
	std::queue<T> _queue;
	std::mutex _mutex;
	std::condition_variable _condition;
};

template<class T>
SafeQueue<T>::SafeQueue() {}

template<class T>
void SafeQueue<T>::enqueue(const T& value)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_queue.push(value);
	_condition.notify_one();
}

template<class T>
T SafeQueue<T>::dequeue()
{
	std::unique_lock<std::mutex> lock(_mutex);
	while (_queue.empty())
		_condition.wait(lock);
	T value = _queue.front();
	_queue.pop();
	return value;
}
