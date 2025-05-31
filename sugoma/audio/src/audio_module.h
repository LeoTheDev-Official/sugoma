#pragma once
#include "engine/module.h"
#include "audio_engine.h"
namespace sugoma::audio 
{
	using namespace core;
	class AudioModule : public Module
	{
	public:
		AudioModule();
		~AudioModule() override;
		void OnAttach() override;
		void OnDetach() override;
		bool Validate() override;
	};
	AudioEngine& AudioContext();
}