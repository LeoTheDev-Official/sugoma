#include <unordered_map>
#include <mutex>
#include <iostream>
#include "sugoma_memory.h"

namespace sugoma::core
{
	struct allocation_info 
	{
		size_t size = 0;
		int group = 0;
	};
	struct memory_group_stats 
	{
		size_t total_allocated = 0;
		size_t total_freed = 0;
		size_t alloc_count = 0;
		size_t free_count = 0;
	};
	struct memory_stats
	{
		memory_stats() { ignore = false; }
		std::unordered_map<void*, allocation_info> allocations;
		std::unordered_map<int, memory_group_stats> group_stats;
		std::mutex mutex;
		thread_local static bool ignore;

		void register_alloc(void* ptr, size_t size, int group)
		{
			if (ignore) return;
			std::lock_guard<std::mutex> lock(mutex);
			ignore = true;
			memory_group_stats& group_stat = group_stats[group];
			group_stat.total_allocated += size;
			++group_stat.alloc_count;
			allocations[ptr] = { size, group };
			ignore = false;
		}
		void register_free(void* ptr)
		{
			if (ignore) return;
			std::lock_guard<std::mutex> lock(mutex);
			ignore = true;
			auto it = allocations.find(ptr);
			if (it == allocations.end()) return;
			memory_group_stats& group_stat = group_stats[it->second.group];
			group_stat.total_freed += it->second.size;
			++group_stat.free_count;
			allocations.erase(it);
			ignore = false;
		}
	} memory_info;
	thread_local bool memory_stats::ignore = true;


	void* sugoma_malloc(size_t size, int group)
	{
		void* ptr = malloc(size);
		if (!ptr) return nullptr;
		memory_info.register_alloc(ptr, size, group);
		return ptr;
	}
	void* sugoma_malloc(size_t size)
	{
		return sugoma_malloc(size, 0);
	}
	void sugoma_free(void* ptr) 
	{
		free(ptr);
		memory_info.register_free(ptr);
	}
	void* sugoma_realloc(void* ptr, size_t size, int group) 
	{
		if (!ptr) return sugoma_malloc(size, group);
		void* new_ptr = realloc(ptr, size);
		if (!new_ptr) return nullptr;
		memory_info.register_free(ptr);
		memory_info.register_alloc(new_ptr, size, memory_info.allocations[ptr].group);
		return new_ptr;
	}
	size_t sugoma_mem_usage(int group) { auto& a = memory_info.group_stats[group]; return a.total_allocated - a.total_freed; }
	size_t sugoma_mem_allocated(int group) { return memory_info.group_stats[group].total_allocated; }
	size_t sugoma_mem_freed(int group) { return memory_info.group_stats[group].total_freed; }
	size_t sugoma_mem_alloc_count(int group) { return memory_info.group_stats[group].alloc_count; }
	size_t sugoma_mem_free_count(int group) { return memory_info.group_stats[group].free_count; }

}
void* operator new(std::size_t size) {
	if (void* ptr = sugoma::core::sugoma_malloc(size, 0)) return ptr;
	throw std::bad_alloc();
}
void operator delete(void* ptr) noexcept {
	sugoma::core::sugoma_free(ptr);
}