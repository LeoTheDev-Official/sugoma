#include "audio_clip_asset.h"
#include "debug/logger.h"
#define STB_VORBIS_IMPLEMENTATION
#include "../../vendor/audio/inc/stb_vorbis.c"

namespace sugoma 
{
	using namespace sugoma::audio;

	struct audio_clip_metadata 
	{
		uint32_t sampleRate;
		uint32_t frameCount;
		uint32_t channels;
	};

	Buffer AssetImpl<AudioClip>::PackAssetFile(const std::filesystem::path& path, const AssetMetadata* meta)
	{
		stb_vorbis* vorbis = stb_vorbis_open_filename(path.string().c_str(), nullptr, nullptr);
		if (!vorbis)
		{
			sugoma_error("Failed to open audio file: " << path.string());
			return Buffer();
		}
		stb_vorbis_info info = stb_vorbis_get_info(vorbis);
		audio_clip_metadata metadata{};
		metadata.sampleRate = info.sample_rate;
		metadata.channels = info.channels;
		metadata.frameCount = stb_vorbis_stream_length_in_samples(vorbis) * metadata.channels;
		
		uint32_t sample_count = metadata.frameCount * metadata.channels;
		Buffer buff = Buffer(sizeof(audio_clip_metadata) + sample_count * sizeof(float));
		buff.Write(&metadata, sizeof(audio_clip_metadata));
		stb_vorbis_get_samples_float_interleaved(vorbis, metadata.channels, reinterpret_cast<float*>(buff.Cursor()), sample_count);

		stb_vorbis_close(vorbis);
		return buff;
	}
	Buffer AssetImpl<AudioClip>::PackAsset(const Asset& asset, AssetMetadata* meta)
	{
		
		return Buffer();
	}
	Ref<AudioClip> AssetImpl<AudioClip>::UnpackAsset(Buffer buffer)
	{
		buffer.ResetCursor();
		audio_clip_metadata meta{};
		buffer.Read(&meta, sizeof(audio_clip_metadata));
		AudioClipCreateInfo info{};
		info.channels = meta.channels;
		info.sampleRate = meta.sampleRate;
		info.frameCount = meta.frameCount / meta.channels;
		Ref<AudioClip> clip = AudioClip::Create(info, reinterpret_cast<float*>(buffer.Cursor()));
		return clip;
	}
	AssetMetadata* AssetImpl<AudioClip>::CreateMetadata()
	{
		return new AssetMetadata();
	}
	const std::vector<std::string>& AssetImpl<AudioClip>::Extensions()
	{
		static std::vector<std::string> extensions = { ".ogg" };
		return extensions;
	}
}