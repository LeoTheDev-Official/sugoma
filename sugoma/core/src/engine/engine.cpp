#include "engine.h"

namespace sugoma::core 
{
	static Engine* s_context = nullptr;
	Engine::Engine(const EngineCreateInfo& info) : m_info(info) {}
	void Engine::PushModule(Module* module) { m_moduleStack.PushModule(module); }
	void Engine::PopModule(Module* module) { m_moduleStack.PopModule(module); }
	Module* Engine::QueryModule(const std::string& name) { return m_moduleStack.QueryModule(name); }
	bool Engine::Valiate() { return m_moduleStack.Validate(); }
	void Engine::Initialize(bool makeContextCurrent)
	{
		if (makeContextCurrent) MakeContextCurrent();
		m_moduleStack.Build();
	}
	void Engine::MakeContextCurrent() { s_context = this; }
	void Engine::Run()
	{
		m_running = true;
		while (m_running) 
		{
			m_moduleStack.Tick();
		}
	}
	void Engine::Shutdown()
	{
		m_running = false;
		m_moduleStack.Clear();
	}
	void Engine::SendEvent(Event& evt) { m_moduleStack.SendEvent(evt); }
	const EngineCreateInfo& Engine::Info() const { return m_info; }
	Engine* Context() { return s_context; }
}