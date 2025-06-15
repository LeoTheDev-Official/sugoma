#pragma once
#include <string>
#include <array>
#include "resources/resource.h"
#include "glm/glm.hpp"
namespace sugoma::graphics 
{
	class GraphicsPipeline;
	class Texture2D;
	class TextureCube;
	using namespace core;
	enum class MaterialPropertyType 
	{
		Float,
		Int,
		Vector2,
		Vector3,
		Vector4,
		Color,
		Texture2D,
		TextureCube
	};
	struct MaterialProperty 
	{
		char name[32] = { 0 };
		MaterialPropertyType type = MaterialPropertyType::Int;
		uint32_t location = 0;
		uint8_t value[8] = { 0 };
	};
	class Material : public Resource 
	{
	public:
		Material() = default;
		Material(Ref<GraphicsPipeline> pipeline);
		Material(const std::string& pipelineName);
	public:
		void SetFloat(const std::string& name, float value);
		void SetInt(const std::string& name, int value);
		void SetVector2(const std::string& name, const glm::vec2& value);
		void SetVector3(const std::string& name, const glm::vec3& value);
		void SetVector4(const std::string& name, const glm::vec4& value);
		void SetColor(const std::string& name, const glm::vec4& value);
		void SetTexture2D(const std::string& name, const Ref<Texture2D>& texture);
		void SetTextureCube(const std::string& name, const Ref<TextureCube>& texture);

	private:
		MaterialProperty* FindProperty(const std::string& name);
	private:
		uint32_t m_propertyCount = 0;
		std::array<MaterialProperty, 16> m_properties;
		std::string m_pipelineName;
	};
}