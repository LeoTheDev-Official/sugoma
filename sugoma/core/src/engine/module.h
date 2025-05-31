#pragma once
#include "common.h"
#include <string>
#include <vector>
#include <functional>
namespace sugoma::core 
{
	class Event;
	using ModuleStepFunc = std::function<void()>;
	struct ModuleStepFunctionBind 
	{
		EngineStepFlag flag;
		ModuleStepFunc func;
	};
	class Module 
	{
	public:
		Module() = delete;
		Module(const char* debugName);
		virtual ~Module() = default;
		virtual bool Validate();
		virtual void OnEvent(Event& event) {};
		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void Tick() {};
	public:
		const std::string& DebugName() const;

	protected:
		void PushStepFunction(EngineStepFlag flag, ModuleStepFunc func);
	private:
		std::vector<ModuleStepFunctionBind> m_functionBinds;
		std::string m_debugName;
		friend class ModuleStack;
	};
}