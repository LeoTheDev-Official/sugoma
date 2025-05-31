#pragma once
#include "buffer.h"

namespace sugoma::graphics 
{
	class UniformBuffer : public Buffer
	{
	public:
		UniformBuffer() = delete;
		UniformBuffer(size_t size, BufferUsage usage, uint32_t bind_point);
		~UniformBuffer();
		uint32_t BindPoint() const;
		void Use(uint32_t bind);
	protected:
		uint32_t m_bind_point;
	};
}
