#pragma once
#include <vector>
#include "shader_types.h"

namespace sugoma::graphics 
{
	enum class MaterialFieldType 
	{
		Float,
		Int,
		Vector,
		Color,
		Texture1D,
		Texture2D,
		Texture3D,
		TextureCube
	};
	struct MaterialFieldInfo
	{
		std::string_view name;
		MaterialFieldType type;
		glm::vec4 defaultValue = glm::vec4(0.0f);
	};
	struct MaterialInfo 
	{
		std::vector<MaterialFieldInfo> fields;
	};

}