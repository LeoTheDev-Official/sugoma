#pragma once
#include "buffer.h"
namespace sugoma::graphics 
{
	enum class IndexBufferElement 
	{
		UNSIGNED_BYTE = 0x1401,
		UNSIGNED_SHORT = 0x1403,
		UNSIGNED_INT = 0x1405
	};
	class IndexBuffer : public Buffer 
	{
	public:
		IndexBuffer() = delete;
		IndexBuffer(size_t size, void* buffer, BufferUsage usage);
	};
}