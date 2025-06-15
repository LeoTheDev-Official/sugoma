#pragma once
#include "shader_types.h"
#include "shader.h"
namespace sugoma::graphics 
{
    struct StructField
    {
        std::string_view name;
        FieldType type;
		uint32_t arraySize = 0; //-1 -> dynamic array
    };
    struct StructInfo
    {
        std::string_view name;
        std::vector<StructField> fields;
    };
}