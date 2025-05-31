#include "audio_mixer.h"
#include "audio_engine.h"

#include "debug/logger.h"
namespace sugoma::audio 
{
	AudioMixer::~AudioMixer()
	{
		sugoma_free(m_buffer);
	}
	void AudioMixer::AddSource(Ref<AudioSource> source)
	{
		m_sources.push_back(source);
	}
	void AudioMixer::RemoveSource(Ref<AudioSource> source)
	{
		auto it = std::find(m_sources.begin(), m_sources.end(), source);
		if (it == m_sources.end()) return;
		m_sources.erase(it);
	}
	void AudioMixer::Mix(float* buffer, const AudioFrame& frame)
	{
		memset(m_buffer, 0, m_buffer_size);
		for (auto& source : m_sources)
			source->Sample(m_buffer, frame);
		for (int i = 0; i < (frame.frameCount * frame.channels); ++i)
			buffer[i] += m_buffer[i] * volume;
	}
	AudioMixer::AudioMixer(const AudioMixerCreateInfo& info, const AudioEngine& engine)
	{
		auto& einfo = engine.GetInfo();
		m_buffer_size = sizeof(float) * einfo.maxNumFrames * einfo.channels;
		m_buffer = reinterpret_cast<float*>(sugoma_malloc(m_buffer_size));
	}
}
