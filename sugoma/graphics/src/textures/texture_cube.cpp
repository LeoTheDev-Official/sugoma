#include "texture_cube.h"
#include <GL/glew.h>

#include "pipeline/compute_pipeline.h"
#include "utility/file_utility.h"
namespace sugoma::graphics
{
	const char* __hdri_to_cube_compute_source = R"(
layout (local_size_x = 16, local_size_y = 16) in;

layout(binding = 0) uniform sampler2D equirectHDR;
layout(rgba16f, binding = 1) writeonly uniform imageCube environmentMap;

const float PI = 3.14159265359;

vec3 SampleDirectionFromUV(uint face, ivec2 uv, ivec2 size) {
    vec2 uvN = (vec2(uv) + 0.5) / vec2(size); // Normalize UV [0,1]
    uvN = uvN * 2.0 - 1.0;                    // [-1,1]

    vec3 dir = vec3(0.0);

    if (face == 0) dir = normalize(vec3( 1.0, -uvN.y, -uvN.x)); // +X
    if (face == 1) dir = normalize(vec3(-1.0, -uvN.y,  uvN.x)); // -X
    if (face == 2) dir = normalize(vec3( uvN.x,  1.0,  uvN.y)); // +Y
    if (face == 3) dir = normalize(vec3( uvN.x, -1.0, -uvN.y)); // -Y
    if (face == 4) dir = normalize(vec3( uvN.x, -uvN.y,  1.0)); // +Z
    if (face == 5) dir = normalize(vec3(-uvN.x, -uvN.y, -1.0)); // -Z

    return dir;
}

void main() {
    ivec2 size = imageSize(environmentMap).xy;
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

    if (texelCoord.x >= size.x || texelCoord.y >= size.y)
        return;

    for (uint face = 0; face < 6; ++face) {
        vec3 dir = SampleDirectionFromUV(face, texelCoord, size);

        float theta = acos(dir.y);
        float phi = atan(dir.z, dir.x) + PI;
        vec2 uv = vec2(phi / (2.0 * PI), theta / PI);

        vec3 color = texture(equirectHDR, uv).rgb;

        imageStore(environmentMap, ivec3(texelCoord, face), vec4(color, 1.0));
    }
}
)";
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
		sinfo.source = __hdri_to_cube_compute_source;
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