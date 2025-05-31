#pragma once
namespace sugoma::core 
{
	void* sugoma_malloc(size_t size, int group = 0);
	void sugoma_free(void* ptr);
	void* sugoma_realloc(void* ptr, size_t size, int group = 0);

	size_t sugoma_mem_usage(int group = 0);
	size_t sugoma_mem_allocated(int group = 0);
	size_t sugoma_mem_freed(int group = 0);
	size_t sugoma_mem_alloc_count(int group = 0);
	size_t sugoma_mem_free_count(int group = 0);

}