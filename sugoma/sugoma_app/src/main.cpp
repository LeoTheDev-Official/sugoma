#include "engine/engine.h"
#include "modules/runtime_module.h"
#include "modules/debug_module.h"
#include "graphics_module.h"
#include "imgui_module.h"
#include "audio_module.h"
#include "app_module.h"
#include "debug/logger.h"
using namespace sugoma::imgui;
using namespace sugoma::graphics;
using namespace sugoma::audio;
using namespace sugoma::core;
using namespace sugoma;

int main()
{
	EngineCreateInfo info{};
	info.appName = "Test App";
	
	Engine engine(info);

	engine.PushModule(new RuntimeModule());
	engine.PushModule(new GraphicsModule());
	engine.PushModule(new AudioModule());
	engine.PushModule(new ImGuiModule());
	engine.PushModule(new AppModule());
#ifdef SUGOMA_DEBUG
	engine.PushModule(new DebugModule());
#endif
	if (!engine.Valiate()) { sugoma_error("Engine validation failed."); return -1; }
	engine.Initialize();
	engine.Run();
}