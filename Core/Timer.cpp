#include <stdlib.h>
#include <sys/time.h>
#include "Timer.h"

Timer::Timer() {
	_beginTime = 0;
	_pauseTime = 0;
	_pause = false;

	start();
}

void Timer::start() {
	_beginTime = GetMicroSeconds();
	_pause = false;
}

void Timer::setPause(bool value) {
	if (_pause == value) {
		return;
	}

	_pause = value;

	if (value) {
		_pauseTime = GetMicroSeconds();
	} else {
		_beginTime += getElapsedPauseMicroSeconds();
	}
}

bool Timer::getPause() const {
	return _pause;
}

void Timer::stop() {
	_beginTime = 0;
	_pause = false;
}

void Timer::reset() {
	stop();
	start();
}

bool Timer::isRunning() const {
	return (_beginTime != 0);
}

uint64 Timer::getElapsedMicroSeconds() const {
	return _pause ? _pauseTime - _beginTime : GetMicroSeconds() - _beginTime;
}

uint64 Timer::getElapsedMilliSeconds() const {
	return (_pause ? _pauseTime - _beginTime : GetMicroSeconds() - _beginTime) / 1000;
}

uint64 Timer::getElapsedSeconds() const {
	return (_pause ? _pauseTime - _beginTime : GetMicroSeconds() - _beginTime) / 1000000;
}

uint64 Timer::getElapsedPauseMicroSeconds() const {
	return _pause ? 0 : GetMicroSeconds() - _pauseTime;
}

uint64 Timer::getElapsedPauseMilliSeconds() const {
	return _pause ? 0 : (GetMicroSeconds() - _pauseTime) / 1000;
}

uint64 Timer::getElapsedPauseSeconds() const {
	return _pause ? 0 : (GetMicroSeconds() - _pauseTime) / 1000000;
}

uint64 Timer::GetMicroSeconds() {
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return tp.tv_sec * 1000000 + tp.tv_usec;
}

uint64 Timer::GetMilliSeconds() {
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

uint64 Timer::GetSeconds() {
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return tp.tv_sec + tp.tv_usec / 1000000;
}
