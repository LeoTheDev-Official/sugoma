#include "module.h"
#include "module_stack.h"
namespace sugoma::core 
{
	Module::Module(const char* debugName) : m_debugName(debugName) 
	{
		PushStepFunction(ModuleLoopStep::Tick, std::bind(&Module::Tick, this));
	}
	bool Module::Validate() { return true; }
	const std::string& Module::DebugName() const { return m_debugName; }
	void Module::PushStepFunction(EngineStepFlag flag, ModuleStepFunc func) { m_functionBinds.emplace_back(ModuleStepFunctionBind{ flag, func }); }
}