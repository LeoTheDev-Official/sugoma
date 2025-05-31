#include "texture_cube.h"
#include <GL/glew.h>

#include "pipeline/compute_pipeline.h"
#include "utility/file_utility.h"
namespace sugoma::graphics
{
	TextureCube::TextureCube(const TextureCreateInfo& info, void** buffers, TextureFormat buffer_format, TextureFormatComponent component):
		Texture(info)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_handle);
		if (buffers) 
		{
			for (uint8_t i = 0; i < 6; ++i)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, info.format, info.width, info.height, info.border, buffer_format, component, buffers[i]);
		}
		else
		{
			for (uint8_t i = 0; i < 6; ++i)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, info.format, info.width, info.height, info.border, buffer_format, component, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, info.horizontalWrap);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, info.horizontalWrap);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, info.horizontalWrap);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, info.minFilter);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, info.magFilter);
	}
	TextureCube::~TextureCube()
	{

	}
	Ref<TextureCube> TextureCube::Create(const TextureCreateInfo& info, void** buffers, TextureFormat buffer_format, TextureFormatComponent component)
	{
		return Resources::Create<TextureCube>(info, buffers, buffer_format, component);
	}
	ComputePipeline* __create_hdri_convert_pipeline() 
	{
		PipelineCreateInfo info{};
		PipelineStageCreateInfo& sinfo = info.stages.emplace_back();
		sinfo.stage = PipelineStageFlagBits::ComputeStageBit;
		sinfo.source = FileUtility::ReadStringFile("data/hdri_to_cube.shader");
		return new ComputePipeline(info);
	}
	Ref<TextureCube> TextureCube::FromHDRI(Ref<Texture2D> hdri, const TextureCreateInfo& info)
	{
		static ComputePipeline* convert_pipeline = nullptr;
		if (!convert_pipeline) convert_pipeline = __create_hdri_convert_pipeline();
		
		TextureCreateInfo ci = info;
		ci.format = TextureFormat::RGBA16F; //ensure that the format ofthe texture matches the one expected by the compute pipeline

		Ref<TextureCube> texture = Create(ci, nullptr, TextureFormat::RGBA, TextureFormatComponent::UNSIGNED_BYTE);

		auto& groupSize = convert_pipeline->GroupSize();

		glUseProgram(convert_pipeline->Handle());
		glBindImageTexture(1, texture->Handle(), 0, GL_TRUE, 0, GL_WRITE_ONLY, ci.format);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, hdri->Handle());
		glDispatchCompute(ci.width / groupSize.x, ci.height / groupSize.y, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		return texture;
	}
}