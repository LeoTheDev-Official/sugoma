#include "pipeline.h"

#include <GL/glew.h>

namespace sugoma::graphics
{
	GLHandle PipelineStage::Handle() const { return m_handle; }
	PipelineStage::~PipelineStage() { if (m_handle) glDeleteShader(m_handle); }
	GLHandle Pipeline::Handle() const { return m_handle; }
	const std::unordered_map<std::string, PipelineParameterInfo>& Pipeline::Parameters() const { return m_params; }
	void Pipeline::Invalidate()
	{
		m_params.clear();

		int32_t param_count;
		glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &param_count);
		for (int32_t i = 0; i < param_count; ++i) 
		{
			char name[256];
			int32_t length = 0;
			int32_t size = 0;
			GLEnum type = 0;

			glGetActiveUniform(m_handle, i, sizeof(name), &length, &size, &type, name);
			int32_t location = glGetUniformLocation(m_handle, name);

			PipelineParameterInfo param{};
			param.count = size;
			param.name = name;
			param.location = location;
			param.type = (PipelineAttributeType)type;

			m_params[param.name] = param;
		}
	}
}
