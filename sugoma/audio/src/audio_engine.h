#pragma once
#include <cstdint>
#include <mutex>
#include <unordered_map>
#include "audio_mixer.h"
namespace sugoma::audio
{
	struct AudioFrame 
	{
		uint32_t frameCount;
		uint32_t sampleRate;
		uint16_t channels;
	};
	struct AudioEngineCreateInfo 
	{
		uint32_t maxNumFrames = 1024;
		uint32_t sampleRate = 44100;
		uint16_t channels = 2; // Stereo
	};
	class AudioEngine 
	{
	public:
		AudioEngine() = delete;
		AudioEngine(const AudioEngineCreateInfo& info);
		void Initialize();
		~AudioEngine();

		void AudioCallback(float* buffer, int num_frames, int num_channels);

		Ref<AudioMixer> CreateMixer(const char* name);
		Ref<AudioMixer> GetMixer(const char* name);
		void DestroyMixer(const char* name);

		void AddSource(Ref<AudioSource> source, const char* mixerName = "");
		void RemoveSource(Ref<AudioSource> source, const char* mixerName = "");

		inline const AudioEngineCreateInfo& GetInfo() const { return m_info; }
	private:
		AudioEngineCreateInfo m_info;
		std::unordered_map<std::string, Ref<AudioMixer>> m_mixer_map;
		std::vector<Ref<AudioMixer>> m_mixers;
		Ref<AudioMixer> m_default_mixer;
		std::mutex m_access_mutex;
	};
}