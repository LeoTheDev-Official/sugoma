#pragma once
#include "shader_types.h"

namespace sugoma::graphics 
{
	struct TransferField
	{
		std::string_view name;
		FieldType type;
		uint32_t arraySize = 0;
		bool flat = false;
	};
}