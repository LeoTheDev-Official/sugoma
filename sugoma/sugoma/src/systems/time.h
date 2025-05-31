#pragma once


namespace sugoma 
{
	class Time 
	{
	public:
		static float ScaledTime();
		static float UnscaledTime();
		static float DeltaTime();
		static float UnscaledDeltaTime();
		static float TimeScale();

		static void SetTimeScale(float scale);

		static void Tick();
	};
}