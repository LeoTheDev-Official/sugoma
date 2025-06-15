#include "decl.h"

namespace sugoma::graphics 
{
	extern ShaderInfo* s_shader_context;
	ShaderStage s_active_stage = ShaderStage::Global;
	StructDecl::StructDecl(std::string_view name, const std::vector<StructField>& fields) { s_shader_context->stages[s_active_stage].structs.push_back({ name, fields }); }
	StageDecl::StageDecl(ShaderStage stage) { s_active_stage = stage; }
	StageSource::StageSource(const char* source) { s_shader_context->stages[s_active_stage].source += source; }
	InputDecl::InputDecl(std::string_view name, FieldType type, uint32_t arraySize, bool flat) { s_shader_context->stages[s_active_stage].inputs.push_back({ name, type, arraySize, flat }); }
	OutputDecl::OutputDecl(std::string_view name, FieldType type, uint32_t arraySize, bool flat) { s_shader_context->stages[s_active_stage].outputs.push_back({ name, type, arraySize, flat }); }
	UniformBufferDecl::UniformBufferDecl(std::string_view name, std::string_view blockName, BufferLayout layout, const std::vector<StructField>& fields, uint32_t binding)
	{ s_shader_context->stages[s_active_stage].uniformBuffers.push_back(UniformBufferInfo{ name, blockName, layout, binding, fields}); }
	ShaderStorageBufferDecl::ShaderStorageBufferDecl(std::string_view name, std::string_view blockName, BufferLayout layout, const std::vector<StructField>& fields, uint32_t binding)
	{ s_shader_context->stages[s_active_stage].storageBuffers.push_back(ShaderStorageBufferInfo{ name, blockName, layout, binding, fields }); }
	MaterialDecl::MaterialDecl(const std::vector<MaterialFieldInfo>& fields) { s_shader_context->material.fields = fields; }
}
