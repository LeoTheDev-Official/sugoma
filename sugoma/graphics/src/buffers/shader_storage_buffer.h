#pragma once
#include "buffer.h"

namespace sugoma::graphics 
{
	class ShaderStorageBuffer : public Buffer 
	{
	public:
		ShaderStorageBuffer() = delete;
		ShaderStorageBuffer(size_t size, BufferUsage usage, uint32_t bind_point);
		~ShaderStorageBuffer();
		static Ref<ShaderStorageBuffer> Create(size_t size, BufferUsage usage, uint32_t bind_point);	
		uint32_t BindPoint() const;
		void Use(uint32_t bind);
	protected:
		uint32_t m_bind_point;
	};
}