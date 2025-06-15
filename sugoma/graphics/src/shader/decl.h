#pragma once
#include "shader_types.h"
#include "shader.h"
#include "struct.h"
#include "transfer.h"
#include "ubuffer.h"
#include "sbuffer.h"
namespace sugoma::graphics
{
	class StageDecl 
	{
	public:
		StageDecl() = delete;
		StageDecl(ShaderStage stage);
	};
	class StageSource
	{
	public:
		StageSource() = delete;
		StageSource(const char* source);
	};
	class StructDecl 
	{
	public:
		StructDecl() = delete;
		StructDecl(std::string_view name, const std::vector<StructField>& fields);
	};
	class InputDecl
	{
	public:
		InputDecl() = delete;
		InputDecl(std::string_view name, FieldType type, uint32_t arraySize = 0, bool flat = false);
	};
	class OutputDecl
	{
	public:
		OutputDecl() = delete;
		OutputDecl(std::string_view name, FieldType type, uint32_t arraySize = 0, bool flat = false);
	};
	class UniformBufferDecl
	{
	public:
		UniformBufferDecl() = delete;
		UniformBufferDecl(std::string_view name, std::string_view blockName, BufferLayout layout, const std::vector<StructField>& fields, uint32_t binding);
	};
	class ShaderStorageBufferDecl
	{
	public:
		ShaderStorageBufferDecl() = delete;
		ShaderStorageBufferDecl(std::string_view name, std::string_view blockName, BufferLayout layout, const std::vector<StructField>& fields, uint32_t binding);
	};
	class MaterialDecl 
	{
	public:
		MaterialDecl() = delete;
		MaterialDecl(const std::vector<MaterialFieldInfo>& fields);
	};
	
}