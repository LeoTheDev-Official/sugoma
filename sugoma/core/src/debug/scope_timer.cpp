#include "scope_timer.h"
#include "logger.h"
namespace sugoma::core
{
	core::ScopeTimer::ScopeTimer(const char* label) : m_label(label)
	{
		m_start = std::chrono::high_resolution_clock::now();
	}

	core::ScopeTimer::~ScopeTimer()
	{
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_start).count();
		sugoma_log(m_label << " - " << duration << "ms");
	}
}
