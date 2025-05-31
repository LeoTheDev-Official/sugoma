#include "buffer.h"

#include <GL/glew.h>
namespace sugoma::graphics 
{
	Buffer::Buffer(GLEnum target, size_t size, void* buffer, BufferUsage usage) : m_size(size), m_target(target), m_usage(usage)
	{
		glGenBuffers(1, &m_handle);
		glBindBuffer(m_target, m_handle);
		glBufferData(m_target, size, buffer, usage);
		glBindBuffer(target, 0);
	}
	Buffer::~Buffer()
	{
		if (m_handle)
			glDeleteBuffers(1, &m_handle);
	}
	GLHandle Buffer::Handle() const { return m_handle; }
	GLEnum Buffer::Target() const { return m_target; }
	BufferUsage Buffer::Usage() const { return m_usage; }
	size_t Buffer::Size() const { return m_size; }
	void Buffer::Data(void* buffer, size_t size)
	{
		glBindBuffer(m_target, m_handle);
		glBufferData(m_target, size, buffer, m_usage);
	}
	void Buffer::SubData(void* buffer, size_t offset, size_t size)
	{
		glBindBuffer(m_target, m_handle);
		glBufferSubData(m_target, offset, size, buffer);
	}
}