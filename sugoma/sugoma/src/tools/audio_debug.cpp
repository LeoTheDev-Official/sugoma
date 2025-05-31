#include "audio_debug.h"
#include "imgui.h"
#include "audio_module.h"
#include "audio_clip.h"
#include "assets/asset.h"
#include "debug/logger.h"
namespace sugoma 
{
	using namespace audio;
	static bool paused = true;
	char audio_asset_guid[sizeof(GUID)*2+1] = {0};

	Ref<AudioClip> clip;
	Ref<AudioSource> source;
	Ref<AudioMixer> mixer;
	char clip_id[17] = { 0 };
	float volume = 1.0f;
	AudioDebugWindow::AudioDebugWindow() : DebugWindow("Audio Debug")
	{
	}
	void AudioDebugWindow::OnCreate()
	{
	}
	void AudioDebugWindow::OnClose()
	{
	}
	void AudioDebugWindow::OnGUI()
	{
		if (ImGui::Button("Get")) 
		{
			GUID id = GUID(audio_asset_guid);
			AssetRef<AudioClip> asset = AssetRef<AudioClip>(id);
			mixer = AudioContext().GetMixer(nullptr);
			if (asset) 
			{
				sugoma_log("Got AudioClip with GUID: " << id.ToString());

				clip = asset;
				AudioClip* clip_ptr = clip.get();
				sugoma_log("Sample Rate: " << clip_ptr->GetSampleRate());
				sugoma_log("Frame Count: " << clip_ptr->GetFrameCount());
				sugoma_log("Channels: " << clip_ptr->GetChannels());
			}
			else
			{
				sugoma_error("Failed to get AudioClip with GUID: " << id.ToString());
			}
		}
		ImGui::SameLine();
		ImGui::InputText("Audio Asset GUID", audio_asset_guid, sizeof(audio_asset_guid));

		if (ImGui::Button("Play"))
		{
			if (!clip)
			{
				sugoma_error("No AudioClip loaded to play.");
				return;
			}
			AudioEngine& engine = AudioContext();
			if (source) 
			{
				engine.RemoveSource(source);
				Resources::Destroy(source);
			}
			source = Resources::Create<AudioSource>(clip);
			engine.AddSource(source);
			source->Play();
		}
		if (ImGui::Button("Replay")) source->Play();
		ImGui::SameLine();
		if (ImGui::Button("Pause")) source->Pause();
		ImGui::SameLine();
		if (ImGui::Button("Resume")) source->Resume();
		ImGui::SameLine();
		if (ImGui::Button("Stop")) source->Stop();
		if (ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f)) 
		{
			if (mixer) mixer->volume = volume;
		}
	}

}