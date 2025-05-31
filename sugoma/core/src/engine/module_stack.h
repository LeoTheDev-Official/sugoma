#pragma once
#include "common.h"
#include "module.h"
#include <vector>
namespace sugoma::core 
{
	enum ModuleLoopStep
	{
		Tick = 16,
		PreTick = 15,
		PostTick = 17
	};
	struct ModuleStackStep
	{
		EngineStepFlag flag = 0;
		std::vector<ModuleStepFunc> functions;
	};
	class ModuleStack 
	{
	public:
		ModuleStack() = default;
	public:
		void PushModule(Module* module);
		void PopModule(Module* module);
		bool Validate();
		void Build();
		void Clear();
		void Tick();
		void SendEvent(Event& evt);

		Module* QueryModule(const std::string& name);
	protected:

	private:
		bool m_built = false;
		std::vector<Module*> m_modules;
		std::vector<ModuleStackStep> m_steps;
	};
}