#include "Timer.h"

void Timer::Start(std::chrono::microseconds delta)
{
	m_start = std::chrono::high_resolution_clock::now();
	m_end = m_start + delta;
}

void Timer::Start(int targetFPS)
{
	m_start = std::chrono::high_resolution_clock::now();
	m_end = m_start + std::chrono::microseconds(1000000 / targetFPS);
}

void Timer::Wait()
{
	while (std::chrono::high_resolution_clock::now() < m_end)
	{
		std::this_thread::yield();
	}
	float milisecDeltaT = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_start).count();
	m_deltaT = milisecDeltaT / 1000.0f;
}

float Timer::GetDeltaTime()
{
	return m_deltaT;
}

EventTimer* Timer::AddEventTimer(float t, std::function<void()> evnt)
{
	return &(eventTimers.emplace_back(t, evnt));
}

float Timer::Update()
{
	std::vector<std::list<EventTimer>::iterator> listToDelete;
	auto it = eventTimers.begin();
	while (it != eventTimers.end())
	{
		if (it->time <= 0)
		{
			it->event();
			listToDelete.push_back(it);
			it++;
		}
		else
		{
			it->time -= m_deltaT;
			++it;
		}
	}
	for (auto& it : listToDelete)
	{
		eventTimers.erase(it);
	}

	return m_deltaT;
}

void Timer::ClearAll()
{
	eventTimers.clear();
}