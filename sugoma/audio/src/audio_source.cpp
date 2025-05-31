#include "audio_source.h"
#include "audio_engine.h"
namespace sugoma::audio 
{
	AudioSource::AudioSource(Ref<AudioClip> clip) : m_source(clip), m_playhead(0), volume(1.0f), pitch(1.0f), loop(false) {}
	void AudioSource::Sample(float* buffer, const AudioFrame& frame)
	{
		if (!m_valid) return;
		if (!m_source)
		{
			m_valid = false;
			return;
		}
		if (!m_playing) return;
		const AudioClip* clip = m_source.get();
		uint32_t totalFrames = clip->GetFrameCount();
		uint32_t channels = frame.channels;
		const float* sourceBuffer = clip->GetBuffer();

		uint32_t clipSampleRate = clip->GetSampleRate();
		uint32_t engineSampleRate = frame.sampleRate;

		double baseRate = (double)clipSampleRate / (double)engineSampleRate;
		double step = pitch * baseRate;

		uint32_t outputFrames = frame.frameCount;
		for (uint32_t i = 0; i < outputFrames; ++i)
		{
			uint32_t frameIndex = (uint32_t)m_playhead;
			double frac = m_playhead - frameIndex;
			if (frameIndex + 1 >= totalFrames)
			{
				if (loop)
					m_playhead = 0.0f;
				else 
				{
					Stop();
					break;
				}
			}
			for (uint32_t ch = 0; ch < channels; ++ch)
			{
				uint32_t idx1 = frameIndex * channels + ch;
				uint32_t idx2 = (frameIndex + 1) * channels + ch;
				float sample = (1.0f - frac) * sourceBuffer[idx1] + frac * sourceBuffer[idx2];
				buffer[i * channels + ch] += sample * volume;
			}
			m_playhead += step;
			if (loop && m_playhead >= totalFrames)
				m_playhead = fmod(m_playhead, (float)totalFrames);
		}
	}
	void AudioSource::Play()
	{
		m_playhead = 0;
		m_playing = true;
	}
	void AudioSource::Stop()
	{
		m_playhead = 0;
		m_playing = false;
	}
	void AudioSource::Pause()
	{
		m_playing = false;
	}
	void AudioSource::Resume()
	{
		m_playing = true;
	}
}