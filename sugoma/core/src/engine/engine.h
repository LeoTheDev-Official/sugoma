#pragma once
#include <cstdint>
#include "module_stack.h"
namespace sugoma::core 
{
	struct EngineCreateInfo 
	{
		const char* appName;

	};
	class Engine 
	{
	public:
		Engine() = delete;
		Engine(const EngineCreateInfo& info);

		void PushModule(Module* module);
		void PopModule(Module* module);

		Module* QueryModule(const std::string& name);

		bool Valiate();
		void Initialize(bool makeContextCurrent = true);
		void MakeContextCurrent();
		void Run();
		void Shutdown();

		void SendEvent(Event& evt);

		const EngineCreateInfo& Info() const;

	private:
		bool m_running = false;
		ModuleStack m_moduleStack;
		EngineCreateInfo m_info;
	};
	Engine* Context();
}