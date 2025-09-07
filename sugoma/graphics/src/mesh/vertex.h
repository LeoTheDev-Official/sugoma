#pragma once
#include <glm/glm.hpp>
#include <string>
namespace sugoma::graphics 
{
	enum class VertexLayoutComponentType
	{
		BYTE = 0x1400,
		HALF_FLOAT = 0x140B,
		FLOAT = 0x1406,
		DOUBLE = 0x140A,
		FIXED = 0x140C,
		UNSIGNED_BYTE = 0x1401,
		SHORT = 0x1402,
		UNSIGNED_SHORT = 0x1403,
		INT = 0x1404,
		UNSIGNED_INT = 0x1405
	};
	struct VertexLayoutComponent
	{
		std::string name;
		VertexLayoutComponentType type;
		uint8_t count;
		bool normalized = false;
	};
	struct VertexLayout
	{
		std::vector<VertexLayoutComponent> components;
		uint16_t VertexLayoutSize() const;
		static uint8_t VertexLayoutComponentSize(VertexLayoutComponentType type);
	};
	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec4 color;
		glm::vec3 normal;
		glm::vec3 tangent;
		static const VertexLayout& Layout()
		{
			static VertexLayout layout =
			{
				{
					{"v_position", VertexLayoutComponentType::FLOAT, 3},
					{"v_uv", VertexLayoutComponentType::FLOAT, 2},
					{"v_color", VertexLayoutComponentType::FLOAT, 4},
					{"v_normal", VertexLayoutComponentType::FLOAT, 3, true},
					{"v_tangent", VertexLayoutComponentType::FLOAT, 3, true}
				}
			};
			return layout;
		}
	};
}