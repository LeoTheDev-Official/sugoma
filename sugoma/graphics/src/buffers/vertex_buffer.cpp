#include "vertex_buffer.h"

#include <GL/glew.h>

namespace sugoma::graphics 
{
	VertexBuffer::VertexBuffer(size_t size, void* buffer, BufferUsage usage) : Buffer(GL_ARRAY_BUFFER, size, buffer, usage) {}
	Ref<VertexBuffer> VertexBuffer::Create(size_t size, void* buffer, BufferUsage usage) { return Resources::Create<VertexBuffer>(size, buffer, usage); }
}