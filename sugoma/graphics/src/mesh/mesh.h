#pragma once
#include "resources/resource.h"
#include "vertex.h"
#include <vector>
#include "math/transform.h"
namespace sugoma::graphics 
{
	using namespace sugoma::core;
	class VertexBuffer;
	class IndexBuffer;
	class VertexArrayObject;
	struct Submesh 
	{
		std::string name;
		uint32_t offset;
		uint32_t size;
		Transform transform;
	};
	class Mesh : public Resource 
	{
	public:
		Mesh() = delete;
		Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, bool dynamic, const std::vector<Submesh>& submeshes = {}, bool recalculateTangents = false);
		~Mesh();
		void Update();
		std::vector<Vertex> vertices;
		std::vector<uint32_t> triangles;
		uint32_t VertexCount() const;
		uint32_t IndexCount() const;

		const VertexBuffer* VBO() const;
		const IndexBuffer* IBO() const;
		const VertexArrayObject* VAO() const;

		uint32_t SubmeshCount() const;
		const Submesh& GetSubmesh(uint32_t index) const;
		const std::vector<Submesh>& GetSubmeshes() const;

		void RecalculateNormals();
		void RecalculateTangents();

		void AddSubmesh(const Submesh& submesh);

		static Ref<Mesh> Create(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, bool dynamic, const std::vector<Submesh>& submeshes = {}, bool recalculateTangents = false);
	private:
		VertexBuffer* m_vbo;
		IndexBuffer* m_ibo;
		VertexArrayObject* m_vao;
		uint32_t m_vertex_count, m_index_count;
		std::vector<Submesh> m_submeshes;
	};
	class Primitives 
	{
	public:
		static const Ref<Mesh> Quad();
		static const Ref<Mesh> Plane();
		static const Ref<Mesh> Cube();
	};
}
