#include "resource.h"
#include <unordered_map>
namespace sugoma::core 
{
	static std::unordered_map<std::type_index, ResourceType> __resource_types;
	static std::unordered_map<ResourceType, ResourceAllocator*> __resource_allocators;

	using resource_version = uint16_t;
#pragma pack(1)
	struct structured_resource_id 
	{
		uint16_t type;
		uint16_t bucket_index;
		uint16_t block_index;
		resource_version version;
		ResourceID To() { return *reinterpret_cast<ResourceID*>(this); }
		void From(ResourceID id) { *this = *reinterpret_cast<structured_resource_id*>(&id); }
	};

#pragma pack(1)
	struct resource_header 
	{
		resource_version version;
	};

	void Resources::Destroy(ResourceID id)
	{
		if (!id) return;
		structured_resource_id res_id;
		res_id.From(id);
		ResourceAllocator* allocator = GetAllocator(res_id.type);
		if (!allocator) return;
		uint8_t* block = allocator->GetBlock({ res_id.bucket_index, res_id.block_index });
		if (!block) return;
		resource_header& header = *reinterpret_cast<resource_header*>(block);
		if (header.version != res_id.version) return;
		++header.version;
		Resource* res = reinterpret_cast<Resource*>(block + sizeof(resource_header));
		res->~Resource();
		allocator->FreeBlock({ res_id.bucket_index, res_id.block_index });
	}

	Resource* Resources::Get(ResourceID id)
	{
		if (!id) return nullptr;
		structured_resource_id res_id;
		res_id.From(id);
		ResourceAllocator* allocator = GetAllocator(res_id.type);
		if (!allocator) return nullptr;
		uint8_t* block = allocator->GetBlock({ res_id.bucket_index, res_id.block_index });
		if (!block) return nullptr;
		resource_header header = *reinterpret_cast<resource_header*>(block);
		if (header.version != res_id.version) return nullptr;
		return reinterpret_cast<Resource*>(block + sizeof(resource_header));
	}
	ResourceID Resources::AllocateBlock(ResourceAllocator* allocator, ResourceType type, uint8_t*& block)
	{
		ResourceAllocator::AllocationPos pos;
		uint8_t* head = allocator->AllocateBlock(&pos);
		resource_header* header = reinterpret_cast<resource_header*>(head);
		structured_resource_id id { type, pos.bucketIndex, pos.blockIndex, header->version };
		ResourceID i = id.To();
		block = head + sizeof(resource_header);
		return i;
	}
	ResourceAllocator* Resources::GetAllocator(ResourceType type)
	{
		auto it = __resource_allocators.find(type);
		if (it == __resource_allocators.end()) return nullptr;
		return it->second;
	}
	ResourceAllocator* Resources::CreateAllocator(ResourceType type, size_t size)
	{
		ResourceAllocator* allocator = new ResourceAllocator(SUGOMA_RESOURCES_PER_BUCKET, size + sizeof(resource_header));
		__resource_allocators[type] = allocator;
		return allocator;
	}
	uint16_t Resources::GetTypeID(const std::type_info& type)
	{
		auto it = __resource_types.find(std::type_index(type));
		if (it == __resource_types.end())
		{
			ResourceType id = (ResourceType)__resource_types.size() + 1;
			__resource_types[type] = id;
			return id;
		}
		return it->second;
	}
}