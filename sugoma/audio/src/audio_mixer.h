#pragma once
#include <string>
#include <vector>
#include "audio_source.h"
namespace sugoma::audio 
{
	class AudioEngine;
	struct AudioMixerCreateInfo 
	{
		const char* debugName;
	};
	class AudioMixer : public Resource
	{
	public:
		AudioMixer() = delete;
		AudioMixer(const AudioMixerCreateInfo& info, const AudioEngine& engine);
		~AudioMixer();
		void AddSource(Ref<AudioSource> source);
		void RemoveSource(Ref<AudioSource> source);
		void Mix(float* buffer, const AudioFrame& frame);
		float volume = 1.0f;
	private:
		std::string m_debugName;
		std::vector<Ref<AudioSource>> m_sources;
		float* m_buffer = nullptr;
		size_t m_buffer_size;
	};
}