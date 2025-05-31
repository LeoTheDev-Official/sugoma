#pragma once
#include "buffer.h"

namespace sugoma::graphics 
{
	class VertexBuffer : public Buffer 
	{
	public:
		VertexBuffer() = delete;
		VertexBuffer(size_t size, void* buffer, BufferUsage usage);
		static Ref<VertexBuffer> Create(size_t size, void* buffer, BufferUsage usage);
	};
}