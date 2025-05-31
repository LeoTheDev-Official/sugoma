#pragma once
#include "audio_clip.h"
#include "resources/resource.h"
namespace sugoma::audio 
{
	using namespace core;
	struct AudioFrame;

	class AudioSource : public Resource
	{
	public:
		AudioSource(Ref<AudioClip> clip);
	public:
		float volume = 1.0f;
		float pitch = 1.0f;
		bool loop = false;
		void Sample(float* buffer, const AudioFrame& frame);

		void Play();
		void Stop();
		void Pause();
		void Resume();
	private:
		bool m_playing = false;
		bool m_valid = true;
		Ref<AudioClip> m_source;
		double m_playhead = 0;
	};
}