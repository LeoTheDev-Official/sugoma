#include "runtime_module.h"
#include "debug/logger.h"

#include "assets/asset.h"

#include "events/event_dispatcher.h"
#include "events/window_events.h"
#include "events/input_events.h"

#include "engine/engine.h"

#include "systems/time.h"
#include "systems/input_system.h"

namespace sugoma 
{
	bool OnAppCloseRequest(WindowCloseEvent& event) 
	{
		Context()->Shutdown();
		return true;
	}

	RuntimeModule::RuntimeModule() : Module("RuntimeModule")
	{
		PushStepFunction(0, std::bind(&RuntimeModule::TickSystems, this));
	}
	void RuntimeModule::TickSystems()
	{
		Time::Tick();
		InputSystem::Tick();
	}
	void RuntimeModule::OnAttach()
	{
		Assets::RegisterNativeAssetTypes();
	}
	void RuntimeModule::OnDetach()
	{
	}
	void RuntimeModule::Tick()
	{
		
	}
	void RuntimeModule::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(OnAppCloseRequest);
		dispatcher.DispatchMany<InputEvent>(InputSystem::SendEvent);
	}
	bool RuntimeModule::Validate()
	{
		return true;
	}


}