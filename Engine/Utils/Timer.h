#pragma once
#include <chrono>
#include <thread>
#include <functional>

struct EventTimer
{
	float time;
	std::function<void()> event;

	EventTimer(float time, std::function<void()> event)
	{
		this->time = time;
		this->event = event;
	}
};

struct Timer
{
	void Start(std::chrono::microseconds delta);
	void Start(int targetFPS);
	void Wait();
	float GetDeltaTime();

	EventTimer* AddEventTimer(float t, std::function<void()> evnt);
	float Update();
	void ClearAll();

private:
	float m_deltaT = 0;
	std::chrono::steady_clock::time_point m_start;
	std::chrono::steady_clock::time_point m_end;
	std::list<EventTimer> eventTimers;
};