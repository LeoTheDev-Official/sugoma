#include "logger.h"

namespace sugoma::core
{
	static const char* level_names[] = {
		"LOG",
		"INFO",
		"WARNING",
		"ERROR"
	};

#ifdef _WIN32
#include <windows.h>
	void internal_log(log_level level, const char* source, const char* msg) 
	{
		static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		static WORD colors[] = {
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, // LOG
			FOREGROUND_GREEN, //INFO
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY, // Warning
			FOREGROUND_RED | FOREGROUND_INTENSITY // Error
		};
		SetConsoleTextAttribute(hConsole, colors[level]);
		printf("[%s][%s] : %s\n", level_names[level], source, msg);
	}
	void internal_error_popup(const char* title, const char* message) 
	{
		MessageBoxA(NULL, message, title, MB_OK | MB_ICONERROR);
	}
#endif
	void __sugoma_log(log_level level, const char* source, const char* message)
	{
		internal_log(level, source, message);
	}
	void __sugoma_assert(const char* message, const char* filepath, size_t line)
	{
		sugoma_error(message);
		std::stringstream str;
		str << "Assertion failed at " << filepath << ":" << line << "\n" << message;
		internal_error_popup("Assertion Failed", str.str().c_str());
	}
}