#include <functional>
#include "audio_module.h"
#include "audio_engine.h"
#define SOKOL_IMPL
#include "sokol_audio.h"
namespace sugoma::audio
{
	void __audio_callback(float* buffer, int num_frames, int num_channels) { AudioContext().AudioCallback(buffer, num_frames, num_channels); }
	AudioEngine::AudioEngine(const AudioEngineCreateInfo& info) : m_info(info){}
	void AudioEngine::Initialize()
	{
		std::lock_guard<std::mutex> lock(m_access_mutex);
		AudioMixerCreateInfo minfo{};
		minfo.debugName = "DefaultMixer";
		m_default_mixer = Resources::Create<AudioMixer>(minfo, *this);
		m_mixer_map["DefaultMixer"] = m_default_mixer;
		m_mixers.push_back(m_default_mixer);

		saudio_desc desc = {};
		desc.sample_rate = m_info.sampleRate;
		desc.num_channels = m_info.channels;
		desc.buffer_frames = m_info.maxNumFrames;
		desc.packet_frames = m_info.maxNumFrames;
		desc.stream_cb = __audio_callback;

		saudio_setup(desc);
	}
	AudioEngine::~AudioEngine()
	{
		std::lock_guard<std::mutex> lock(m_access_mutex);
		for (auto& mixer : m_mixer_map) Resources::Destroy(mixer.second);
		saudio_shutdown();
	}
	void AudioEngine::AudioCallback(float* buffer, int num_frames, int num_channels)
	{
		std::lock_guard<std::mutex> lock(m_access_mutex);
		AudioFrame frame{};
		frame.channels = num_channels;
		frame.frameCount = num_frames;
		frame.sampleRate = m_info.sampleRate;
		memset(buffer, 0, sizeof(float) * num_frames * num_channels);
		for (auto& mixer : m_mixers)
		{
			if (mixer->volume <= 0.0f) continue;
			mixer->Mix(buffer, frame);
		}
	}
	Ref<AudioMixer> AudioEngine::CreateMixer(const char* name)
	{
		std::lock_guard<std::mutex> lock(m_access_mutex);
		auto it = m_mixer_map.find(name);
		if (it != m_mixer_map.end()) return it->second;
		AudioMixerCreateInfo info{};
		info.debugName = name;
		Ref<AudioMixer> mixer = Resources::Create<AudioMixer>(info, *this);
		m_mixer_map[name] = mixer;
		m_mixers.push_back(mixer);
		return mixer;
	}
	Ref<AudioMixer> AudioEngine::GetMixer(const char* name)
	{
		if (!name) return m_default_mixer;
		std::lock_guard<std::mutex> lock(m_access_mutex);
		auto it = m_mixer_map.find(name);
		if (it == m_mixer_map.end()) return m_default_mixer;
		return it->second;
	}
	void AudioEngine::DestroyMixer(const char* name)
	{
		std::lock_guard<std::mutex> lock(m_access_mutex);
		auto it = m_mixer_map.find(name);
		if (it == m_mixer_map.end()) return;
		m_mixers.erase(std::remove(m_mixers.begin(), m_mixers.end(), it->second), m_mixers.end());
		m_mixer_map.erase(it);
	}
	void AudioEngine::AddSource(Ref<AudioSource> source, const char* mixerName)
	{
		std::lock_guard<std::mutex> lock(m_access_mutex);
		auto it = m_mixer_map.find(mixerName);
		if (it == m_mixer_map.end()) m_default_mixer->AddSource(source);
		else it->second->AddSource(source);
	}
	void AudioEngine::RemoveSource(Ref<AudioSource> source, const char* mixerName)
	{
		std::lock_guard<std::mutex> lock(m_access_mutex);
		auto it = m_mixer_map.find(mixerName);
		if (it == m_mixer_map.end()) m_default_mixer->RemoveSource(source);
		else it->second->RemoveSource(source);
	}
}
