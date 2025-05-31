#pragma once
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "mesh/vertex.h"

namespace sugoma::graphics 
{
	class VertexArrayObject : public Resource 
	{
	public:
		VertexArrayObject(VertexBuffer* vbo, IndexBuffer* ibo, const VertexLayout& layout);
		~VertexArrayObject();

		VertexBuffer* VBO();
		const VertexBuffer* VBO() const;
		IndexBuffer* IBO();
		const IndexBuffer* IBO() const;
		GLHandle Handle() const;

		void SetLayout(const VertexLayout& layout);

	protected:
		VertexBuffer* m_vbo;
		IndexBuffer* m_ibo;
		GLHandle m_handle;
	};
}