#ifndef __TIMER_H__
#define __TIMER_H__

typedef signed long long int64;
typedef unsigned long long uint64;

class Timer {
	uint64 _beginTime;
	uint64 _pauseTime;
	bool _pause;

public:	
	Timer();

	void start();

	void pause();

	void setPause(bool value);

	bool getPause() const;

	void stop();

	void reset();

	bool isRunning() const;

	uint64 getElapsedMicroSeconds() const;

	uint64 getElapsedMilliSeconds() const;

	uint64 getElapsedSeconds() const;

	uint64 getElapsedPauseMicroSeconds() const;

	uint64 getElapsedPauseMilliSeconds() const;

	uint64 getElapsedPauseSeconds() const;

	static uint64 GetMicroSeconds();

	static uint64 GetMilliSeconds();

	static uint64 GetSeconds();
};

#endif // __TIMER_H__
