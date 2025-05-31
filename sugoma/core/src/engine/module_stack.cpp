#include "module_stack.h"

namespace sugoma::core 
{
	void ModuleStack::PushModule(Module* module)
	{
		auto it = std::find(m_modules.begin(), m_modules.end(), module);
		if (it != m_modules.end()) return;
		m_modules.push_back(module);
	}
	void ModuleStack::PopModule(Module* module)
	{
		auto it = std::find(m_modules.begin(), m_modules.end(), module);
		if (it == m_modules.end()) return;
		m_modules.erase(it);
	}
	bool ModuleStack::Validate()
	{
		for (auto& module : m_modules) if (!module->Validate()) return false;
		return true;
	}
	void ModuleStack::Build()
	{
		static constexpr EngineStepFlag step_count = sizeof(EngineStepFlag) * 8;
		std::vector<ModuleStepFunc> step_funcs[step_count];

		if (m_built) Clear();

		for (auto& module : m_modules)
			module->OnAttach();

		for (auto& module : m_modules)
			for (auto& bind : module->m_functionBinds)
				if (bind.flag < step_count)
					step_funcs[bind.flag].push_back(bind.func);
		for (EngineStepFlag i = 0; i < step_count; ++i) 
		{
			auto& v = step_funcs[i];
			if (v.empty()) continue;
			ModuleStackStep& step = m_steps.emplace_back();
			step.flag = (1 << i);
			step.functions = std::move(v);
		}
		m_built = true;
	}
	void ModuleStack::Clear()
	{
		for (auto& module : m_modules)
			module->OnDetach();
		m_built = false;
		m_steps.clear();
	}
	void ModuleStack::Tick()
	{
		if (!m_built) return;
		for (auto& step : m_steps)
			for (auto& func : step.functions) func();
	}
	void ModuleStack::SendEvent(Event& evt)
	{
		for(auto& module : m_modules)
			module->OnEvent(evt);
	}
	Module* ModuleStack::QueryModule(const std::string& name)
	{
		for (auto module : m_modules) if (module->DebugName() == name) return module;
		return nullptr;
	}
}