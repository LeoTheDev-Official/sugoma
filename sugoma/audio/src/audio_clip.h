#pragma once
#include "resources/resource.h"
namespace sugoma::audio 
{
	using namespace core;
	struct AudioClipCreateInfo 
	{
		uint32_t sampleRate = 44100;
		uint32_t frameCount = 0;
		uint16_t channels = 2;
	};
	
	class AudioClip : public Resource
	{
	public:
		AudioClip() = delete;
		AudioClip(const AudioClipCreateInfo& info, float* buffer);
		~AudioClip();
		static Ref<AudioClip> Create(const AudioClipCreateInfo& info, float* buffer);

		inline uint32_t GetSampleRate() const { return m_info.sampleRate; }
		inline uint32_t GetFrameCount() const { return m_info.frameCount; }
		inline uint16_t GetChannels() const { return m_info.channels; }
		inline const float* GetBuffer() const { return m_buffer; }

	private:
		float* m_buffer = nullptr;
		AudioClipCreateInfo m_info;
	};
}