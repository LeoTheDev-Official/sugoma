#include "shader.h"

namespace sugoma::graphics
{
    ShaderInfo* s_shader_context = nullptr;
    Shader::Shader(std::string_view name)
    {
		s_shader_context = &m_shaderInfo;
		s_shader_context->name = name;
    }
    Shader::~Shader()
    {
		if (s_shader_context == &m_shaderInfo) s_shader_context = nullptr;
    }
}