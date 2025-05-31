#pragma once
#include "event.h"
#include <typeindex>
#include <functional>
#include <unordered_map>
#include <vector>
namespace sugoma::core
{
	template<typename T>
	using EventDispatchFunc = std::function<bool(T& evt)>;
	class EventDispatcher
	{
	public:
		EventDispatcher(Event& evt);
		template<typename T>
		void Dispatch(EventDispatchFunc<T> func)
		{
			std::type_index t1 = typeid(T&);
			std::type_index t2 = typeid(*m_event);
			if (t2 == t1) m_event->m_handled = func(*(T*)m_event);
		}
		template<typename T>
		void DispatchMany(EventDispatchFunc<T> func)
		{
			T* t = dynamic_cast<T*>(m_event);
			if (t != nullptr)
				t->m_handled = func(*t);
		}

	protected:
		Event* m_event;
	};
}