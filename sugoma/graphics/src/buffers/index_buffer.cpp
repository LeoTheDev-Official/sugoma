#include "index_buffer.h"

#include <GL/glew.h>

namespace sugoma::graphics 
{
	IndexBuffer::IndexBuffer(size_t size, void* buffer, BufferUsage usage) : Buffer(GL_ELEMENT_ARRAY_BUFFER, size, buffer, usage) {}
}