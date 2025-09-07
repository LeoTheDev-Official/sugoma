#include "mesh.h"

#include "buffers/vertex_buffer.h"
#include "buffers/index_buffer.h"
#include "buffers/vertex_array_object.h"

namespace sugoma::graphics 
{
	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, bool dynamic, const std::vector<Submesh>& submeshes, bool recalculateTangents):
		m_submeshes(submeshes)
	{
		this->vertices = vertices;
		this->triangles = indices;
		if (recalculateTangents) RecalculateTangents();
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
	void Mesh::RecalculateNormals()
	{
		for(uint32_t i = 0; i < triangles.size(); i += 3)
		{
			uint32_t i0 = triangles[i];
			uint32_t i1 = triangles[i + 1];
			uint32_t i2 = triangles[i + 2];
			glm::vec3 v0 = vertices[i0].position;
			glm::vec3 v1 = vertices[i1].position;
			glm::vec3 v2 = vertices[i2].position;
			glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
			vertices[i0].normal = normal;
			vertices[i1].normal = normal;
			vertices[i2].normal = normal;
		}
	}
	void Mesh::RecalculateTangents()
	{
		for(uint32_t i = 0; i < triangles.size(); i += 3)
		{
			uint32_t i0 = triangles[i];
			uint32_t i1 = triangles[i + 1];
			uint32_t i2 = triangles[i + 2];
			glm::vec3 v0 = vertices[i0].position;
			glm::vec3 v1 = vertices[i1].position;
			glm::vec3 v2 = vertices[i2].position;
			glm::vec2 uv0 = vertices[i0].uv;
			glm::vec2 uv1 = vertices[i1].uv;
			glm::vec2 uv2 = vertices[i2].uv;
			glm::vec3 edge1 = v1 - v0;
			glm::vec3 edge2 = v2 - v0;
			glm::vec2 deltaUV1 = uv1 - uv0;
			glm::vec2 deltaUV2 = uv2 - uv0;
			float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
			glm::vec3 tangent;
			tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
			vertices[i0].tangent = tangent;
			vertices[i1].tangent = tangent;
			vertices[i2].tangent = tangent;
		}
	}
	void Mesh::AddSubmesh(const Submesh& submesh) { m_submeshes.push_back(submesh); }
	Ref<Mesh> Mesh::Create(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, bool dynamic, const std::vector<Submesh>& submeshes, bool recalculateTangents)
	{
		return Resources::Create<Mesh>(vertices, indices, dynamic, submeshes, recalculateTangents);
	}
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
			mesh->RecalculateTangents();
			mesh->Update();
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
			v.normal = glm::vec3(0, 1, 0);

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
			mesh->RecalculateTangents();
			mesh->Update();
		}
		return mesh;
	}
	const Ref<Mesh> Primitives::Cube() 
	{
		static Ref<Mesh> mesh = 0;
		if (!mesh) 
		{
			std::vector<Vertex> vertices;
			vertices.reserve(24);
			Vertex v;

			v.color = glm::vec4(1);

			//front
			v.normal = glm::vec3(0, 0, 1);
			v.position = { -0.5f, -0.5f, 0.5f };
			v.uv = { 0, 0 };
			vertices.push_back(v);

			v.position = { -0.5f, 0.5f, 0.5f };
			v.uv = { 0, 1 };
			vertices.push_back(v);

			v.position = { 0.5f, 0.5f, 0.5f };
			v.uv = { 1, 1 };
			vertices.push_back(v);

			v.position = { 0.5f, -0.5f, 0.5f };
			v.uv = { 1, 0 };
			vertices.push_back(v);

			//back
			v.normal = glm::vec3(0, 0, -1);
			v.position = { -0.5f, -0.5f, -0.5f };
			v.uv = { 0, 0 };
			vertices.push_back(v);

			v.position = { 0.5f, -0.5f, -0.5f };
			v.uv = { 1, 0 };
			vertices.push_back(v);

			v.position = { 0.5f, 0.5f, -0.5f };
			v.uv = { 1, 1 };
			vertices.push_back(v);

			v.position = { -0.5f, 0.5f, -0.5f };
			v.uv = { 0, 1 };
			vertices.push_back(v);

			//left
			v.normal = glm::vec3(-1, 0, 0);
			v.position = { -0.5f, -0.5f, -0.5f };
			v.uv = { 0, 0 };
			vertices.push_back(v);

			v.position = { -0.5f, 0.5f, -0.5f };
			v.uv = { 0, 1 };
			vertices.push_back(v);

			v.position = { -0.5f, 0.5f, 0.5f };
			v.uv = { 1, 1 };
			vertices.push_back(v);

			v.position = { -0.5f, -0.5f, 0.5f };
			v.uv = { 1, 0 };
			vertices.push_back(v);

			//right
			v.normal = glm::vec3(1, 0, 0);
			v.position = { 0.5f, -0.5f, -0.5f };
			v.uv = { 0, 0 };
			vertices.push_back(v);

			v.position = { 0.5f, -0.5f, 0.5f };
			v.uv = { 0, 1 };
			vertices.push_back(v);

			v.position = { 0.5f, 0.5f, 0.5f };
			v.uv = { 1, 1 };
			vertices.push_back(v);

			v.position = { 0.5f, 0.5f, -0.5f };
			v.uv = { 1, 0 };
			vertices.push_back(v);

			//top
			v.normal = glm::vec3(0, 1, 0);
			v.position = { -0.5f, 0.5f, -0.5f };
			v.uv = { 0, 0 };
			vertices.push_back(v);

			v.position = { 0.5f, 0.5f, -0.5f };
			v.uv = { 1, 0 };
			vertices.push_back(v);

			v.position = { 0.5f, 0.5f, 0.5f };
			v.uv = { 1, 1 };
			vertices.push_back(v);

			v.position = { -0.5f, 0.5f, 0.5f };
			v.uv = { 0, 1 };
			vertices.push_back(v);

			//bottom
			v.normal = glm::vec3(0, -1, 0);
			v.position = { -0.5f, -0.5f, -0.5f };
			v.uv = { 0, 0 };
			vertices.push_back(v);

			v.position = { -0.5f, -0.5f, 0.5f };
			v.uv = { 0, 1 };
			vertices.push_back(v);

			v.position = { 0.5f, -0.5f, 0.5f };
			v.uv = { 1, 1 };
			vertices.push_back(v);

			v.position = { 0.5f, -0.5f, -0.5f };
			v.uv = { 1, 0 };
			vertices.push_back(v);

			std::vector<uint32_t> indices;
			indices.reserve(36);
			for (uint32_t i = 0; i < 6; ++i) 
			{
				uint32_t base = i * 4;
				indices.push_back(base);
				indices.push_back(base + 1);
				indices.push_back(base + 2);
				indices.push_back(base);
				indices.push_back(base + 2);
				indices.push_back(base + 3);
			}
			mesh = Mesh::Create(vertices, indices, false);
			mesh->RecalculateTangents();
			mesh->Update();
		}
		return mesh;
	}
}