#include "vertex_array_object.h"

#include <GL/glew.h>

namespace sugoma::graphics 
{
	VertexArrayObject::VertexArrayObject(VertexBuffer* vbo, IndexBuffer* ibo, const VertexLayout& layout) : m_vbo(vbo), m_ibo(ibo)
	{
		glGenVertexArrays(1, &m_handle);
		glBindVertexArray(m_handle);
		glBindBuffer(vbo->Target(), vbo->Handle());
		glBindBuffer(ibo->Target(), ibo->Handle());
		uint16_t stride = layout.VertexLayoutSize();
		uint16_t offset = 0;
		for (uint8_t i = 0; i < layout.components.size(); ++i)
		{
			auto& c = layout.components[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, c.count, (GLenum)c.type, c.normalized, stride, (void*)offset);
			offset += c.count * VertexLayout::VertexLayoutComponentSize(c.type);
		}
		glBindVertexArray(0);
	}
	VertexArrayObject::~VertexArrayObject()
	{
		if (m_handle)
			glDeleteVertexArrays(1, &m_handle);
	}
	VertexBuffer* VertexArrayObject::VBO() { return m_vbo; }
	const VertexBuffer* VertexArrayObject::VBO() const { return m_vbo; }
	IndexBuffer* VertexArrayObject::IBO() { return m_ibo; }
	const IndexBuffer* VertexArrayObject::IBO() const { return m_ibo; }
	GLHandle VertexArrayObject::Handle() const { return m_handle; }
	void VertexArrayObject::SetLayout(const VertexLayout& layout)
	{
		glBindVertexArray(m_handle);
		uint16_t stride = layout.VertexLayoutSize();
		uint16_t offset = 0;
		for (uint8_t i = 0; i < layout.components.size(); ++i)
		{
			auto& c = layout.components[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, c.count, (GLenum)c.type, c.normalized, stride, (void*)offset);
			offset += c.count * VertexLayout::VertexLayoutComponentSize(c.type);
		}
		glBindVertexArray(0);
	}
	uint16_t VertexLayout::VertexLayoutSize() const
	{
		uint16_t s = 0;
		for (auto c : components)
			s += VertexLayoutComponentSize(c.type) * c.count;
		return s;
	}
	uint8_t VertexLayout::VertexLayoutComponentSize(VertexLayoutComponentType type)
	{
		switch (type)
		{
		case VertexLayoutComponentType::BYTE:
			return 1;
		case VertexLayoutComponentType::HALF_FLOAT:
			return 2;
		case VertexLayoutComponentType::FLOAT:
			return 4;
		case VertexLayoutComponentType::DOUBLE:
			return 8;
		case VertexLayoutComponentType::FIXED:
			return 4;
		case VertexLayoutComponentType::UNSIGNED_BYTE:
			return 1;
		case VertexLayoutComponentType::SHORT:
			return 2;
		case VertexLayoutComponentType::UNSIGNED_SHORT:
			return 2;
		case VertexLayoutComponentType::INT:
			return 4;
		case VertexLayoutComponentType::UNSIGNED_INT:
			return 4;
		default:
			return 0;
		}
	}
}