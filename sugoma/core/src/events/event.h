#pragma once
#include <string>

namespace sugoma::core
{
	class Event 
	{
	public:
		virtual std::string ToString() const { return ""; };
		bool Handled() const { return m_handled; }
	private:
		bool m_handled = false;
		friend class EventDispatcher;
	};
}