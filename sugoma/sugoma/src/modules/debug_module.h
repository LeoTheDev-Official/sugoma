#pragma once
#include "engine/module.h"
#include <vector>
#include <typeindex>
namespace sugoma 
{
	using namespace sugoma::core;
	class DebugWindow 
	{
	public:
		DebugWindow(const char* title);
		virtual ~DebugWindow();
		virtual void OnCreate() {};
		virtual void OnClose() {};
		virtual void OnGUI() {};
		void Close();

		const std::string& Title() const;
	protected:
		int m_flags = 0;
	private:
		std::string m_title;
		bool m_open = true;
		friend class DebugModule;
	};
	class DebugModule : public Module
	{
	public:
		DebugModule();
		void OnAttach() override;
		void OnDetach() override;
		void Tick() override;
		bool Validate() override;

		template<class T>
		DebugWindow* GetWindow() 
		{
			static_assert(std::is_base_of<DebugWindow, T>::value, "T is not a valid DebugWindow");
			std::type_index idx = typeid(T);
			for (auto win : m_windows) 
			{
				std::type_index t = typeid(win);
				if (t == idx) return win;
			}
			DebugWindow* window = new T();
			window->OnCreate();
			m_windows.push_back(window);
			return window;
		}

	private:
		std::vector<DebugWindow*> m_windows;
	};

}