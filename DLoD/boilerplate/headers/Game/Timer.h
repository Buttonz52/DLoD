#ifndef TIMER_H
#define TIMER_H

#include <ctime>
#include <iostream>
class Timer
{
  clock_t startedAt;
  clock_t pausedAt;
  bool started;
  bool paused;

public:

  Timer();
  bool isStarted();
  bool isStopped();
  bool isPaused();
  bool isActive();

  void pause();
  void resume();
  void stop();
  void start();
  void reset();

  clock_t getTicks();
  bool checkSleep();
  void startSleep(const clock_t & sleepAmount);
private:
	bool isSleep;
	clock_t startedSleep, sleepDuration;
};

#endif