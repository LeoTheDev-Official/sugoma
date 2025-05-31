#include "audio_clip.h"
#include "memory/sugoma_memory.h"
namespace sugoma::audio
{
	AudioClip::AudioClip(const AudioClipCreateInfo& info, float* buffer) : m_info(info)
	{
		size_t size = info.frameCount * info.channels * sizeof(float);
		m_buffer = reinterpret_cast<float*>(sugoma_malloc(size));
		if (!m_buffer) return;
		memcpy(m_buffer, buffer, size);
	}

	AudioClip::~AudioClip()
	{
		if (m_buffer) sugoma_free(m_buffer);
	}
	Ref<AudioClip> AudioClip::Create(const AudioClipCreateInfo& info, float* buffer) { return Resources::Create<AudioClip>(info, buffer); }
}
