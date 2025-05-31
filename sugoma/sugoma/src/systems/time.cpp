#include "time.h"
#include "window.h"

namespace sugoma 
{
	static float __time, __unscaledTime;
	static float __deltaTime, __unscaledDeltaTime;
	static float __timeScale = 1;

	float Time::ScaledTime() { return __time; }
	float Time::UnscaledTime() { return __unscaledTime; }
	float Time::DeltaTime() { return __deltaTime; }
	float Time::UnscaledDeltaTime() { return __unscaledDeltaTime; }
	float Time::TimeScale() { return __timeScale; }
	void Time::SetTimeScale(float scale) { __timeScale = scale; }
	void Time::Tick()
	{
		float time = graphics::glfwTime();
		__unscaledDeltaTime = time - __unscaledTime;
		__unscaledTime = time;
		__deltaTime = __unscaledDeltaTime * __timeScale;
		__time += __deltaTime;
	}
}