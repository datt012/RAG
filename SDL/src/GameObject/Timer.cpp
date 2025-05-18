#include "Timer.h"
#include <SDL_timer.h>
Timer::Timer() {
	mStartTicks = 0;
	mPausedTicks = 0;
	mStarted = false;
	mPaused = false;
}
void Timer::Start() {
	mStarted = true;
	mPaused = false;
	mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}
void Timer::Stop() {
	mStarted = false;
	mPaused = false;
	mStartTicks = 0;
	mPausedTicks = 0;
}
void Timer::Pause() {
	if (mStarted && !mPaused) {
		mPaused = true;
		mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
	}
}
void Timer::UnPause() {
	if (mStarted && mPaused) {
		mPaused = false;
		mStartTicks = SDL_GetTicks() - mPausedTicks;
		mPausedTicks = 0;
	}
}
Uint32 Timer::GetTicks() {
	Uint32 time = 0;
	if (mStarted) {
		if (mPaused) {
			time = mPausedTicks;
		}
		else {
			time = SDL_GetTicks() - mStartTicks;
		}
	}
	return time;
}
bool Timer::IsStarted() {
	return mStarted;
}
bool Timer::IsPaused() {
	return mPaused && mStarted;
}