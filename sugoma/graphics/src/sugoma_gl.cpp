#include "sugoma_gl.h"
#include <GL/glew.h>
#include <format>
#include "debug/logger.h"
namespace sugoma::graphics
{
	std::string gen_define_source(const std::vector<std::string_view>& defines) 
	{
		std::string src;
		for (auto& def : defines) src += std::format("#define {}", def) + '\n';
		return src;
	}

	GLHandle GLUtility::CompileShader(const std::string& src, GLEnum kind, const std::vector<std::string_view>& defines, const char* version)
	{
		std::string version_def = std::format("#version {}\n", version);
		std::string def_src = gen_define_source(defines);

		GLHandle shader = glCreateShader(kind);
		const char* sources[] = { version_def.c_str(), def_src.c_str(), src.c_str() };
		glShaderSource(shader, sizeof(sources) / sizeof(sources[0]), sources, nullptr);
		glCompileShader(shader);

		GLint compile_status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
		if (!compile_status)
		{
			GLint logLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

			std::string log(logLength, ' ');
			glGetShaderInfoLog(shader, logLength, nullptr, &log[0]);

			sugoma_error("Shader compilation failed.\n" << log);
			glDeleteShader(shader);
			return 0;
		}
		return shader;
	}
	bool GLUtility::LinkProgram(GLHandle handle)
	{
		glLinkProgram(handle);
		int link_status = 0;
		glGetProgramiv(handle, GL_LINK_STATUS, &link_status);
		if (!link_status)
		{
			char infoLog[512];
			glGetProgramInfoLog(handle, 512, NULL, infoLog);
			sugoma_error("Pipeline link failure : " << infoLog);
			return false;
		}
		return true;
	}
}
