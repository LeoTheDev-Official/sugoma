#include <memory>
#include "buffer.h"
#include "sugoma_memory.h"
namespace sugoma::core 
{
	Buffer::Buffer(size_t size) : m_size(size) { m_buffer = (uint8_t*)sugoma_malloc(size); m_cursor = m_buffer; }
	void Buffer::Free() { if (m_buffer) sugoma_free(m_buffer); }
	uint8_t* Buffer::Data() { return m_buffer; }
	const uint8_t* Buffer::Data() const { return m_buffer; }
	size_t Buffer::Size() const { return m_size; }
	uint8_t* Buffer::Cursor() const { return m_cursor; }
	void Buffer::ResetCursor() { m_cursor = m_buffer; }
	void Buffer::Write(void* source, size_t size) { memcpy(m_cursor, source, size); m_cursor += size; }
	void Buffer::Read(void* destination, size_t size) { memcpy(destination, m_cursor, size); m_cursor += size; }
}