#pragma once

#include <sstream>
#ifdef _DEBUG
#define sugoma_message(level, message) \
	{ \
		std::ostringstream oss; \
		oss << message; \
		core::__sugoma_log(level, SUGOMA_MODULE_NAME, oss.str().c_str()); \
	}
#define sugoma_log(message) sugoma_message(core::log_level::log, message)
#define sugoma_info(message) sugoma_message(core::log_level::info, message)
#define sugoma_warning(message) sugoma_message(core::log_level::warning, message)
#define sugoma_error(message) sugoma_message(core::log_level::error, message)
#define sugoma_assert(condition, message) if(!(condition)) { core::__sugoma_assert(message, __FILE__, __LINE__); __debugbreak(); }
#else
#define sugoma_message(level, message) 
#define sugoma_log(message)
#define sugoma_info(message)
#define sugoma_warning(message)
#define sugoma_error(message)
#endif
namespace sugoma::core
{
	enum log_level
	{
		log,
		info,
		warning,
		error
	};
	void __sugoma_log(log_level level, const char* source, const char* message);
	void __sugoma_assert(const char* message, const char* filepath, size_t line);
}