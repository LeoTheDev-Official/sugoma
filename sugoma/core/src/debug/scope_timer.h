#pragma once
#include <chrono>
namespace sugoma::core 
{
	class ScopeTimer 
	{
	public:
		ScopeTimer(const char* label);
		~ScopeTimer();
	private:
		const char* m_label;
		std::chrono::high_resolution_clock::time_point m_start;
	};
}