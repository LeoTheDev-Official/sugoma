#pragma once
#include "shader.h"
#include "decl.h"
namespace sugoma::graphics 
{
	class TestShader : public Shader 
	{
	public:
		TestShader() : Shader("TestShader") {}
	private:
		StructDecl m_vertex_struct
		{
			"Vertex",
			{
				{ "position", FieldType::Vec3},
				{ "normal", FieldType::Vec3},
				{ "texCoord", FieldType::Vec2},
				{ "color", FieldType::Vec4}
			}
		};
		MaterialDecl m_material
		{
			{
				{ "Albedo", MaterialFieldType::Color, glm::vec4(1) },
				{ "Metallic", MaterialFieldType::Float },
				{ "Roughness", MaterialFieldType::Float },
				{ "AlbedoMap", MaterialFieldType::Texture2D },
				{ "NormalMap", MaterialFieldType::Texture2D }
			}
		};
		StageDecl m_vertex_stage{ ShaderStage::Vertex };
		InputDecl m_vertex_input{ "Vertex in_", FieldType::Struct};
		OutputDecl m_vertex_output{ "Vertex v_", FieldType::Struct};
		StageSource m_vertex_source
		{
			R"(
			void main() {
				v_position = in_position;
				v_normal = in_normal;
				v_texCoord = in_texCoord;
				v_color = in_color;
				gl_Position = vec4(in_.position, 1.0);
			}
			)"
		};
		StageDecl m_fragment_stage{ ShaderStage::Fragment };
		OutputDecl m_frag_color{ "f_color", FieldType::Vec4 };
		OutputDecl m_frag_normal{ "f_normal", FieldType::Vec3 };
		OutputDecl m_frag_position{ "f_position", FieldType::Vec3 };
		StageSource m_fragment_source
		{
			R"(
			void main() {
				f_color = v_color;
				f_normal = v_normal;
				f_position = v_position;
			}
			)"
		};
	};
}