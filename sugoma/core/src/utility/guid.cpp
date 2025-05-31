#include <random>
#include "guid.h"
#include "debug/logger.h"
namespace sugoma::core 
{
	void byte_to_hex(uint8_t byte, char* buff) 
	{
		static char chars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
		uint8_t a = byte & 0x0F;
		uint8_t b = (byte & 0xF0) >> 0x04;
		buff[0] = chars[a];
		buff[1] = chars[b];
	}
	uint8_t hex_to_nibble(char c) 
	{
		c = std::toupper(c);
		if (c >= 'A') return 10 + c - 'A';
		return c - '0';
	}
	uint8_t hex_to_byte(char ca, char cb) 
	{
		uint8_t a = hex_to_nibble(ca);
		uint8_t b = hex_to_nibble(cb);
		return a | (b << 0x04);
	}
	GUID::GUID(const std::string& string)
	{
		uint8_t* buff = reinterpret_cast<uint8_t*>(&m_values);
		for (uint8_t i = 1; i < string.length(); i += 2)
			buff[i / 2] = hex_to_byte(string[i - 1], string[i]);
	}
	GUID::GUID(uint8_t* buffer) 
	{
		memcpy(m_values.data(), buffer, sizeof(GUID));
	}
	GUID GUID::NewGUID()
	{
		static thread_local std::mt19937_64 rng(std::random_device{}());
		GUID id{};
		for (auto& a : id.m_values) a = rng();
		return id;
	}
	std::string GUID::ToString() const
	{
		static constexpr auto len = sizeof(m_values) * 2;
		std::string result(len, '0');
		const uint8_t* data = reinterpret_cast<const uint8_t*>(m_values.data());
		for (uint8_t i = 0; i < sizeof(m_values); ++i) byte_to_hex(data[i], result.data() + 2 * i);
		return result;
	}
}
namespace std {
	std::size_t hash<sugoma::core::GUID>::operator()(const sugoma::core::GUID& guid) const noexcept {
		std::size_t h1 = std::hash<uint64_t>{}(guid.m_values[0]);
		std::size_t h2 = std::hash<uint64_t>{}(guid.m_values[1]);
		return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
	}
}