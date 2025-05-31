#pragma once
#include "modules/debug_module.h"

namespace sugoma 
{
	class AudioDebugWindow : public DebugWindow 
	{
	public:
		AudioDebugWindow();
		void OnCreate() override;
		void OnClose() override;
		void OnGUI() override;
	};
}