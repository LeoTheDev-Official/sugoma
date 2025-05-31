#include "mesh_asset.h"
#include "assets/vendor/ufbx.h"
#include "debug/logger.h"
#include "math/transform.h"
#include "glm/gtx/quaternion.hpp"
namespace sugoma 
{
#pragma pack (1)
	struct mesh_metadata 
	{
		uint32_t vertexCount, indexCount;
		uint32_t submeshCount;
	};
#pragma pack (1)
	struct submesh_data 
	{
		char name[32] = { 0 };
		uint32_t offset, size;
		Transform transform;
	};
	glm::vec2 to_vec_2(ufbx_vec2 v) { return { (float)v.x, (float)v.y }; }
	glm::vec3 to_vec_3(ufbx_vec3 v) { return { (float)v.x, (float)v.y, (float)v.z }; }
	glm::vec4 to_vec_4(ufbx_vec4 v) { return { (float)v.x, (float)v.y, (float)v.z, (float)v.w }; }
	glm::quat to_quat(ufbx_quat v) { return { (float)v.x, (float)v.y, (float)v.z, (float)v.w }; }

	Buffer AssetImpl<Mesh>::PackAssetFile(const std::filesystem::path& path, const AssetMetadata* meta) 
	{
		ufbx_load_opts opts = { 0 };
		ufbx_error error;
		ufbx_scene* scene = ufbx_load_file(path.string().c_str(), &opts, &error);
		if (!scene) {
			sugoma_error("Failed to load fbx ( " << path << " ) : " << error.description.data);
			return Buffer();
		}
		uint32_t vertex_count = 0, index_count = 0;
		for (uint32_t i = 0; i < scene->nodes.count; ++i) 
		{
			auto node = scene->nodes.data[i];
			if (node->is_root) continue;
			auto mesh = node->mesh;
			vertex_count += mesh->num_vertices;
			index_count += mesh->num_indices;
		}
		std::vector<Vertex> vertices(vertex_count);
		std::vector<uint32_t> indices(index_count);
		std::vector<submesh_data> submeshes;
		submeshes.reserve(scene->nodes.count);
		uint32_t vertex_offset = 0;
		uint32_t index_offset = 0;
		for (uint32_t i = 0; i < scene->nodes.count; ++i) 
		{
			auto node = scene->nodes.data[i];
			if (node->is_root) continue;
			auto mesh = node->mesh;
			for (uint32_t j = 0; j < mesh->num_vertices; ++j) 
			{
				Vertex& v = vertices[vertex_offset + j];
				v.position = to_vec_3(mesh->vertex_position[j]);
				if(mesh->vertex_color.exists)
					v.color = to_vec_4(mesh->vertex_color[j]);
				if (mesh->vertex_normal.exists)
					v.normal = to_vec_3(mesh->vertex_normal[j]);
				if (mesh->vertex_uv.exists)
					v.uv = to_vec_2(mesh->vertex_uv[j]);
			}
			for (uint32_t j = 0; j < mesh->num_indices; ++j) 
				indices[index_offset + j] = mesh->vertex_indices[j] + vertex_offset;

			submesh_data& sm = submeshes.emplace_back();
			sm.offset = index_offset;
			sm.size = mesh->num_indices;
			sm.transform.position = to_vec_3(node->local_transform.translation);
			sm.transform.scale = to_vec_3(node->local_transform.scale);
			glm::quat q = to_quat(node->local_transform.rotation);
			sm.transform.eulerAngles = glm::eulerAngles(q);
			memcpy(sm.name, node->name.data, std::min(sizeof(sm.name) - 1, node->name.length));

			index_offset += mesh->num_indices;
			vertex_offset += mesh->num_vertices;
		}

		size_t vertex_size = sizeof(Vertex) * vertices.size();
		size_t index_size = sizeof(uint32_t) * indices.size();
		size_t submesh_size = sizeof(submesh_data) * submeshes.size();

		mesh_metadata mm{};
		mm.indexCount = indices.size();
		mm.vertexCount = vertices.size();
		mm.submeshCount = submeshes.size();

		size_t buffer_size = sizeof(mm) + vertex_size + index_size + submesh_size;

		Buffer b(buffer_size);
		b.Write(&mm, sizeof(mm));
		b.Write(vertices.data(), vertex_size);
		b.Write(indices.data(), index_size);
		b.Write(submeshes.data(), submesh_size);

		ufbx_free_scene(scene);
		return b;
	}
	Buffer AssetImpl<Mesh>::PackAsset(const Asset& asset, AssetMetadata* meta) 
	{
		return Buffer();
	}
	Ref<Mesh> AssetImpl<Mesh>::UnpackAsset(Buffer buffer) 
	{
		mesh_metadata mm{};
		buffer.ResetCursor();
		buffer.Read(&mm, sizeof(mm));
		std::vector<Vertex> vertices(mm.vertexCount);
		std::vector<uint32_t> indices(mm.indexCount);
		std::vector<submesh_data> submeshes(mm.submeshCount);
		buffer.Read(vertices.data(), vertices.size() * sizeof(Vertex));
		buffer.Read(indices.data(), indices.size() * sizeof(uint32_t));
		buffer.Read(submeshes.data(), submeshes.size() * sizeof(submesh_data));

		std::vector<Submesh> subMesh(submeshes.size());
		for (uint32_t i = 0; i < submeshes.size(); ++i) 
		{
			Submesh& s = subMesh[i];
			submesh_data& sd = submeshes[i];
			s.name = sd.name;
			s.offset = sd.offset;
			s.size = sd.size;
			s.transform = sd.transform;
		}
		return Resources::Create<Mesh>(vertices, indices, false, subMesh);
	}
	AssetMetadata* AssetImpl<Mesh>::CreateMetadata() { return new MeshMetadata(); }

	const std::vector<std::string>& AssetImpl<Mesh>::Extensions()
	{
		static std::vector<std::string> extensions = { ".fbx" };
		return extensions;
	}
}