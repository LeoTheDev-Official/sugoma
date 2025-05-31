#pragma once
#include "resources/resource.h"
#include "sugoma_gl.h"
namespace sugoma::graphics 
{
	using namespace sugoma::core;
	enum BufferUsage 
	{
		STATIC = 0x88E4,
		DYNAMIC = 0x88E8
	};
	class Buffer : public Resource 
	{
	public:
		virtual ~Buffer();
		GLHandle Handle() const;
		GLEnum Target() const;
		BufferUsage Usage() const;
		size_t Size() const;

		void Data(void* buffer, size_t size);
		void SubData(void* buffer, size_t offset, size_t size);
	protected:
		Buffer(GLEnum target, size_t size, void* buffer, BufferUsage usage);
		GLHandle m_handle;
		GLEnum m_target;
		size_t m_size;
		BufferUsage m_usage;
	};
}