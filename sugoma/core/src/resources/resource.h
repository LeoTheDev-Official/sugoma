#pragma once
#include <cstdint>
#include <typeindex>
#include "memory/sugoma_allocator.h"

namespace sugoma::core 
{
#define SUGOMA_RESOURCES_PER_BUCKET 0x400
	using ResourceID = uint64_t;
	using ResourceType = uint16_t;
	using ResourceAllocator = BlockAllocator<uint16_t, uint16_t>;
	template<typename T>
	class Ref;
	class Resource { public: Resource() = default; virtual ~Resource() = default; };
	class Resources 
	{
	public:
		template<typename T, typename... Args>
		static Ref<T> Create(Args&&... args)
		{
			ResourceType type = GetTypeID(typeid(T));
			ResourceAllocator* allocator = GetAllocator(type);
			if (!allocator) allocator = CreateAllocator(type, sizeof(T));
			uint8_t* block = nullptr;
			ResourceID id = AllocateBlock(allocator, type, block);
			new (block) T(std::forward<Args>(args)...);
			return Ref<T>(id);
		}
		static void Destroy(ResourceID id);
		static Resource* Get(ResourceID id);
	private:
		static ResourceID AllocateBlock(ResourceAllocator* allocator, ResourceType type, uint8_t*& block);
		static ResourceAllocator* GetAllocator(ResourceType type);
		static ResourceAllocator* CreateAllocator(ResourceType type, size_t size);
		static uint16_t GetTypeID(const std::type_info& type);
	};
	template<typename T>
	class Ref
	{
		//static_assert(std::is_base_of<Resource, T>::value, "T must be derived from Resource");
	public:
		Ref() = default;
		Ref(ResourceID id) : m_id(id) {}

		ResourceID GetID() const { return m_id; }
		T* get() { return reinterpret_cast<T*>(Resources::Get(m_id)); }
		const T* get() const { return reinterpret_cast<T*>(Resources::Get(m_id)); }
		T* operator->() { return reinterpret_cast<T*>(Resources::Get(m_id)); }
		const T* operator->() const { return reinterpret_cast<const T*>(Resources::Get(m_id)); }
		operator ResourceID() const { return m_id; }
		operator bool() const { return Resources::Get(m_id); }
		bool operator==(const Ref<T>& other) const { return m_id == other.m_id; }
		template<typename B>
		operator Ref<B>() { Ref<B> b{}; b.m_id = m_id; return b; }
		template<typename B>
		friend class Ref;
	private:
		ResourceID m_id = 0;
	};
}