#pragma once
#include <vector>
#include "shader_types.h"
#include "struct.h"
namespace sugoma::graphics 
{
	struct ShaderStorageBufferInfo
	{
		std::string_view name;
		std::string_view blockName;
		BufferLayout layout = BufferLayout::Std140;
		uint32_t binding = 0;
		std::vector<StructField> fields;
	};
}