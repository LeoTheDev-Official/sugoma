#pragma once
#include <cstdint>
namespace sugoma::core 
{
	class Buffer 
	{
	public:
		Buffer() = default;
		Buffer(size_t size);
		void Free();
		uint8_t* Data();
		const uint8_t* Data() const;
		size_t Size() const;
		uint8_t* Cursor() const;
		void Write(void* source, size_t size);
		void ResetCursor();
		void Read(void* destination, size_t size);

	private:
		uint8_t* m_buffer = nullptr;
		size_t m_size = 0;
		uint8_t* m_cursor = 0;
	};
}