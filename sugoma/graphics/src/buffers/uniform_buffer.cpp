#include "uniform_buffer.h"

#include <GL/glew.h>

namespace sugoma::graphics 
{
	UniformBuffer::UniformBuffer(size_t size, BufferUsage usage, uint32_t bind_point) : Buffer(GL_UNIFORM_BUFFER, size, nullptr, usage), m_bind_point(bind_point)
	{
		Use(bind_point);
	}
	UniformBuffer::~UniformBuffer()
	{
		GLHandle b;
		glGetIntegeri_v(GL_UNIFORM_BUFFER_BINDING, m_bind_point, (GLint*)&b);
		if (b == m_handle)
			glBindBufferBase(GL_UNIFORM_BUFFER, m_bind_point, 0);
	}
	uint32_t UniformBuffer::BindPoint() const { return m_bind_point; }
	void UniformBuffer::Use(uint32_t bind)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_handle);
		glBindBufferBase(GL_UNIFORM_BUFFER, bind, m_handle);
		m_bind_point = bind;
	}
}