#include "material.h"
#include "debug/logger.h"
#include "pipeline/graphics_pipeline.h"
#include <string>
namespace sugoma::graphics 
{
	MaterialPropertyType ToMaterialType(PipelineAttributeType type)
	{
		switch (type)
		{
		case Float:
			return MaterialPropertyType::Float;
		case FloatVec2:
			return MaterialPropertyType::Vector2;
		case FloatVec3:
			return MaterialPropertyType::Vector3;
		case FloatVec4:
			return MaterialPropertyType::Vector4;
		case Double:
			break;
		case DoubleVec2:
			break;
		case DoubleVec3:
			break;
		case DoubleVec4:
			break;
		case Int:
			return MaterialPropertyType::Int;
		case IntVec2:
			break;
		case IntVec3:
			break;
		case IntVec4:
			break;
		case UnsignedInt:
			break;
		case UnsignedIntVec2:
			break;
		case UnsignedIntVec3:
			break;
		case UnsignedIntVec4:
			break;
		case Bool:
			break;
		case BoolVec2:
			break;
		case BoolVec3:
			break;
		case BoolVec4:
			break;
		case Mat2:
			break;
		case Mat3:
			break;
		case Mat4:
			break;
		case Mat2x3:
			break;
		case Mat2x4:
			break;
		case Mat3x2:
			break;
		case Mat3x4:
			break;
		case Mat4x2:
			break;
		case Mat4x3:
			break;
		case DoubleMat2:
			break;
		case DoubleMat3:
			break;
		case DoubleMat4:
			break;
		case DoubleMat2x3:
			break;
		case DoubleMat2x4:
			break;
		case DoubleMat3x2:
			break;
		case DoubleMat3x4:
			break;
		case DoubleMat4x2:
			break;
		case DoubleMat4x3:
			break;
		case Sampler1D:
			break;
		case Sampler2D:
			break;
		case Sampler3D:
			break;
		case SamplerCube:
			break;
		case Sampler1DShadow:
			break;
		case Sampler2DShadow:
			break;
		case Sampler1DArray:
			break;
		case Sampler2DArray:
			break;
		case Sampler1DArrayShadow:
			break;
		case Sampler2DArrayShadow:
			break;
		case SamplerCubeShadow:
			break;
		case Sampler2DRect:
			break;
		case Sampler2DRectShadow:
			break;
		case IntSampler1D:
			break;
		case IntSampler2D:
			break;
		case IntSampler3D:
			break;
		case IntSamplerCube:
			break;
		case IntSampler1DArray:
			break;
		case IntSampler2DArray:
			break;
		case UIntSampler1D:
			break;
		case UIntSampler2D:
			break;
		case UIntSampler3D:
			break;
		case UIntSamplerCube:
			break;
		case UIntSampler1DArray:
			break;
		case UIntSampler2DArray:
			break;
		case Image1D:
			break;
		case Image2D:
			break;
		case Image3D:
			break;
		case Image2DRect:
			break;
		case ImageCube:
			break;
		case ImageBuffer:
			break;
		case Image1DArray:
			break;
		case Image2DArray:
			break;
		case Image2DMS:
			break;
		case Image2DMSArray:
			break;
		case IntImage1D:
			break;
		case IntImage2D:
			break;
		case IntImage3D:
			break;
		case IntImage2DRect:
			break;
		case IntImageCube:
			break;
		case IntImageBuffer:
			break;
		case IntImage1DArray:
			break;
		case IntImage2DArray:
			break;
		case IntImage2DMS:
			break;
		case IntImage2DMSArray:
			break;
		case UIntImage1D:
			break;
		case UIntImage2D:
			break;
		case UIntImage3D:
			break;
		case UIntImage2DRect:
			break;
		case UIntImageCube:
			break;
		case UIntImageBuffer:
			break;
		case UIntImage1DArray:
			break;
		case UIntImage2DArray:
			break;
		case UIntImage2DMS:
			break;
		case UIntImage2DMSArray:
			break;
		default:
			break;
		}
		sugoma_assert(false, "Unknown material property type");
		return MaterialPropertyType::Int; // Default to Int if unknown
	}
	Material::Material(Ref<GraphicsPipeline> pipeline)
	{
		if (!pipeline) return;
		m_pipelineName = pipeline->PipelineName();

		auto& props = pipeline->PublicParams();
		auto& params = pipeline->Parameters();
		m_propertyCount = (uint32_t)props.size();
		for (auto& property : props) 
		{
			MaterialProperty prop{};
			auto& param = params.at(property.internalName);
			strncpy_s(prop.name, property.name.c_str(), sizeof(prop.name) - 1);
			prop.location = param.location;
			//prop.type = param.type;
			//prop.value = 
		}


	}
	Material::Material(const std::string& pipelineName)
	{

	}
	void Material::SetFloat(const std::string& name, float value)
	{
		auto* property = FindProperty(name);
		if (!property) return;
		*reinterpret_cast<float*>(property->value) = value;
	}
	void Material::SetInt(const std::string& name, int value)
	{
		auto* property = FindProperty(name);
		if (!property) return;
		*reinterpret_cast<int*>(property->value) = value;
	}
	void Material::SetVector2(const std::string& name, const glm::vec2& value)
	{
		auto* property = FindProperty(name);
		if (!property) return;
		*reinterpret_cast<glm::vec2*>(property->value) = value;
	}
	void Material::SetVector3(const std::string& name, const glm::vec3& value)
	{
		auto* property = FindProperty(name);
		if (!property) return;
		*reinterpret_cast<glm::vec3*>(property->value) = value;
	}
	void Material::SetVector4(const std::string& name, const glm::vec4& value)
	{
		auto* property = FindProperty(name);
		if (!property) return;
		*reinterpret_cast<glm::vec4*>(property->value) = value;
	}
	void Material::SetColor(const std::string& name, const glm::vec4& value)
	{
		SetVector4(name, value);
	}
	void Material::SetTexture2D(const std::string& name, const Ref<Texture2D>& texture)
	{
		auto* property = FindProperty(name);
		if (!property) return;
		*reinterpret_cast<Ref<Texture2D>*>(property->value) = texture;
	}
	void Material::SetTextureCube(const std::string& name, const Ref<TextureCube>& texture)
	{
		auto* property = FindProperty(name);
		if (!property) return;
		*reinterpret_cast<Ref<TextureCube>*>(property->value) = texture;
	}
	MaterialProperty* Material::FindProperty(const std::string& name)
	{
		for (uint32_t i = 0; i < m_propertyCount; ++i)
			if (name == m_properties[i].name)
				return &m_properties[i];
		return nullptr;
	}
}