#pragma once
#include <SDL_stdinc.h>
class Timer {
public:
	Timer();
	void Start();
	void Stop();
	void Pause();
	void UnPause();
	Uint32 GetTicks();
	bool IsStarted();
	bool IsPaused();
private:
	Uint32 mStartTicks;
	Uint32 mPausedTicks;
	bool mPaused;
	bool mStarted;
};

