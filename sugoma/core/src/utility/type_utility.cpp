#include <vector>
#include <unordered_map>
#include <typeindex>
#include "type_utility.h"

namespace sugoma::core 
{
	struct TypeInfo 
	{
		const char* name;
		size_t size;
	};

	static std::vector<TypeInfo> s_types = { {"void", 0} };
	static std::unordered_map<std::type_index, TypeID> s_typeset;

	inline const TypeInfo& __internal_type_info(TypeID type) { return s_types[type]; }

	Type::Type(TypeID type) : m_id(type) {}
	const char* Type::Name() const { return __internal_type_info(m_id).name; }
	size_t Type::Size() const { return __internal_type_info(m_id).size; }
	Type::operator TypeID() const { return m_id; }
	TypeID __sugoma_get_type_id(const std::type_info& info)
	{
		auto it = s_typeset.find(info);
		if (it != s_typeset.end())
			return it->second;
		return 0;
	}

	TypeID __sugoma_register_type_id(const std::type_info& info, size_t size)
	{
		auto it = s_typeset.find(info);
		if (it != s_typeset.end())
			return it->second;
		TypeID id = (TypeID)s_typeset.size() + 1;
		s_typeset[info] = id;
		s_types.push_back({ info.name(), size });
		return id;
	}

}

