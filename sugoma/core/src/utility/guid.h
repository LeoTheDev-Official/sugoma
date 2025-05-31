#pragma once
#include <cstdint>
#include <array>
#include <string>
namespace sugoma::core 
{
	class GUID 
	{
	public:
		GUID() = default;
		GUID(const std::string& string);
		GUID(uint8_t* buffer);
		static GUID NewGUID();
		auto operator<=>(const GUID&) const = default;
		std::string ToString() const;
	private:
		std::array<uint64_t, 2> m_values{};
		friend struct std::hash<sugoma::core::GUID>;
	};
}
namespace std {
	template <>
	struct hash<sugoma::core::GUID> {
		std::size_t operator()(const sugoma::core::GUID& guid) const noexcept;
	};
}