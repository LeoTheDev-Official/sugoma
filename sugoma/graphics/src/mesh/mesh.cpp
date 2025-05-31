#include "mesh.h"

#include "buffers/vertex_buffer.h"
#include "buffers/index_buffer.h"
#include "buffers/vertex_array_object.h"

namespace sugoma::graphics 
{
	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, bool dynamic, const std::vector<Submesh>& submeshes):
		m_submeshes(submeshes)
	{
		this->vertices = vertices;
		this->triangles = indices;
		m_vbo = new VertexBuffer(sizeof(Vertex) * vertices.size(), (void*)vertices.data(), dynamic ? BufferUsage::DYNAMIC : BufferUsage::STATIC);
		m_ibo = new IndexBuffer(sizeof(uint32_t) * indices.size(), (void*)indices.data(), dynamic ? BufferUsage::DYNAMIC : BufferUsage::STATIC);
		m_vao = new VertexArrayObject(m_vbo, m_ibo, Vertex::Layout());
		m_vertex_count = vertices.size();
		m_index_count = indices.size();
	}
	Mesh::~Mesh()
	{
		if (m_vao) delete m_vao;
		if (m_vbo) delete m_vbo;
		if (m_ibo) delete m_ibo;
	}
	void Mesh::Update()
	{
		m_vbo->Data(vertices.data(), vertices.size() * sizeof(Vertex));
		m_ibo->Data(triangles.data(), triangles.size() * sizeof(uint32_t));
		m_vertex_count = vertices.size();
		m_index_count = triangles.size();
	}
	uint32_t Mesh::VertexCount() const { return m_vertex_count; }
	uint32_t Mesh::IndexCount() const { return m_index_count; }
	const VertexBuffer* Mesh::VBO() const { return m_vbo; }
	const IndexBuffer* Mesh::IBO() const { return m_ibo; }
	const VertexArrayObject* Mesh::VAO() const { return m_vao; }
	uint32_t Mesh::SubmeshCount() const { return (uint32_t)m_submeshes.size(); }
	const Submesh& Mesh::GetSubmesh(uint32_t index) const { return m_submeshes[index]; }
	const std::vector<Submesh>& Mesh::GetSubmeshes() const { return m_submeshes; }
	void Mesh::AddSubmesh(const Submesh& submesh) { m_submeshes.push_back(submesh); }
	const Ref<Mesh> Primitives::Quad()
	{
		static Ref<Mesh> mesh = 0;
		if (!mesh) {
			std::vector<Vertex> vertices(4);

			Vertex v;
			v.color = glm::vec4(1);
			v.normal = glm::vec3(0, 0, 1);

			v.position = { -0.5f, -0.5f, 0.0f };
			v.uv = { 0, 0 };
			vertices[0] = v;

			v.position = { -0.5f, 0.5f, 0.0f };
			v.uv = { 0, 1 };
			vertices[1] = v;

			v.position = { 0.5f, 0.5f, 0.0f };
			v.uv = { 1, 1 };
			vertices[2] = v;

			v.position = { 0.5f, -0.5f, 0.0f };
			v.uv = { 1, 0 };
			vertices[3] = v;

			std::vector<uint32_t> indices = { 0, 1, 2, 0, 2, 3 };
			mesh = Resources::Create<Mesh>(vertices, indices, false);
		}
		return mesh;
	}
	const Ref<Mesh> Primitives::Plane()
	{
		static Ref<Mesh> mesh = 0;
		if (!mesh) {
			std::vector<Vertex> vertices(4);

			Vertex v;
			v.color = glm::vec4(1);
			v.normal = glm::vec3(0, 0, 1);

			v.position = { -0.5f, 0.0f, -0.5f };
			v.uv = { 0, 0 };
			vertices[0] = v;

			v.position = { -0.5f, 0.0f, 0.5f };
			v.uv = { 0, 1 };
			vertices[1] = v;

			v.position = { 0.5f, 0.0f, 0.5f };
			v.uv = { 1, 1 };
			vertices[2] = v;

			v.position = { 0.5f, 0.0f, -0.5f };
			v.uv = { 1, 0 };
			vertices[3] = v;

			std::vector<uint32_t> indices = { 0, 1, 2, 0, 2, 3 };
			mesh = Resources::Create<Mesh>(vertices, indices, false);
		}
		return mesh;
	}
}