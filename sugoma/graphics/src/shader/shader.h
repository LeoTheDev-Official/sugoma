#pragma once
#include <vector>
#include <string>
#include "resources/resource.h"
#include "shader_types.h"
#include "transfer.h"
#include "struct.h"
#include "ubuffer.h"
#include "sbuffer.h"
#include "material.h"
namespace sugoma::graphics
{
	using namespace core;
	struct ShaderStageInfo 
	{
		std::vector<TransferField> inputs;
		std::vector<TransferField> outputs;
		std::vector<StructInfo> structs;
		std::vector<UniformBufferInfo> uniformBuffers;
		std::vector<ShaderStorageBufferInfo> storageBuffers;
		std::string source;
		std::string_view entryPoint = "main";
	};
	struct ShaderInfo
	{
		std::string_view name;
		std::unordered_map<ShaderStage, ShaderStageInfo> stages;
		MaterialInfo material;
	};

	class Shader : public Resource
	{
	public:
		Shader(std::string_view name);
		virtual ~Shader();
	protected:
		ShaderInfo m_shaderInfo;
	};
}