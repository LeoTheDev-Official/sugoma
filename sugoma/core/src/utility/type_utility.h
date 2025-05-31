#pragma once
#include <cstdint>
#include <typeinfo>

namespace sugoma::core 
{
	using TypeID = uint16_t;
	TypeID __sugoma_get_type_id(const std::type_info& info);
	TypeID __sugoma_register_type_id(const std::type_info& info, size_t size);
	class Type 
	{
	public:
		Type(TypeID type);
		const char* Name() const;
		size_t Size() const;
		operator TypeID() const;
	private:
		TypeID m_id = 0;
	};
	template<typename T>
	Type typeof() 
	{
		TypeID t = __sugoma_get_type_id(typeid(T));
		if (!t) t = __sugoma_register_type_id(typeid(T), sizeof(T));
		return t;
	}
}