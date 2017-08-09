#include "Game\Timer.h"

Timer::Timer()
{
  startedAt = 0;
  pausedAt = 0;
  paused = false;
  started = false;
  isSleep = false;
  startedSleep = 0;
  sleepDuration = 0;
}

bool Timer::isStarted()
{
  return started;
}

bool Timer::isStopped()
{
  return !started;
}

bool Timer::isPaused()
{
  return paused;
}

bool Timer::isActive()
{
  return !paused & started;
}

void Timer::pause()
{
  if (paused || !started)
    return;

  paused = true;
  pausedAt = clock();
}

void Timer::resume()
{
  if (!paused)
    return;

  paused = false;
  startedAt += clock() - pausedAt;
}

void Timer::stop()
{
  started = false;
}

void Timer::start()
{
  if (started)
    return;

  started = true;
  paused = false;
  startedAt = clock();
}

void Timer::reset()
{
  paused = false;
  startedAt = clock();
}

clock_t Timer::getTicks()
{
  if (!started)
    return 0;

  if (paused)
    return pausedAt - startedAt;

  return clock() - startedAt;
}

bool Timer::checkSleep() {
	if (isSleep) {
		if (startedSleep + sleepDuration < clock()) {
			isSleep = false;
			//std::cout << "Still sleeping" << std::endl;
		}
	}
	return isSleep;
}

void Timer::startSleep(const clock_t &sleepAmount) {
	if (!isSleep) {
		//std::cout << "starting sleep" << std::endl;
		isSleep = true;
		startedSleep = clock();
		sleepDuration = sleepAmount;
	}
}
