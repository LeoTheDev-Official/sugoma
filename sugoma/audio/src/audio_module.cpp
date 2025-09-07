#include "audio_module.h"

namespace sugoma::audio 
{
	static AudioEngine* s_audioEngine = nullptr;
	AudioModule::AudioModule() : Module("AudioModule")
	{
		AudioEngineCreateInfo info{};
		info.maxNumFrames = 2048;
		s_audioEngine = new AudioEngine(info);
		s_audioEngine->Initialize();
	}
	AudioModule::~AudioModule()
	{
		delete s_audioEngine;
		s_audioEngine = nullptr;
	}
	void AudioModule::OnAttach()
	{
		
	}
	void AudioModule::OnDetach()
	{
		delete s_audioEngine;
		s_audioEngine = nullptr;
	}
	bool AudioModule::Validate()
	{
		return true;
	}
	AudioEngine& AudioContext() { return *s_audioEngine; }
}