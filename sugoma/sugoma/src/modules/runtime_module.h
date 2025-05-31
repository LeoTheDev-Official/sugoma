#pragma once
#include "engine/module.h"

namespace sugoma 
{
	using namespace core;
	class RuntimeModule : public Module
	{
	public:
		RuntimeModule();
		void TickSystems();
		void OnAttach() override;
		void OnDetach() override;
		void Tick() override;
		void OnEvent(Event& event) override;
		bool Validate() override;
	};
}